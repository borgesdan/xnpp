#ifndef XNA_CONTENTPIPELINE_CONTENTIMPORTERCONTEXT_HPP
#define XNA_CONTENTPIPELINE_CONTENTIMPORTERCONTEXT_HPP

#include <filesystem>
#include "ContentBuildLogger.hpp"

namespace Xna {
	struct ContentImporterContext {
		virtual ~ContentImporterContext() = default;
		virtual ContentBuildLogger Logger() const = 0;
		virtual void AddDependency(std::filesystem::path const& filename) const = 0;
		virtual std::filesystem::path OutputDirectory() const = 0;
		virtual std::filesystem::path IntermediateDirectory() const = 0;
	};
}

#endif