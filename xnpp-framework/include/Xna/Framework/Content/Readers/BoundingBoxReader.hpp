#ifndef XNA_CONTENT_READERS_BOUNDINGBOXREADER_HPP
#define XNA_CONTENT_READERS_BOUNDINGBOXREADER_HPP

#include "../ContentTypeReader.hpp"
#include "../ContentReader.hpp"
#include "Xna/Framework/BoundingBox.hpp"

namespace Xna {
	struct BoundingBoxReader : public ContentTypeReaderT<BoundingBox> {
	protected:
		inline BoundingBox Read(ContentReader& input, BoundingBox& existingInstance) override {
			BoundingBox boundingBox;
			boundingBox.Min = input.ReadVector3();
			boundingBox.Max = input.ReadVector3();
			return boundingBox;
		}
	};
}

#endif