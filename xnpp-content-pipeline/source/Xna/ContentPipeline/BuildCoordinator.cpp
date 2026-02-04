#include "Xna/ContentPipeline/BuildCoordinator.hpp"
#include <Xna/CSharp/IO/FileStream.hpp>
#include <algorithm>
#include <format>
#include <stdexcept>
#include "Xna/ContentPipeline/PathUtils.hpp"
#include "Xna/ContentPipeline/XnaContentProcessorContext.hpp"
#include "Xna/ContentPipeline/XnaContentImporterContext.hpp"

namespace Xna {
	struct Resources final {
		static constexpr std::string_view BuildLogCompiling = "Compiling {0}";
		static constexpr std::string_view RebuildAll = "Rebuilding all content";
		static constexpr std::string_view SourceAssetNotFound = "Missing asset '{0}'.";
		static constexpr std::string_view InvalidAssetName = "Invalid asset name '{0}'.";
		static constexpr std::string_view RebuildNewAsset = "Rebuilding because asset is new";
		static constexpr std::string_view CompiledAssetFilenameConflict = "More than one output asset is named '{0}'. You must explicitly specify asset names to resolve the conflict.";
		static constexpr std::string_view RebuildReferenceRenamed = "Rebuilding because referenced asset {0} was renamed.";
		static constexpr std::string_view BuildLogAlreadyUpToDate = "{0} is already up to date";
		static constexpr std::string_view BuildLogBuilding = "Building {0} -> {1}";
		static constexpr std::string_view BuildLogProcessing = "Processing {0} with {1}";
		static constexpr std::string_view RebuildNotBuilt = "Rebuilding because asset is out of date";
		static constexpr std::string_view RebuildDirtyDependency = "Rebuilding because {0} has changed";
		static constexpr std::string_view RebuildMissingOutput = "Rebuilding because {0} is missing";
		static constexpr std::string_view BuildAndLoadAssetNotIntermediate = "The BuildAndLoadAsset function can only be used with intermediate content files.";
		static constexpr std::string_view BuildLogDeserializing = "Deserializing {0}";
		static constexpr std::string_view BuildLogImporting = "Importing {0} with {1}";
		static constexpr std::string_view BuildLogSerializing = "Serializing {0}";
	};

	BuildCoordinator::BuildCoordinator(
		ContentBuildLogger& buildLogger,
		BuildCoordinatorSettings& settings,
		TimestampCache timestampCache) {

		impl = std::make_shared<Implementation>();

		if (buildLogger == nullptr)
			throw std::invalid_argument("BuildLogger is null.");

		settings.InitializePaths();
		buildLogger.LoggerRootDirectory(settings.LoggerRootDirectory);

		if (timestampCache == nullptr)
			timestampCache = TimestampCache();

		impl->buildLogger = buildLogger;
		impl->settings = settings;
		impl->timestampCache = timestampCache;

		std::string rebuildReason;
		impl->buildItems = BuildItemCollection::Load(settings.CacheFilename(), rebuildReason);
		bool flag1 = impl->buildItems.UpdateSettings(settings, rebuildReason);
		bool flag2 = impl->buildItems.UpdateAssemblies(timestampCache, rebuildReason);

		if (flag1 || flag2) {
			impl->buildItems.Clear();
			impl->buildItemsChanged = true;
		}

		if (settings.RebuildAll) {
			buildLogger.LogMessage(Resources::RebuildAll);
		}
		else {
			if (rebuildReason.empty())
				return;
			settings.RebuildAll = true;
			buildLogger.LogMessage("{0}", rebuildReason);
		}
	}

	Xna::ImporterManager BuildCoordinator::ImporterManager() {
		if (impl->importerManager == nullptr)
			impl->importerManager = Xna::ImporterManager();

		return impl->importerManager;
	}
	Xna::ProcessorManager BuildCoordinator::ProcessorManager() {
		if (impl->processorManager == nullptr)
			impl->processorManager = Xna::ProcessorManager();

		return impl->processorManager;
	}
	Xna::ContentCompiler BuildCoordinator::ContentCompiler() {
		if (impl->contentCompiler == nullptr)
			impl->contentCompiler = Xna::ContentCompiler();

		return impl->contentCompiler;
	}

