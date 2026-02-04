#ifndef XNA_CONTENTPIPELINE_PATHUTILS_HPP
#define XNA_CONTENTPIPELINE_PATHUTILS_HPP

#include <filesystem>
#include "Shared.hpp"

namespace Xna {
	struct PathUtils final {
		static std::filesystem::path GetFullPath(std::filesystem::path const& path);
		static std::filesystem::path GetAbsoluteUri(std::filesystem::path const& path);
		static std::filesystem::path GetRelativePath(std::filesystem::path const& baseUri, std::filesystem::path const& filename);
		static std::filesystem::path GetFullDirectoryName(std::filesystem::path const& path);
		static void CreateDirectory(std::filesystem::path const& path);
		static void SafeDeleteFile(std::filesystem::path const& filename);
		static void SafeDeleteFileAndDirectories(std::filesystem::path const& filename, std::vector<std::filesystem::path> const& rootDirectories);
		static TimePoint GetTimestamp(std::filesystem::path const& filename);
	};
}

#endif