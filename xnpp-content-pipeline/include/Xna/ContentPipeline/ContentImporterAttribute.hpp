#ifndef XNA_CONTENTPIPELINE_CONTENTIMPORTERATTRIBUTE_HPP
#define XNA_CONTENTPIPELINE_CONTENTIMPORTERATTRIBUTE_HPP

#include <vector>
#include <string>
#include <filesystem>

namespace Xna {
	struct ContentImporterAttribute {
		inline ContentImporterAttribute() = default;
		inline ContentImporterAttribute(std::vector<std::filesystem::path> const& fileExtensions)
			: fileExtensions(fileExtensions){ } //TODO: Adicionar validação

		const std::vector<std::filesystem::path>& FileExtension() const { return fileExtensions; }

		constexpr bool CacheImportedData() const { return cacheImportedData; }
		constexpr void CacheImportedData(bool value) { cacheImportedData = value; }
		constexpr std::string DisplayName() const { return displayName; }
		constexpr void DisplayName(std::string const& value) { displayName = value; }
		constexpr std::string DefaultProcessor() const { return defaultProcessor; }
		constexpr void DefaultProcessor(std::string const& value) { defaultProcessor = value; }

	private:
		std::vector<std::filesystem::path> fileExtensions;
		bool cacheImportedData{ false };
		std::string displayName;
		std::string defaultProcessor;
	};
}

#endif