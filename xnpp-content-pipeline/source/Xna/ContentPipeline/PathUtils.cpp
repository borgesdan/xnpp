#include "Xna/ContentPipeline/PathUtils.hpp"
#include <stdexcept>
#include <chrono>

namespace fs = std::filesystem;

namespace Xna {
	std::filesystem::path PathUtils::GetFullPath(std::filesystem::path const& path) {
		if (path.empty())
			throw std::invalid_argument("Path is empty.");

		return fs::absolute(path);
	}

	std::filesystem::path PathUtils::GetAbsoluteUri(std::filesystem::path const& path) {
		const auto fullPath = GetFullPath(path);

		// UriKind.Absolute
		if (!fullPath.is_absolute()) 
			throw std::invalid_argument("Invalid filename: not an absolute path");		

		return fullPath;
	}

	std::filesystem::path PathUtils::GetRelativePath(std::filesystem::path const& baseUri, std::filesystem::path const& filename) {
		if(filename.empty())
			throw std::invalid_argument("Filename is empty.");
		
		const auto result = baseUri / filename;

		if (!result.is_absolute()) 
			throw std::invalid_argument("Filename is not an absolute path.");

		return result;
	}

	std::filesystem::path PathUtils::GetFullDirectoryName(std::filesystem::path const& path) {
		return GetFullPath(path);
	}

	void PathUtils::CreateDirectory(std::filesystem::path const& path) {
		try {
			std::filesystem::create_directories(path);
		}
		catch (std::exception const& ex) {
			std::string error{ "Error creating directory. Ex: " };
			error.append(ex.what());

			throw std::runtime_error(error);
		}		
	}

	void PathUtils::SafeDeleteFile(std::filesystem::path const& filename) {
		try {
			std::filesystem::remove(filename);
		}
		catch (...) {

		}
	}

	void PathUtils::SafeDeleteFileAndDirectories(std::filesystem::path const& filename, std::vector<std::filesystem::path> const& rootDirectories) {
		try {
			std::filesystem::remove(filename);

			while (true) {
				auto filename1 = filename.relative_path();

				if (filename1.empty()) {
					for (auto& rootDirectory : rootDirectories) {
						if (rootDirectory.native().starts_with(filename1.native())) //TODO:: StringComparison.OrdinalIgnoreCase
							return;
					}

					size_t count{ 0 };
					for (auto entry : std::filesystem::directory_iterator(filename1)) {
						// Conta apenas arquivos (ignora pastas)
						if (entry.is_regular_file()) {
							count++;							
						}

						if (count > 0)
							break;
					}

					if(count == 0)
						std::filesystem::remove(filename1);
				}
			}
		}
		catch (...) {

		}
	}

	TimePoint PathUtils::GetTimestamp(std::filesystem::path const& filename) {
		try
		{			
			if (std::filesystem::exists(filename)) {
				auto time = std::filesystem::last_write_time(filename);
				auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
					time - fs::file_time_type::clock::now() + std::chrono::system_clock::now()
				);
				return sctp;
			}				
		}
		catch(...)
		{
		}

		return TimePoint::min();
	}
}