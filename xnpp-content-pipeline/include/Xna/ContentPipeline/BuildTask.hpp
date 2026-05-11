#ifndef XNA_CONTENTPIPELINE_BUILDTASK_HPP
#define XNA_CONTENTPIPELINE_BUILDTASK_HPP

#include <optional>
#include <filesystem>
#include "ContentBuildLogger.hpp"
#include "BuildContent.hpp"
#include <Xna/Internal/Macros.hpp>

namespace Xna {
	struct BuildTask {
		XNPP_API static bool OpenProject(std::filesystem::path const& filename);

	private:
		//static inline nlohmann::json data = {};
		static inline ContentBuildLogger logger = nullptr;
//		static inline std::optional<BuildContent> buildContent = std::nullopt;
	};
}

#endif