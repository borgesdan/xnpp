#include "Xna/ContentPipeline/BuildCoordinatorSettings.hpp"
#include "Xna/ContentPipeline/PathUtils.hpp"
#include <stdexcept>

namespace Xna {
	void BuildCoordinatorSettings::InitializePaths() {
		//Em C# ".", aqui deveria obter o path do executável, mas deixa para lá...
		if (RootDirectory.empty()) 
			throw std::runtime_error("Root directory is empty.");

		RootDirectory = PathUtils::GetFullDirectoryName(RootDirectory);
		IntermediateDirectory = PrepareOutputDirectory(IntermediateDirectory, "obj");
		OutputDirectory = PrepareOutputDirectory(OutputDirectory, "bin");
	}

	static std::string ToString(TargetPlatform platform)
	{
		switch (platform)
		{
		case TargetPlatform::Windows:
			return "Windows";
		case TargetPlatform::WindowsPhone:
			return "WindowsPhone";
		case TargetPlatform::Xbox360:
			return "Xbox360";
		default:
			return "Windows";
		}
	}

	std::filesystem::path BuildCoordinatorSettings::PrepareOutputDirectory(std::filesystem::path directory, std::filesystem::path defaultValue) {
		if (directory.empty()) {
			auto rootDirectory = RootDirectory;
			directory = rootDirectory / defaultValue / ToString(TargetPlatform);

			if (BuildConfiguration.empty())
				directory /= BuildConfiguration;
		}
		else
			directory = PathUtils::GetFullDirectoryName(directory);

		PathUtils::CreateDirectory(directory);
		return directory;
	}

	std::filesystem::path BuildCoordinatorSettings::GetAbsolutePath(std::filesystem::path const& path) const {
		if (path.empty())
			throw std::invalid_argument("Path is empty.");

		auto rootDirectory = RootDirectory;
		return rootDirectory / path;
	}

	std::filesystem::path BuildCoordinatorSettings::GetRelativePath(std::filesystem::path const& path) const {
		if (!path.is_absolute())
			throw std::invalid_argument("Path is not um absolute path.");

		if (path.native().starts_with(RootDirectory.native())) //TODO: StringComparison.OrdinalIgnoreCase
			return path;

		return path.native().substr(RootDirectory.native().length());
	}
}

