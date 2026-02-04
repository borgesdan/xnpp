#ifndef XNA_CONTENT_READERS_PLANEREADER_HPP
#define XNA_CONTENT_READERS_PLANEREADER_HPP

#include "../ContentTypeReader.hpp"
#include "../ContentReader.hpp"
#include "Xna/Framework/Plane.hpp"

namespace Xna {
	struct PlaneReader : public ContentTypeReaderT<Plane> {
	protected:
		inline Plane Read(ContentReader& input, Plane& existingInstance) override {
			Plane plane;
			plane.Normal = input.ReadVector3();
			plane.D = input.ReadSingle();
			return plane;
		}
	};
}

#endif