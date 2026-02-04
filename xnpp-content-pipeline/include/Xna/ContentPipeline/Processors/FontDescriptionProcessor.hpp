#ifndef XNA_PROCESSORS_FONTDESCRIPTIONPROCESSOR_HPP
#define XNA_PROCESSORS_FONTDESCRIPTIONPROCESSOR_HPP

#include "../ContentProcessor.hpp"
#include "../Graphics/FontDescription.hpp"
#include "../Graphics/SpriteFontContent.hpp"

namespace Xna {
	struct FontDescriptionProcessor : public ContentProcessor<FontDescription, SpriteFontContent> {
		SpriteFontContent ProcessT(FontDescription& input, ContentProcessorContext& context) override;
	};
}

#endif