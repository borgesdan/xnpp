#ifndef XNA_CONTENT_READERS_RAYREADER_HPP
#define XNA_CONTENT_READERS_RAYREADER_HPP

#include "../ContentTypeReader.hpp"
#include "../ContentReader.hpp"
#include "Xna/Framework/Ray.hpp"

namespace Xna {
	struct RayReader : public ContentTypeReaderT<Ray> {
	protected:
		inline Ray Read(ContentReader& input, Ray& existingInstance) override {
			Ray ray;
			ray.Position = input.ReadVector3();
			ray.Direction = input.ReadVector3();
			return ray;
		}
	};
}

#endif