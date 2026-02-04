#ifndef XNA_CONTENT_READERS_MATRIXRREADER_HPP
#define XNA_CONTENT_READERS_MATRIXRREADER_HPP

#include "../ContentTypeReader.hpp"
#include "../ContentReader.hpp"
#include "Xna/Framework/Matrix.hpp"

namespace Xna {
	struct MatrixReader : public ContentTypeReaderT<Matrix> {
	protected:
		inline Matrix Read(ContentReader& input, Matrix& existingInstance) override {
			return input.ReadMatrix();
		}
	};
}

#endif