	void BuildCoordinator::Dispose() {
		if (impl->buildItems == nullptr)
			return;
		if (impl->buildItemsChanged) {
			impl->buildItems.DeleteStaleOutputFiles();
			impl->buildItems.Save(impl->settings.CacheFilename());
		}
		impl->buildItems = nullptr;
	}

	void BuildCoordinator::RequestBuild(
		std::filesystem::path const& sourceFilename,
		std::filesystem::path const& assetName,
		std::string const& importerName,
		std::string const& processorName,
		OpaqueDataDictionary const& processorParameters) {

		if (sourceFilename.empty())
			throw std::invalid_argument("Source filename is empty.");

		impl->buildLogger.PushFile(sourceFilename);
		auto request = BuildRequest();
		request.SourceFilename = sourceFilename;
		request.AssetName = assetName;
		request.ImporterName = importerName;
		request.ProcessorName = processorName;
		if (processorParameters != nullptr) {
			for (auto& [k, v] : processorParameters)
				request.ProcessorParameters.Add(k, v);
		}

		RequestBuild(request);
		impl->buildLogger.PopFile();
	}


	BuildItem BuildCoordinator::RequestBuild(BuildRequest& request) {		
		if (request.SourceFilename.empty())
			throw std::invalid_argument("Source filename in BuildRequest is empty.");
		if (!impl->timestampCache.FileExists(request.SourceFilename))
			throw std::runtime_error(std::format(Resources::SourceAssetNotFound, request.SourceFilename.string()));
		request.SourceFilename = GetRelativePath(request.SourceFilename);

		if (!request.AssetName.empty()) {
			const auto request1 = request.AssetName.string();
			if (request.AssetName.has_root_path() || request1.find("..") != std::string::npos)
				throw std::runtime_error(std::format(Resources::InvalidAssetName, request.AssetName.string()));
		}
		if (request.ImporterName.empty())
			request.ImporterName = ImporterManager().GuessFromFilename(request.SourceFilename);

		auto buildItem = impl->buildItems.Find(request);

		if (buildItem && buildItem.BuildRequest().AssetName.empty() && !request.AssetName.empty()) {
			RemoveBuildItem(buildItem);
			buildItem = nullptr;
		}
		if (!buildItem) {
			buildItem = BuildItem();
			buildItem.BuildRequest() = request;
			buildItem.OutputFilename() = ChooseOutputFilename(request);
			buildItem.SetNeedsRebuild(std::string(Resources::RebuildNewAsset));
			impl->buildItems.Add(buildItem);
			impl->buildItemsChanged = true;
		}

		impl->buildItems.WantItem(buildItem);
		return buildItem;
	}

	std::filesystem::path BuildCoordinator::ChooseOutputFilename(BuildRequest& request) {
		std::filesystem::path str1 = impl->settings.OutputDirectory;;
		std::filesystem::path str2 = ".xnb";

		if (((int)request.BuildOptions & (int)BuildOptions::IntermediateFile) != (int)BuildOptions::None) {
			str1 = impl->settings.IntermediateDirectory;
			str2 = ".xml";
		}

		std::filesystem::path relativePath1;

		if (request.AssetName.empty())
		{
			if (!request.SourceFilename.has_root_path()) {
				auto str3 = request.SourceFilename.parent_path();
				auto relativePath2 = GetRelativePath(impl->settings.IntermediateDirectory);

				if (!str3.empty() && str3.native().starts_with(relativePath2.native())) //TODO: [!] StringComparison.OrdinalIgnoreCase
					str3 = str3.native().substr(relativePath2.native().size());

				if (!str3.empty())
					str1 = str1 / str3;
			}

			auto assetNameStub = GetAssetNameStub(request.SourceFilename);
			size_t num = 0;

			do {
				auto str1_1 = str1;
				relativePath1 = GetRelativePath(str1_1 / assetNameStub / "_" / std::to_string(num) / str2);
				++num;
			} while (impl->buildItems.Find(relativePath1) != nullptr);
		}
		else {
			auto str1_2 = str1;
			relativePath1 = GetRelativePath(str1_2 / request.AssetName.replace_extension(str2));
			auto buildItem = impl->buildItems.Find(relativePath1);

			if (buildItem != nullptr) {
				if (buildItem.IsWanted())
					throw std::runtime_error(std::format(Resources::CompiledAssetFilenameConflict, relativePath1.string()));
				RemoveBuildItem(buildItem);
			}
		}

		return relativePath1;
	}

