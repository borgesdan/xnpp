#ifndef XNA_CONTENT_READERS_TEXTURE2DREADER_HPP
#define XNA_CONTENT_READERS_TEXTURE2DREADER_HPP

#include "../ContentTypeReader.hpp"
#include "../ContentReader.hpp"
#include "Xna/Framework/Graphics/Texture2D.hpp"

namespace Xna {
	struct Texture2DReader : public ContentTypeReaderT<Texture2D> {
	protected:
		Texture2D Read(ContentReader& input, Texture2D& existingInstance) override;
	};
}

#endif