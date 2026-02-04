#ifndef XNA_CONTENTPIPELINE_GRAPHICS_TEXTURECONTENT_HPP
#define XNA_CONTENTPIPELINE_GRAPHICS_TEXTURECONTENT_HPP

#include "MipmapChainCollection.hpp"
#include <memory>

namespace Xna {
	struct TextureContent {
		virtual ~TextureContent() = default;

		inline MipmapChainCollection Faces() const { return impl->faces; }

		inline TextureContent(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(TextureContent const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	protected:
		inline TextureContent(MipmapChainCollection const& faces) {
			impl = std::make_shared<Implementation>();
			impl->faces = faces;
		}
	
	private:
		struct Implementation {
			MipmapChainCollection faces{ nullptr };
		};
		
		std::shared_ptr<Implementation> impl;
	};
}

#endif