	void BuildCoordinator::SerializeOutputAsset(BuildItem& item, std::any const& assetData, std::filesystem::path const& outputFilename) {
		impl->buildLogger.LogMessage(Resources::BuildLogCompiling, item.OutputFilename().string());
		auto output = std::make_shared<CSharp::FileStream>(outputFilename, CSharp::FileMode::Create);
		ContentCompiler().Compile(output, assetData, impl->settings.TargetPlatform, impl->settings.TargetProfile, impl->settings.CompressContent, impl->settings.OutputDirectory, outputFilename);
		impl->rebuiltFiles.push_back(outputFilename);
	}

	std::filesystem::path BuildCoordinator::RemoveNonAnsiCharacters(std::filesystem::path const& value) {
		auto value1 = value.native();

		value1.erase(
			std::remove_if(value1.begin(), value1.end(),
				[](unsigned char c)
				{
					return c < 0x20 || c > 0x7E;
				}),
			value1.end()
		);

		return value1;
	}

	std::filesystem::path BuildCoordinator::GetAssetNameStub(std::filesystem::path sourceFilename) {
		auto assetNameStub = RemoveNonAnsiCharacters(sourceFilename.replace_extension());
		if (assetNameStub.native().size() > 32)
			assetNameStub = assetNameStub.native().substr(0, 32);

		return assetNameStub;
	}

	void BuildCoordinator::RemoveBuildItem(BuildItem& item) {
		impl->buildItems.Remove(item);
		auto absolutePath = GetAbsolutePath(item.OutputFilename());
		auto it = std::find(impl->rebuiltFiles.begin(), impl->rebuiltFiles.end(), absolutePath);
		if (it != impl->rebuiltFiles.end())
			impl->rebuiltFiles.erase(it);

		impl->timestampCache.Remove(absolutePath);
		PathUtils::SafeDeleteFile(absolutePath);
		for (auto& buildItem : impl->buildItems) {
			if (buildItem.Dependencies().contains(item.OutputFilename()) || buildItem.Requests().Contains(item.OutputFilename())) {
				buildItem.Dependencies().erase(item.OutputFilename());
				buildItem.Requests().Remove(item.OutputFilename());
				buildItem.SetNeedsRebuild(std::format(Resources::RebuildReferenceRenamed, item.OutputFilename().string()));
				buildItem.SourceTimestamp() = TimePoint::min(); //DateTime.MinValue;

				if (buildItem.IsBuilt()) {
					it = std::find(impl->rebuiltFiles.begin(), impl->rebuiltFiles.end(), buildItem.OutputFilename());
					if (it != impl->rebuiltFiles.end())
						impl->rebuiltFiles.erase(it);

					buildItem.IsBuilt() = false;
					buildItem.IsWanted() = false;
					impl->buildItems.WantItem(buildItem);
				}
			}
		}
	}

	std::any BuildCoordinator::BuildAsset(BuildItem& item) {
		if (item.IsBuilt())
			return {};

		try
		{
			impl->buildLogger.PushFile(GetAbsolutePath(item.BuildRequest().SourceFilename));
			auto obj = BuildAssetWorker(item);
			item.IsBuilt() = true;
			impl->buildLogger.PopFile();
			return obj;
		}
		catch (std::exception& ex) {
			item.SourceTimestamp() = TimePoint::min();
			impl->buildItemsChanged = true;
			std::cout << ex.what() << std::endl;
		}
	}

