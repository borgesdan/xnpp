#ifndef XNA_CONTENTPIPELINE_PATHUTILS_HPP
#define XNA_CONTENTPIPELINE_PATHUTILS_HPP

#include <filesystem>
#include "Shared.hpp"
#include <Xna/Internal/Macros.hpp>

namespace Xna {
	struct PathUtils final {
		XNPP_API static std::filesystem::path GetFullPath(std::filesystem::path const& path);
		XNPP_API static std::filesystem::path GetAbsoluteUri(std::filesystem::path const& path);
		XNPP_API static std::filesystem::path GetRelativePath(std::filesystem::path const& baseUri, std::filesystem::path const& filename);
		XNPP_API static std::filesystem::path GetFullDirectoryName(std::filesystem::path const& path);
		XNPP_API static void CreateDirectory(std::filesystem::path const& path);
		XNPP_API static void SafeDeleteFile(std::filesystem::path const& filename);
		XNPP_API static void SafeDeleteFileAndDirectories(std::filesystem::path const& filename, std::vector<std::filesystem::path> const& rootDirectories);
		XNPP_API static TimePoint GetTimestamp(std::filesystem::path const& filename);
	};
}

#endif