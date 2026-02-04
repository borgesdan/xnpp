#ifndef XNA_CONTENT_READERS_BOUNDINGSPHEREREADER_HPP
#define XNA_CONTENT_READERS_BOUNDINGSPHEREREADER_HPP

#include "../ContentTypeReader.hpp"
#include "../ContentReader.hpp"
#include "Xna/Framework/BoundingSphere.hpp"

namespace Xna {
	struct BoundingSphereReader : public ContentTypeReaderT<BoundingSphere> {
	protected:
		inline BoundingSphere Read(ContentReader& input, BoundingSphere& existingInstance) override {
			BoundingSphere boundingSphere;
			boundingSphere.Center = input.ReadVector3();
			boundingSphere.Radius = input.ReadSingle();
			return boundingSphere;
		}
	};
}

#endif