	std::any BuildCoordinator::BuildAssetWorker(BuildItem& item) {
		std::string rebuildReason;
		if (!NeedIncrementalBuild(item, rebuildReason)) {
			impl->buildLogger.LogMessage(Resources::BuildLogAlreadyUpToDate, item.OutputFilename().string());
			return {};
		}

		auto flag1 = GetAbsolutePath(item.BuildRequest().SourceFilename).native().starts_with(impl->settings.IntermediateDirectory.native()); //TODO: [!] StringComparison.OrdinalIgnoreCase
		auto flag2 = (int)item.BuildRequest().BuildOptions == ((int)BuildOptions::IntermediateFile | (int)BuildOptions::DontCacheImportedData);

		if (!flag1 && !flag2) {
			impl->buildLogger.LogImportantMessage(Resources::BuildLogBuilding, item.BuildRequest().SourceFilename.string(), item.OutputFilename().string());

			if (!rebuildReason.empty())
				impl->buildLogger.LogMessage("{0}", rebuildReason);
		}

		impl->buildItemsChanged = true;
		item.Dependencies().clear();
		item.Requests().Clear();
		item.ExtraOutputFiles().clear();

		auto obj = ImportAsset(item);
		auto& processorName = item.BuildRequest().ProcessorName;

		if (!processorName.empty()) {
			auto contentIdentity = ContentIdentity(impl->settings.GetAbsolutePath(item.BuildRequest().SourceFilename));
			auto instance = ProcessorManager().GetInstance(processorName, item.BuildRequest().ProcessorParameters, contentIdentity, impl->buildLogger);
			impl->buildLogger.LogMessage(Resources::BuildLogProcessing, item.BuildRequest().SourceFilename.string(), processorName);

			//Em C# aqui tinha algo como if (!instance->InputType().IsAssignableFrom(obj.GetType())) throw...			

			auto context = XnaContentProcessorContext(*this, item, impl->buildLogger, impl->settings.TargetPlatform, impl->settings.TargetProfile, impl->settings.BuildConfiguration);
			
			const auto obj1 = instance->Process(obj, context);
			obj.reset();
			obj = obj1;
		}

		SerializeAsset(item, obj);
		return obj;
	}

	bool BuildCoordinator::NeedIncrementalBuild(BuildItem& item, std::string& rebuildReason) {
		if (impl->settings.RebuildAll)
			return true;
		if (!item.NeedsRebuild().has_value())
		{
			auto it = std::find(impl->incrementalRecurseDetector.begin(), impl->incrementalRecurseDetector.end(), item);
			if (it != impl->incrementalRecurseDetector.end())
				return false;

			impl->incrementalRecurseDetector.push_back(item);
			if (TimestampsHaveChanged(item, rebuildReason))
				item.SetNeedsRebuild(rebuildReason);
			else
				item.ClearNeedsRebuild();

			impl->incrementalRecurseDetector.erase(it);
		}
		else if (item.NeedsRebuild().value())
			rebuildReason = item.RebuildReason();

		return item.NeedsRebuild().value();
	}

	bool BuildCoordinator::TimestampsHaveChanged(BuildItem& item, std::string& rebuildReason) {
		if (item.SourceTimestamp() == TimePoint::min()) {
			rebuildReason = Resources::RebuildNotBuilt;
			return true;
		}
		if (impl->timestampCache.GetTimestamp(GetAbsolutePath(item.BuildRequest().SourceFilename)) != item.SourceTimestamp())
		{
			rebuildReason = std::format(Resources::RebuildDirtyDependency, item.BuildRequest().SourceFilename.string());
			return true;
		}
		if (!impl->timestampCache.FileExists(GetAbsolutePath(item.OutputFilename())))
		{
			rebuildReason = std::format(Resources::RebuildMissingOutput, item.OutputFilename().string());
			return true;
		}

		for (auto& extraOutputFile : item.ExtraOutputFiles()) {
			if (!impl->timestampCache.FileExists(GetAbsolutePath(extraOutputFile))) {
				rebuildReason = std::format(Resources::RebuildMissingOutput, extraOutputFile.string());
				return true;
			}
		}
		for (auto& [key, value] : item.Dependencies()) {
			auto buildItem = impl->buildItems.Find(key);

			if (buildItem != nullptr) {
				if (NeedIncrementalBuild(buildItem, rebuildReason))
					return true;
			}
			else if (impl->timestampCache.GetTimestamp(GetAbsolutePath(key)) != value) {
				rebuildReason = std::format(Resources::RebuildDirtyDependency, key.string());
				return true;
			}
		}
		return false;
	}

