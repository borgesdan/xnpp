#ifndef XNA_CONTENT_READERS_POINTREADER_HPP
#define XNA_CONTENT_READERS_POINTREADER_HPP

#include "../ContentTypeReader.hpp"
#include "../ContentReader.hpp"
#include "Xna/Framework/Point.hpp"

namespace Xna {
	struct PointReader : public ContentTypeReaderT<Point> {
	protected:
		inline Point Read(ContentReader& input, Point& existingInstance) override {
			Point point;
			point.X = input.ReadInt32();
			point.Y = input.ReadInt32();
			return point;
		}
	};
}

#endif