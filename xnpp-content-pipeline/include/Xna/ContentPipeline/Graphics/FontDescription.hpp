#ifndef XNA_CONTENTPIPELINE_GRAPHICS_FONTDESCRIPTION_HPP
#define XNA_CONTENTPIPELINE_GRAPHICS_FONTDESCRIPTION_HPP

#include "Xna/ContentPipeline/ContentItem.hpp"
#include "CharacterCollection.hpp"
#include "CharacterRegion.hpp"
#include <filesystem>
#include <optional>
#include <vector>

namespace Xna {
	enum class FontDescriptionStyle
	{
		Regular = 0,
		Bold = 1,
		Italic = 2,
	};

	struct FontDescription : public ContentItem {
		std::filesystem::path FontName;
		float Size{0};
		float Spacing{0};
		bool UseKerning{false};
		FontDescriptionStyle Style{FontDescriptionStyle::Regular};
		std::optional<uint32_t> DefaultCharacter;
		CharacterCollection Characters{};

		std::vector<CharacterRegion> CharacterRegions() const;
		void CharacterRegions(std::vector<CharacterRegion> const& value);
	};
}

#endif