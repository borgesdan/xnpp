#include "Xna/Framework/Graphics/SpriteFont.hpp"
#include "Xna/Framework/Graphics/SpriteBatch.hpp"
#include "Xna/CSharp/Exception.hpp"
#include <cmath>

namespace Xna {
	SpriteFont::SpriteFont(Texture2D const& texture, std::vector<Rectangle> const& glyphs, std::vector<Rectangle> const& cropping,
		std::vector<char32_t> const& charMap, int32_t lineSpacing, float spacing, std::vector<Vector3> const& kerning, std::optional<char32_t> const& defaultCharacter) {		
        assert(texture != nullptr && "Texture is null.");
        
        impl = std::make_shared<Implementation>();
		impl->textureValue = texture;
		impl->glyphData = glyphs;
		impl->croppingData = cropping;
		impl->characterMap = charMap;
		impl->lineSpacing = lineSpacing;
		impl->spacing = spacing;
		impl->kerning = kerning;
		impl->defaultCharacter = defaultCharacter;
	}	

    size_t SpriteFont::GetIndexForCharacter(char32_t character) const {
        size_t num1 = 0;
        size_t num2 = impl->characterMap.size() - 1;
        
        while (num1 <= num2) {
            auto index = num1 + (num2 - num1 >> 1);
            const auto character1 = impl->characterMap[index];
            
            if (character1 == character)
                return index;

            if (character1 < character)
                num1 = index + 1;
            else
                num2 = index - 1;
        }

        assert(!impl->defaultCharacter && "Default character is empty.");
        
        const auto character2 = *impl->defaultCharacter;

        if (character != character2)
            return GetIndexForCharacter(character2);
    }	        
}