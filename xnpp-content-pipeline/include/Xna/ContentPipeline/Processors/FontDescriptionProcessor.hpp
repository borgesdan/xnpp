#ifndef XNA_PROCESSORS_FONTDESCRIPTIONPROCESSOR_HPP
#define XNA_PROCESSORS_FONTDESCRIPTIONPROCESSOR_HPP

#include "../ContentProcessor.hpp"
#include "../Graphics/FontDescription.hpp"
#include "../Graphics/SpriteFontContent.hpp"
#include <Xna/Internal/Macros.hpp>

namespace Xna {
	struct FontDescriptionProcessor : public ContentProcessor<FontDescription, SpriteFontContent> {
		XNPP_API SpriteFontContent ProcessT(FontDescription& input, ContentProcessorContext& context) override;
	};
}

#endif