#ifndef XNA_FRAMEWORK_PATH_HPP
#define XNA_FRAMEWORK_PATH_HPP

#include <filesystem>
#include <string>

namespace Xna::CSharp {
	struct Path final {
		static std::filesystem::path Combine(std::filesystem::path const& path1, std::filesystem::path const& path2) {
			return std::filesystem::path(path1) / path2;
		}
	};
}

#endif