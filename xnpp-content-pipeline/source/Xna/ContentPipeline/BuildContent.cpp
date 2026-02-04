#include "Xna/ContentPipeline/BuildContent.hpp"
#include "Xna/ContentPipeline/CompileItem.hpp"
#include "Xna/ContentPipeline/BuildRequestFix.hpp"

namespace Xna {
	void BuildContent::CopyItems(BuildCoordinatorSettings settings) {
		for (const auto& i : copyAssets) {
			auto file = i.Include;		

			auto filename = file;
			filename.replace_extension();
			const auto extension = file.extension();

			if (filename != i.Name) {
				file.replace_filename(i.Name);
				file.replace_extension(extension);
			}			

			auto source = settings.GetAbsolutePath(i.Include);
			auto destination = settings.OutputDirectory / file;

			std::string message = "Copying file: ";
			message.append(i.Include.string());
			message.append(" to ");
			message.append(destination.string());
			logger.LogImportantMessage(message);

			try {
				const auto options = i.CopyOptions();
				auto directory = destination.parent_path();
				if (!std::filesystem::exists(directory))
					std::filesystem::create_directory(directory);

				if (options == CopyToOutputDirectoryOptions::Always) {					
					std::filesystem::copy_file(source, destination, std::filesystem::copy_options::overwrite_existing);
				}
				else if (options == CopyToOutputDirectoryOptions::PreserveNewest) {
					auto result = std::filesystem::copy_file(source, destination, std::filesystem::copy_options::update_existing);
					
					if(!result)
						logger.LogMessage("The file was preserved.");
				}
			}
			catch (std::exception const& ex) {
				std::string error = ("Failed copy file: ");
				error.append(filename.string());
				error.append(". Ex: ");
				error.append(ex.what());

				logger.LogImportantMessage(error);
				continue;
			}			
		}
	}

	BuildContent::BuildContent(
		ContentBuildLogger logger,
		TargetPlatform targetPlatform,
		GraphicsProfile targetProfile,
		std::string const& buildConfiguration,
		bool compressContent,
		std::filesystem::path const& rootDirectory,
		std::filesystem::path const& loggerRootDirectory,
		std::filesystem::path const& intermediateDirectory,
		std::filesystem::path const& outputDirectory,
		bool rebuildAll,
		std::vector<CompileItem> const& sourceAssets,
		std::vector<NoneItem> const& copyAssets)
		: logger(logger), targetPlatform(targetPlatform), targetProfile(targetProfile),
		buildConfiguration(buildConfiguration), compressContent(compressContent),
		rootDirectory(rootDirectory), loggerRootDirectory(loggerRootDirectory), intermediateDirectory(intermediateDirectory),
		rebuildAll(rebuildAll), sourceAssets(sourceAssets), copyAssets(copyAssets){ 	
		
	}

	bool BuildContent::Execute() {
		std::vector<std::filesystem::path> outputContent;
		std::vector<std::filesystem::path> rebuiltContent;
		std::vector<std::filesystem::path> intermediates;
		std::unordered_map<std::filesystem::path, TimePoint> dependencyTimestamps;
		std::vector<std::pair<std::filesystem::path, std::string>> warnings;

		TimestampCache timestampCache;

		BuildCoordinatorSettings settings;
		settings.TargetPlatform = targetPlatform;
		settings.TargetProfile = targetProfile;
		settings.BuildConfiguration = buildConfiguration;
		settings.CompressContent = compressContent;
		settings.RootDirectory = rootDirectory;
		settings.LoggerRootDirectory = loggerRootDirectory;
		settings.IntermediateDirectory = intermediateDirectory;
		settings.OutputDirectory = outputDirectory;
		settings.RebuildAll = rebuildAll;		

		auto value = RunTheBuild(settings,
			timestampCache,
			sourceAssets,
			outputContent,
			rebuiltContent,
			intermediates,
			dependencyTimestamps,
			warnings);

		if(value)
			CopyItems(settings);

		return value;
	}

	

	void BuildContent::RequestBuild(BuildCoordinator& coordinator, CompileItem const& item) {
		auto include = item.Include;
		auto assetName = item.Include;
		assetName.replace_extension();
		
		if (!include.is_absolute())
			include = rootDirectory / include;		

		std::string importer = item.Importer;
		std::string processor = item.Processor;
		BuildRequestFix::FixRequest(item, importer, processor);

		coordinator.RequestBuild(include, assetName, importer, processor, nullptr);
	}

	bool BuildContent::RunTheBuild(BuildCoordinatorSettings& settings,
		TimestampCache timestampCache,
		std::vector<CompileItem> const& sourceAssets,
		std::vector<std::filesystem::path>& outputContent,
		std::vector<std::filesystem::path>& rebuiltContent,
		std::vector<std::filesystem::path>& intermediates,
		std::unordered_map<std::filesystem::path, TimePoint>& dependencyTimestamps,
		std::vector<std::pair<std::filesystem::path, std::string>>& warnings) {

		auto coordinator = BuildCoordinator(logger, settings, timestampCache);		

		for (const auto& sourceAsset : sourceAssets)
			RequestBuild(coordinator, sourceAsset);

		coordinator.RunTheBuild();
		outputContent = coordinator.GetOutputFiles();
		rebuiltContent = coordinator.GetRebuiltFiles();
		intermediates = coordinator.GetIntermediateFiles();
		dependencyTimestamps = coordinator.GetDependencyTimestamps();
		warnings = coordinator.GetWarnings();

		return true;
	}
}