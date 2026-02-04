#include "Xna/ContentPipeline/Importers/FontDescriptionImporter.hpp"
#include <pugixml.hpp>
#include <exception>

namespace Xna {
    static uint32_t GetUtf8CodePoint(const char* s) {
        if (!s || s[0] == '\0') return 0;

        const unsigned char* str = reinterpret_cast<const unsigned char*>(s);
        uint32_t cp = str[0];

        if (cp < 0x80) {
            return cp; // 1 byte (ASCII)
        }
        else if ((cp & 0xE0) == 0xC0) {
            return ((cp & 0x1F) << 6) | (str[1] & 0x3F); // 2 bytes
        }
        else if ((cp & 0xF0) == 0xE0) {
            return ((cp & 0x0F) << 12) | ((str[1] & 0x3F) << 6) | (str[2] & 0x3F); // 3 bytes
        }
        else if ((cp & 0xF8) == 0xF0) {
            return ((cp & 0x07) << 18) | ((str[1] & 0x3F) << 12) | ((str[2] & 0x3F) << 6) | (str[3] & 0x3F); // 4 bytes
        }
        return cp;
    }

    static void Deserialize(std::filesystem::path const& filePath, FontDescription& outDesc) {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(filePath.native().c_str());
        
        if (!result) {
            throw std::runtime_error(result.description());
        }

        // O XNA usa o nó <Asset Type="Graphics:FontDescription">
        pugi::xml_node asset = doc.child("XnaContent").child("Asset");

        outDesc.FontName = asset.child_value("FontName");
        outDesc.Size = asset.child("Size").text().as_float();
        outDesc.Spacing = asset.child("Spacing").text().as_float();
        outDesc.UseKerning = asset.child("UseKerning").text().as_bool();

        // Converter Style String para Enum
        std::string styleStr = asset.child_value("Style");
        if (styleStr == "Bold") 
            outDesc.Style = FontDescriptionStyle::Bold;
        else if (styleStr == "Italic") 
            outDesc.Style = FontDescriptionStyle::Italic;
        else 
            outDesc.Style = FontDescriptionStyle::Regular;

        // DefaultCharacter (Opcional no XML)
        pugi::xml_node defCharNode = asset.child("DefaultCharacter");
        if (defCharNode) {
            outDesc.DefaultCharacter = defCharNode.text().get()[0];
        }

        // Regiões de Caracteres
        std::vector<CharacterRegion> characterRegions;

        for (pugi::xml_node region : asset.child("CharacterRegions").children("CharacterRegion")) {
            // Pegar o texto bruto
            const auto startText = region.child("Start").text().get();
            const auto endText = region.child("End").text().get();

            uint32_t startCode = GetUtf8CodePoint(startText);
            uint32_t endCode = GetUtf8CodePoint(endText);
           
            characterRegions.push_back(CharacterRegion(startCode, endCode));
        }

        outDesc.CharacterRegions(characterRegions);        
    }

    

    FontDescription FontDescriptionImporter::ImportT(std::filesystem::path const& filename, ContentImporterContext& context) {
        FontDescription fontDescription{};
        Deserialize(filename, fontDescription);
        fontDescription.Identity = ContentIdentity(filename, "FontDescriptionImporter");
        return fontDescription;
    }
}