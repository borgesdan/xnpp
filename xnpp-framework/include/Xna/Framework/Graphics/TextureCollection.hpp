#ifndef XNA_FRAMEWORK_GRAPHICS_TEXTURECOLLECTION_HPP
#define XNA_FRAMEWORK_GRAPHICS_TEXTURECOLLECTION_HPP

#include "Texture.hpp"
#include "_GraphicsDevice.hpp"
#include <memory>

namespace Xna {
	class TextureCollection final {
	public:
		TextureCollection() = default;
		std::shared_ptr<Texture> operator[](size_t index);
		
	private://Internal
		TextureCollection(GraphicsDevice parent, size_t textureOffset, size_t maxTextures);
		void ResetState();

	private:
		struct Implementation {
			GraphicsDevice parent{ nullptr };
			size_t textureOffset;
			size_t maxTextures;
		};

		std::shared_ptr<Implementation> impl;
	};
}

#endif