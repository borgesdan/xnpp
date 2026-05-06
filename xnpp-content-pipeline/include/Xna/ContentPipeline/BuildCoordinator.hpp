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
#include <Xna/Internal/Macros.hpp>

namespace Xna {
	class BuildCoordinator final {
	public:
		XNPP_API BuildCoordinator(
			ContentBuildLogger& buildLogger,
			BuildCoordinatorSettings& settings,
			TimestampCache timestampCache);

		inline BuildCoordinatorSettings Settings() const {
			return impl->settings;
		}

		XNPP_API Xna::ImporterManager ImporterManager();
		XNPP_API ProcessorManager ProcessorManager();
		XNPP_API Xna::ContentCompiler ContentCompiler();

		XNPP_API void Dispose();

		XNPP_API void RequestBuild(
			std::filesystem::path const& sourceFilename,
			std::filesystem::path const& assetName,
			std::string const& importerName,
			std::string const& processorName,
			OpaqueDataDictionary const& processorParameters);

		XNPP_API BuildItem RequestBuild(BuildRequest& request);
		XNPP_API std::any BuildAndLoadAsset(BuildItem& item, BuildItem& requestingItem);

		XNPP_API void RunTheBuild();
		XNPP_API std::vector<std::filesystem::path> GetOutputFiles() const; //TODO: [!] deixar as funções const
		XNPP_API std::vector<std::filesystem::path> GetRebuiltFiles() const;
		XNPP_API std::vector<std::filesystem::path> GetIntermediateFiles() const;
		XNPP_API std::unordered_map<std::filesystem::path, TimePoint> GetDependencyTimestamps() const;
		//tuple é um KeyValuePair<string, string>
		XNPP_API std::vector<std::pair<std::filesystem::path, std::string>> GetWarnings() const;
		XNPP_API void AddDependency(BuildItem& item, std::filesystem::path const& filename);
		XNPP_API void AddExtraOutput(BuildItem& item, std::filesystem::path const& filename);

		XNPP_API std::filesystem::path GetRelativePath(std::filesystem::path const& path) const;
		XNPP_API std::filesystem::path GetAbsolutePath(std::filesystem::path const& path) const;

		inline BuildCoordinator(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(BuildCoordinator const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:		
		XNPP_API std::filesystem::path ChooseOutputFilename(BuildRequest& request);

		XNPP_API static std::filesystem::path GetAssetNameStub(std::filesystem::path sourceFilename);
		XNPP_API static std::filesystem::path RemoveNonAnsiCharacters(std::filesystem::path const& value);
		XNPP_API void RemoveBuildItem(BuildItem& item);
		XNPP_API std::any BuildAsset(BuildItem& item);
		XNPP_API std::any BuildAssetWorker(BuildItem& item);
		XNPP_API bool NeedIncrementalBuild(BuildItem& item, std::string& rebuildReason);
		XNPP_API bool TimestampsHaveChanged(BuildItem& item, std::string& rebuildReason);
		
		XNPP_API std::any ImportAsset(BuildItem& item);
		XNPP_API std::any ImportAssetDirectly(BuildItem& item, std::string const& importerName);
		XNPP_API std::any ImportAssetViaCache(BuildItem& item, std::string const& importerName);
		XNPP_API void SerializeAsset(BuildItem& item, std::any const& assetData);
		XNPP_API void SerializeIntermediateAsset(BuildItem& item, std::any const& assetData, std::filesystem::path const& outputFilename);
		XNPP_API void SerializeOutputAsset(BuildItem& item, std::any const& assetData, std::filesystem::path const& outputFilename);

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