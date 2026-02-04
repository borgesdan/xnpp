#ifndef XNA_CONTENTPIPELINE_BUILDCOORDINATOR_HPP
#define XNA_CONTENTPIPELINE_BUILDCOORDINATOR_HPP

#include "BuildCoordinatorSettings.hpp"
#include "BuildItem.hpp"
#include "BuildItemCollection.hpp"
#include "ContentBuildLogger.hpp"
#include "ImporterManager.hpp"
#include "OpaqueDataDictionary.hpp"
#include "ProcessorManager.hpp"
#include "Serialization/Compiler/ContentCompiler.hpp"
#include "Shared.hpp"
#include "TimestampCache.hpp"
#include <any>
#include <memory>
#include <tuple>
#include <vector>

namespace Xna {
	class BuildCoordinator final {
	public:
		BuildCoordinator(
			ContentBuildLogger& buildLogger,
			BuildCoordinatorSettings& settings,
			TimestampCache timestampCache);

		inline BuildCoordinatorSettings Settings() const {
			return impl->settings;
		}

		Xna::ImporterManager ImporterManager();
		Xna::ProcessorManager ProcessorManager();
		Xna::ContentCompiler ContentCompiler();		

		void Dispose();

		void RequestBuild(
			std::filesystem::path const& sourceFilename,
			std::filesystem::path const& assetName,
			std::string const& importerName,
			std::string const& processorName,
			OpaqueDataDictionary const& processorParameters);

		BuildItem RequestBuild(BuildRequest& request);
		std::any BuildAndLoadAsset(BuildItem& item, BuildItem& requestingItem);

		void RunTheBuild();
		std::vector<std::filesystem::path> GetOutputFiles() const; //TODO: [!] deixar as funções const
		std::vector<std::filesystem::path> GetRebuiltFiles() const;
		std::vector<std::filesystem::path> GetIntermediateFiles() const;
		std::unordered_map<std::filesystem::path, TimePoint> GetDependencyTimestamps() const;
		//tuple é um KeyValuePair<string, string>
		std::vector<std::pair<std::filesystem::path, std::string>> GetWarnings() const;
		void AddDependency(BuildItem& item, std::filesystem::path const& filename);
		void AddExtraOutput(BuildItem& item, std::filesystem::path const& filename);

		std::filesystem::path GetRelativePath(std::filesystem::path const& path) const;
		std::filesystem::path GetAbsolutePath(std::filesystem::path const& path) const;

		inline BuildCoordinator(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(BuildCoordinator const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:		
		std::filesystem::path ChooseOutputFilename(BuildRequest& request);

		static std::filesystem::path GetAssetNameStub(std::filesystem::path sourceFilename);
		static std::filesystem::path RemoveNonAnsiCharacters(std::filesystem::path const& value);
		void RemoveBuildItem(BuildItem& item);
		std::any BuildAsset(BuildItem& item);
		std::any BuildAssetWorker(BuildItem& item);
		bool NeedIncrementalBuild(BuildItem& item, std::string& rebuildReason);
		bool TimestampsHaveChanged(BuildItem& item, std::string& rebuildReason);
		
		std::any ImportAsset(BuildItem& item);
		std::any ImportAssetDirectly(BuildItem& item, std::string const& importerName);
		std::any ImportAssetViaCache(BuildItem& item, std::string const& importerName);
		void SerializeAsset(BuildItem& item, std::any const& assetData);
		void SerializeIntermediateAsset(BuildItem& item, std::any const& assetData, std::filesystem::path const& outputFilename);
		void SerializeOutputAsset(BuildItem& item, std::any const& assetData, std::filesystem::path const& outputFilename);

	private:
		struct Implementation {
			Xna::ContentCompiler contentCompiler{ nullptr };
			BuildCoordinatorSettings settings{};
			ContentBuildLogger buildLogger{};
			TimestampCache timestampCache{};
			std::vector<std::filesystem::path> rebuiltFiles;
			std::vector<BuildItem> incrementalRecurseDetector;
			Xna::ImporterManager importerManager{nullptr};
			Xna::ProcessorManager processorManager{nullptr};
			BuildItemCollection buildItems{ nullptr };
			bool buildItemsChanged{ false };
		};

		std::shared_ptr<Implementation> impl;
	};
}

#endif