#ifndef XNA_CONTENT_READERS_QUATERNIONREADER_HPP
#define XNA_CONTENT_READERS_QUATERNIONREADER_HPP

#include "../ContentTypeReader.hpp"
#include "../ContentReader.hpp"
#include "Xna/Framework/Quaternion.hpp"

namespace Xna {
	struct QuaternionReader : public ContentTypeReaderT<Quaternion> {
	protected:
		inline Quaternion Read(ContentReader& input, Quaternion& existingInstance) override {
			return input.ReadQuaternion();
		}
	};
}

#endif