#ifndef XNA_FRAMEWORK_GRAPHICS_SPRITEFONT_HPP
#define XNA_FRAMEWORK_GRAPHICS_SPRITEFONT_HPP

#include <memory>
#include <optional>
#include <string>
#include <cstdint>
#include "Xna/Platform/Platform.hpp"
#include "GraphicsResource.hpp"
#include "Xna/Framework/Vector2.hpp"
#include "Xna/Framework/Vector3.hpp"
#include "Xna/Internal/Export.hpp"

namespace Xna {
	//Represents a font texture.
	class SpriteFont final : public GraphicsResource{
		~SpriteFont() override = default;

		// Returns the width and height of a string.
		XNPP_API Vector2 MeasureString(std::string const& text, bool ignoreWhiteSpace = true);
		// Returns the width and height of a string.
		XNPP_API Vector2 MeasureString(std::wstring const& text, bool ignoreWhiteSpace = true);

		//Gets or sets the default character for the font.
		inline std::optional<char32_t> DefaultCharacter() const { return impl->defaultCharacter; }
		//Gets or sets the default character for the font.
		inline void DefaultCharacter(std::optional<char32_t> const& value) { impl->defaultCharacter = value; }
		//Gets or sets the vertical distance (in pixels) between the base lines of two consecutive lines of text
		inline int32_t LineSpacing() const { return impl->lineSpacing; }
		//Gets or sets the vertical distance (in pixels) between the base lines of two consecutive lines of text
		inline void LineSpacing(int32_t value) { impl->lineSpacing = value; }
		//Gets or sets the spacing of the font characters.
		inline float Spacing() const { return impl->spacing; }
		//Gets or sets the spacing of the font characters.
		inline void Spacing(float value) { impl->spacing = value; }

	private:
		SpriteFont(
			Texture2D const& texture,
			std::vector<Rectangle> const& glyphs,
			std::vector<Rectangle> const& cropping,
			std::vector<char32_t> const& charMap,
			int32_t lineSpacing,
			float spacing,
			std::vector<Vector3> const& kerning,
			std::optional<char32_t> const& defaultCharacter);

		struct Implementation {
			
			std::optional<char32_t> defaultCharacter;
			int32_t lineSpacing{ 0 };
			float spacing{ 0 };
			PlatformImpl::SpriteFontImpl platformImpl;
		};

		std::shared_ptr<Implementation> impl;

		friend struct Platform;
	};
}

#endif