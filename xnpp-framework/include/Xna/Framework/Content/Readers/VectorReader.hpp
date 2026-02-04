#ifndef XNA_CONTENT_READERS_VECTORREADER_HPP
#define XNA_CONTENT_READERS_VECTORREADER_HPP

#include "../ContentTypeReader.hpp"
#include "../ContentReader.hpp"
#include "Xna/Framework/Vector2.hpp"
#include "Xna/Framework/Vector3.hpp"
#include "Xna/Framework/Vector4.hpp"

namespace Xna {
	struct Vector2Reader : public ContentTypeReaderT<Vector2> {
	protected:
		inline Vector2 Read(ContentReader& input, Vector2& existingInstance) override {
			return input.ReadVector2();
		}		
	};
	
	struct Vector3Reader : public ContentTypeReaderT<Vector3> {
	protected:
		inline Vector3 Read(ContentReader& input, Vector3& existingInstance) override {
			return input.ReadVector3();
		}		
	};
	
	struct Vector4Reader : public ContentTypeReaderT<Vector4> {
	protected:
		inline Vector4 Read(ContentReader& input, Vector4& existingInstance) override {
			return input.ReadVector4();
		}		
	};
}

#endif