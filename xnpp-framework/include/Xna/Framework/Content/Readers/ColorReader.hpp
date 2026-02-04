#ifndef XNA_CONTENT_READERS_COLORREADER_HPP
#define XNA_CONTENT_READERS_COLORREADER_HPP

#include "../ContentTypeReader.hpp"
#include "../ContentReader.hpp"
#include "Xna/Framework/Color.hpp"

namespace Xna {
	struct ColorReader : public ContentTypeReaderT<Color> {
	protected:
		inline Color Read(ContentReader& input, Color& existingInstance) override {
			return Color(input.ReadUInt32());
		}
	};
}

#endif