	std::any BuildCoordinator::BuildAndLoadAsset(BuildItem& item, BuildItem& requestingItem) {
		if (item == nullptr)
			throw std::invalid_argument("Item is null.");

		if (((int)item.BuildRequest().BuildOptions & (int)BuildOptions::IntermediateFile) == (int)BuildOptions::None)
			throw std::runtime_error(std::string(Resources::BuildAndLoadAssetNotIntermediate));
		auto obj = BuildAsset(item);

		if (!obj.has_value()) {
			impl->buildLogger.LogMessage(Resources::BuildLogDeserializing, item.OutputFilename().string());
			auto absolutePath = GetAbsolutePath(item.OutputFilename());

			//TODO: [!] uso de XML
			//using (XmlReader input = XmlReader.Create(absolutePath))
			//	obj = IntermediateSerializer.Deserialize<object>(input, absolutePath);
		}
		AddDependency(requestingItem, GetAbsolutePath(item.OutputFilename()));
		return obj;
	}

	std::any BuildCoordinator::ImportAsset(BuildItem& item) {
		auto absolutePath = GetAbsolutePath(item.BuildRequest().SourceFilename);

		if (!impl->timestampCache.FileExists(absolutePath))
			throw std::runtime_error(std::format(Resources::SourceAssetNotFound, absolutePath.string()));

		item.SourceTimestamp() = impl->timestampCache.GetTimestamp(absolutePath);
		auto& importerName = item.BuildRequest().ImporterName;

		const auto cachedData = ImporterManager().GetImporterAttribute(importerName).CacheImportedData();
		const auto options = ((int)item.BuildRequest().BuildOptions & (int)BuildOptions::DontCacheImportedData) == (int)BuildOptions::None;

		if (cachedData && options)
			return ImportAssetViaCache(item, importerName);

		return ImportAssetDirectly(item, importerName);
	}

	std::any BuildCoordinator::ImportAssetDirectly(BuildItem& item, std::string const& importerName) {
		auto instance = ImporterManager().GetInstance(importerName);
		auto absolutePath = GetAbsolutePath(item.BuildRequest().SourceFilename);
		auto context = XnaContentImporterContext(*this, item, impl->buildLogger);
		impl->buildLogger.LogMessage(Resources::BuildLogImporting, item.BuildRequest().SourceFilename.string(), importerName); //instance.GetType
		return instance->Import(absolutePath, context);
	}

	std::any BuildCoordinator::ImportAssetViaCache(BuildItem& item, std::string const& importerName) {
		auto request = BuildRequest();
		request.SourceFilename = GetAbsolutePath(item.BuildRequest().SourceFilename);
		request.ImporterName = importerName;
		request.BuildOptions = static_cast<BuildOptions>((int)BuildOptions::IntermediateFile | (int)BuildOptions::DontCacheImportedData);
		auto item1 = RequestBuild(request);
		auto value = BuildAndLoadAsset(item1, item);

		return value;
	}

	void BuildCoordinator::SerializeAsset(BuildItem& item, std::any const& assetData) {
		PathUtils::CreateDirectory(GetAbsolutePath(item.OutputFilename()).parent_path());
		auto absolutePath = GetAbsolutePath(item.OutputFilename());
		impl->timestampCache.Remove(absolutePath);

		try {
			if (((int)item.BuildRequest().BuildOptions & (int)BuildOptions::IntermediateFile) != (int)BuildOptions::None)
				SerializeIntermediateAsset(item, assetData, absolutePath);
			else
				SerializeOutputAsset(item, assetData, absolutePath);
		}
		catch (std::exception const& ex)
		{
			PathUtils::SafeDeleteFile(absolutePath);
			std::string error{ "Serialize file failed. Ex: " };
			error.append(ex.what());

			throw std::runtime_error(error);
		}
	}

	void BuildCoordinator::SerializeIntermediateAsset(BuildItem& item, std::any const& assetData, std::filesystem::path const& outputFilename) {
		impl->buildLogger.LogMessage(Resources::BuildLogSerializing, item.OutputFilename().string());
		//TODO: [!] uso de xml
		//using (XmlWriter output = XmlWriter.Create(outputFilename, new XmlWriterSettings()
		//	{
		//		Indent = true
		//	}))
		//	IntermediateSerializer.Serialize<object>(output, assetData, outputFilename);
	}

