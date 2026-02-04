#ifndef XNA_GRAPHICS_SPRITEFONTCONTENT_HPP
#define XNA_GRAPHICS_SPRITEFONTCONTENT_HPP

#include "Texture2DContent.hpp"
#include <Xna/Framework/Rectangle.hpp>
#include <Xna/Framework/Vector3.hpp>
#include <cstdint>
#include <vector>
#include <optional>

namespace Xna {
	struct SpriteFontContent {
		Texture2DContent Texture;
		std::vector<Rectangle> Glyphs;
		std::vector<Rectangle> Cropping;
		std::vector<uint32_t> CharacterMap;
		int32_t LineSpacing{0};
		float Spacing{0};
		std::vector<Vector3>Kerning;
		std::optional<uint32_t> DefaultCharacter;
	};
}

#endif