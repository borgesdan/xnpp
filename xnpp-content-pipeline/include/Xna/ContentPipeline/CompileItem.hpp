#ifndef XNA_CONTENTPIPELINE_COMPILEITEM_HPP
#define XNA_CONTENTPIPELINE_COMPILEITEM_HPP

#include <filesystem>

namespace Xna {
	struct CompileItem
	{
		std::filesystem::path Include;
		std::string Name;
		std::string Importer;
		std::string Processor;
	};

	enum class CopyToOutputDirectoryOptions {
		//(Default): The file is not copied to the output directory.
		Never,
		//The file is always copied every time you build, which can slow down incremental builds.
		Always,
		//The file is copied only if it is newer or does not exist in the output directory. This is highly recommended for performance
		PreserveNewest,
	};

	struct NoneItem {
		std::filesystem::path Include;
		std::string Name;
		std::string CopyToOutputDirectory;

		constexpr CopyToOutputDirectoryOptions CopyOptions() const {
			if (CopyToOutputDirectory.empty() || CopyToOutputDirectory == "PreserveNewest")
				return CopyToOutputDirectoryOptions::PreserveNewest;
			else if (CopyToOutputDirectory == "Never")
				return CopyToOutputDirectoryOptions::Never;
			else if (CopyToOutputDirectory == "Always")
				return CopyToOutputDirectoryOptions::Always;

			return CopyToOutputDirectoryOptions::PreserveNewest;
		}
	};
}

#endif