	void BuildCoordinator::RunTheBuild() {
		impl->buildItems.ReverseWantedItems(0);
		BuildItem buildItem1;
		while ((buildItem1 = impl->buildItems.PopWantedItem()) != nullptr)
		{
			if (!buildItem1.IsBuilt()) {
				auto wantedItemsCount = impl->buildItems.WantedItemsCount();
				BuildAsset(buildItem1);
				for (auto& [key, _] : buildItem1.Dependencies()) {
					auto buildItem2 = impl->buildItems.Find(key);
					if (buildItem2 != nullptr)
						impl->buildItems.WantItem(buildItem2);
				}

				for (auto& request : buildItem1.Requests())
					impl->buildItems.WantItem(impl->buildItems.Find(request));

				impl->buildItems.ReverseWantedItems(wantedItemsCount);
			}
		}
		impl->buildItemsChanged |= impl->buildItems.RemoveUnwantedItems();
		impl->buildItems.BuildSuccessful(true);
	}

	std::vector<std::filesystem::path> BuildCoordinator::GetOutputFiles() const {
		std::vector<std::filesystem::path> stringList;

		for (auto& buildItem : impl->buildItems) {
			if (((int)buildItem.BuildRequest().BuildOptions & (int)BuildOptions::IntermediateFile) == (int)BuildOptions::None)
			{
				stringList.push_back(GetAbsolutePath(buildItem.OutputFilename()));

				for (auto& extraOutputFile : buildItem.ExtraOutputFiles())
					stringList.push_back(GetAbsolutePath(extraOutputFile));
			}
		}

		return stringList;
	}

	std::vector<std::filesystem::path> BuildCoordinator::GetRebuiltFiles()  const {
		return impl->rebuiltFiles;
	}

	std::vector<std::filesystem::path> BuildCoordinator::GetIntermediateFiles() const {
		std::vector<std::filesystem::path> stringList;
		stringList.push_back(impl->settings.CacheFilename());

		for (auto& buildItem : impl->buildItems) {
			if (((int)buildItem.BuildRequest().BuildOptions & (int)BuildOptions::IntermediateFile) != (int)BuildOptions::None)
				stringList.push_back(GetAbsolutePath(buildItem.OutputFilename()));
		}

		return stringList;
	}

	std::unordered_map<std::filesystem::path, TimePoint>  BuildCoordinator::GetDependencyTimestamps() const {
		std::unordered_map<std::filesystem::path, TimePoint> timestamps;

		for (auto& buildItem : impl->buildItems) {
			timestamps[GetAbsolutePath(buildItem.BuildRequest().SourceFilename)] = buildItem.SourceTimestamp();

			for (auto& [key, value] : buildItem.Dependencies()) {
				if (impl->buildItems.Find(key) == nullptr)
					timestamps[GetAbsolutePath(key)] = value;
			}
		}
		impl->buildItems.GetAssemblyTimestamps(timestamps);
		return timestamps;
	}

	std::vector<std::pair<std::filesystem::path, std::string>> BuildCoordinator::GetWarnings()  const {
		return impl->buildItems.GetDuplicateWarnings();
	}

	void BuildCoordinator::AddDependency(BuildItem& item, std::filesystem::path const& filename) {
		auto relativePath = GetRelativePath(filename);
		auto timestamp = impl->timestampCache.GetTimestamp(filename);
		item.Dependencies()[relativePath] = timestamp;
	}

	void BuildCoordinator::AddExtraOutput(BuildItem& item, std::filesystem::path const& filename) {
		auto relativePath = GetRelativePath(filename);
		for (auto& extraOutputFile : item.ExtraOutputFiles()) {
			if (extraOutputFile == relativePath) //TODO: StringComparison.OrdinalIgnoreCase
				return;
		}

		item.ExtraOutputFiles().push_back(relativePath);
	}

	std::filesystem::path BuildCoordinator::GetRelativePath(std::filesystem::path const& path) const {
		return impl->settings.GetRelativePath(path);
	}

	std::filesystem::path BuildCoordinator::GetAbsolutePath(std::filesystem::path const& path) const {
		return impl->settings.GetAbsolutePath(path);
	}
}