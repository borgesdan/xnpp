#include "Xna/Framework/Graphics/SpriteFont.hpp"
#include "Xna/CSharp/Exception.hpp"

namespace Xna {
	SpriteFont::SpriteFont(
		Texture2D const& texture,
		std::vector<Rectangle> const& glyphs,
		std::vector<Rectangle> const& cropping,
		std::vector<char32_t> const& charMap,
		int32_t lineSpacing,
		float spacing,
		std::vector<Vector3> const& kerning,
		std::optional<char32_t> const& defaultCharacter) {
		
		if (cropping.size() != glyphs.size() || charMap.size() != glyphs.size() || (!kerning.empty() && kerning.size() != glyphs.size()))
			throw CSharp::InvalidOperationException("Cropping, charmap and kerning (if not empty) must all be the same size.");

		impl = std::shared_ptr<Implementation>();
		impl->defaultCharacter = defaultCharacter;
		impl->lineSpacing = lineSpacing;
		impl->spacing = spacing;

		Platform::SpriteFont_Create(*this, texture, glyphs, cropping, charMap, lineSpacing, spacing, kerning, defaultCharacter);
	}

	Vector2 SpriteFont::MeasureString(std::string const& text, bool ignoreWhiteSpace) {
		if (text.empty())
			return Vector2::Zero();

		return Platform::SpriteFont_MeasureString(*this, text, ignoreWhiteSpace);
	}
	
	Vector2 SpriteFont::MeasureString(std::wstring const& text, bool ignoreWhiteSpace) {
		if (text.empty())
			return Vector2::Zero();

		return Platform::SpriteFont_MeasureString(*this, text, ignoreWhiteSpace);
	}
}