#include "Xna/Framework/Graphics/Texture3D.hpp"

namespace Xna {
	Texture3D::Texture3D(GraphicsDevice& graphicsDevice, size_t width, size_t height, size_t depth, bool mipMap, SurfaceFormat format) {
		impl = std::make_shared<Implementation>();
		impl->depth = depth;
		impl->width = width;
		impl->height = height;
		impl->format = format;
		impl->hasMipMap = mipMap;

		Platform::Texture3D_Init(*this, graphicsDevice, width, height, depth, mipMap, format);
	}
}