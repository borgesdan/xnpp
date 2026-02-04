#ifndef XNA_CONTENTPIPELINE_BUILDCONTENT_HPP
#define XNA_CONTENTPIPELINE_BUILDCONTENT_HPP

#include "BuildCoordinatorSettings.hpp"
#include "Shared.hpp"
#include "TimestampCache.hpp"
#include "ContentBuildLogger.hpp"
#include "BuildCoordinator.hpp"
#include <filesystem>
#include <set>
#include <unordered_map>
#include <vector>
#include <string>
#include <optional>
#include "CompileItem.hpp"

namespace Xna {
	struct BuildContent {
		BuildContent(		
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
			std::vector<NoneItem> const& copyAssets
		);

		bool Execute();

	private:
		bool RunTheBuild(BuildCoordinatorSettings& settings,
			TimestampCache timestampCache,
			std::vector<CompileItem> const& sourceAssets,
			std::vector<std::filesystem::path>& outputContent,
			std::vector<std::filesystem::path>& rebuiltContent,
			std::vector<std::filesystem::path>& intermediates,
			std::unordered_map<std::filesystem::path, TimePoint>& dependencyTimestamps,
			std::vector<std::pair<std::filesystem::path, std::string>>& warnings);		

		void RequestBuild(BuildCoordinator& coordinator, CompileItem const& item);
		void CopyItems(BuildCoordinatorSettings settings);

	private:
		ContentBuildLogger logger{nullptr};
		TargetPlatform targetPlatform{TargetPlatform::Windows};
		GraphicsProfile targetProfile{GraphicsProfile::HiDef};
		std::string const& buildConfiguration{};
		bool compressContent;
		std::filesystem::path const& rootDirectory{};
		std::filesystem::path const& loggerRootDirectory{};
		std::filesystem::path const& intermediateDirectory{};
		std::filesystem::path const& outputDirectory{};
		bool rebuildAll{false};
		std::vector<CompileItem> const& sourceAssets{};
		std::vector<NoneItem> const& copyAssets{};
	};
}

#endif