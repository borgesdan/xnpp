#include "Xna/ContentPipeline/Graphics/FontDescription.hpp"
#include <algorithm>
#include <exception>
#include <format>

namespace Xna {
	std::vector<CharacterRegion> FontDescription::CharacterRegions() const {
        std::vector<CharacterRegion> characterRegionList;
        auto charList = Characters.CharList();
        
        if (Characters.Count() == 0)
            return characterRegionList;        
        
        std::sort(charList.begin(), charList.end());

        auto start = charList[0];
        auto end = charList[0];
        for (size_t index = 1; index < charList.size(); ++index) {
            if (charList[index] != end + 1) {
                characterRegionList.push_back(CharacterRegion(start, end));
                start = charList[index];
            }
            end = charList[index];
        }
        characterRegionList.push_back(CharacterRegion(start, end));
        return characterRegionList;
	}

    void FontDescription::CharacterRegions(std::vector<CharacterRegion> const& value) {
        for (size_t index = 0; index < value.size(); ++index)
        {
            CharacterRegion characterRegion = value[index];
            if (characterRegion.End < characterRegion.Start) {
                throw std::runtime_error("The value is invalid. Start character must be less than or equal to End character");
            }                

            for (auto start = characterRegion.Start; start <= characterRegion.End; ++start)
                Characters.Add(start);
        }
    }
}