#ifndef XNA_CONTENTPIPELINE_GRAPHICS_TEXTURE2DCONTENT_HPP
#define XNA_CONTENTPIPELINE_GRAPHICS_TEXTURE2DCONTENT_HPP

#include "TextureContent.hpp"

namespace Xna {
	struct Texture2DContent : public TextureContent {
		~Texture2DContent() override = default;

		Texture2DContent() : TextureContent(MipmapChainCollection(1, true)){}

		inline MipmapChain& Mipmaps() { return Faces()[0]; }
		inline void Mipmaps(MipmapChain const& value) { Faces()[0] = value; }

		inline Texture2DContent(std::nullptr_t) : TextureContent(nullptr) { }
	};
}

#endif