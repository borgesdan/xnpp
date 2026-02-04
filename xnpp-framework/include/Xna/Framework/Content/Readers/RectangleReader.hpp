#ifndef XNA_CONTENT_READERS_RECTANGLEREADER_HPP
#define XNA_CONTENT_READERS_RECTANGLEREADER_HPP

#include "../ContentTypeReader.hpp"
#include "../ContentReader.hpp"
#include "Xna/Framework/Rectangle.hpp"

namespace Xna {
	struct RectangleReader : public ContentTypeReaderT<Rectangle> {
	protected:
		inline Rectangle Read(ContentReader& input, Rectangle& existingInstance) override {
			Rectangle rectangle;
			rectangle.X = input.ReadInt32();
			rectangle.Y = input.ReadInt32();
			rectangle.Width = input.ReadInt32();
			rectangle.Height = input.ReadInt32();
			return rectangle;
		}
	};
}

#endif