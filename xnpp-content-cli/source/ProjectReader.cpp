#include "ProjectReader.hpp"

namespace Xna {
	bool ProjectReader::LoadFromFile(const std::filesystem::path& filePath, ContentProject& outProject) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Erro: Nao foi possivel abrir o arquivo " << filePath << std::endl;
            return false;
        }

        nlohmann::json j;
        try {
            file >> j;
        }
        catch (const nlohmann::json::parse_error& e) {
            std::cerr << "Erro de parse no JSON: " << e.what() << std::endl;
            return false;
        }

        // Parse manual seguro usando .value() para prever atributos ausentes
        if (j.contains("ItemGroups")) {
            const auto& jGroups = j["ItemGroups"];

            // 1. Lendo os CompileItems
            if (jGroups.contains("CompileItems")) {
                for (const auto& jItem : jGroups["CompileItems"]) {
                    CompileItem item;
                    // nlohmann::json converte std::string nativamente. 
                    // Passamos para a string primeiro e o construtor do path faz o resto
                    item.Include = jItem.value("Include", "");
                    item.Name = jItem.value("Name", "");
                    item.Importer = jItem.value("Importer", "");
                    item.Processor = jItem.value("Processor", "");

                    outProject.Groups.CompileItems.push_back(item);
                }
            }

            // 2. Lendo os NoneItems
            if (jGroups.contains("NoneItems")) {
                for (const auto& jItem : jGroups["NoneItems"]) {
                    NoneItem item;
                    item.Include = jItem.value("Include", "");
                    item.Name = jItem.value("Name", "");
                    item.CopyToOutputDirectory = jItem.value("CopyToOutputDirectory", "");

                    outProject.Groups.NoneItems.push_back(item);
                }
            }
        }

        return true;
	}
}