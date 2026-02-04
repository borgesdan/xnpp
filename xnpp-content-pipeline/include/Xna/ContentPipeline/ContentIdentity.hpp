#ifndef XNA_CONTENTPIPELINE_CONTENTIDENTITY_HPP
#define XNA_CONTENTPIPELINE_CONTENTIDENTITY_HPP

#include <filesystem>

namespace Xna {
	//[Serializable]
	struct ContentIdentity final {
		//[ContentSerializer(Optional = true)]
		std::filesystem::path SourceFileName;
		//[ContentSerializer(Optional = true)]
		std::filesystem::path SourceTool;
		//[ContentSerializer(Optional = true)]
		std::string FragmentIdentifier;

		constexpr ContentIdentity() = default;
		inline ContentIdentity(std::filesystem::path const& sourceFilename)
			: SourceFileName(sourceFilename) { }
		inline ContentIdentity(std::filesystem::path const& sourceFilename, std::filesystem::path const& sourceTool)
			: SourceFileName(sourceFilename), SourceTool(sourceTool) { }
		inline ContentIdentity(std::filesystem::path const& sourceFilename, std::filesystem::path const& sourceTool, std::string const& fragmentIdentifier)
			: SourceFileName(sourceFilename), SourceTool(sourceTool), FragmentIdentifier(fragmentIdentifier) { }
	};
}

#endif