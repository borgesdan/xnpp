#include "Xna/ContentPipeline/ImporterManager.hpp"
#include "Xna/ContentPipeline/Shared.hpp"
#include "Xna/ContentPipeline/ContentImporter.hpp"
#include <Xna/CSharp/Exception.hpp>
#include <format>
#include "Xna/ContentPipeline/App.hpp"

namespace Xna {
	struct Resources {
		static constexpr std::string_view CantFindImporter = "Cannot find importer '{0}'.";
		static constexpr std::string_view CantDeduceImporterNoneAvailable = "Cannot autodetect which importer to use for '{0}'.There are no importers which handle this file type.Specify the importer that handles this file type in your project.";
	};	

	ImporterManager::ImporterManager() {

	}

	std::shared_ptr<IContentImporter> ImporterManager::GetInstance(std::string const& importerName) {
		return App::GetImporterRegistry().CreateInstance(importerName);
	}

	ContentImporterAttribute ImporterManager::GetImporterAttribute(std::string& importerName) {
		const auto& registries = App::GetImporterRegistry().GetRegistries();		

		for (size_t i = 0; i < registries.size(); ++i) {
			auto& registry = registries[i];
			auto name = App::GetImporterRegistry().GetName(i);
			
			if (!name)
				continue;			
			
			if (name == importerName) {
				const auto attribute = registry->GetAttribute();
				return attribute.value_or({});
			}
			else {
				const auto& friendlyNames = App::GetImporterRegistry().GetFriendlyNames(i);

				for (const auto& friendly : friendlyNames) {
					if (friendly == importerName) {
						const auto attribute = registry->GetAttribute();
						return attribute.value_or({});
					}
				}
			}
		}

		throw std::runtime_error(std::format(Resources::CantFindImporter, importerName));
	}

	std::string ImporterManager::GuessFromFilename(std::filesystem::path const& filename) {
		const auto extension = filename.extension();
		const auto& registries = App::GetImporterRegistry().GetRegistries();

		for (size_t i = 0; i < registries.size(); ++i) {
			auto& registry = registries[i];
			auto attribute = registry->GetAttribute();

			if (attribute) {
				auto it = std::find(attribute->FileExtension().begin(), attribute->FileExtension().end(), extension.string());

				if (it != attribute->FileExtension().end())
				{
					auto name = App::GetImporterRegistry().GetName(i);
					if (!name)
						throw std::runtime_error(std::format(Resources::CantDeduceImporterNoneAvailable, filename.string()));

					return name.value();
				}					
			}
		}

		throw std::runtime_error(std::format(Resources::CantDeduceImporterNoneAvailable, filename.string()));
		return {};
	}
}