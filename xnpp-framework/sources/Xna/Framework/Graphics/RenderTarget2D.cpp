#include "Xna/Framework/Graphics/RenderTarget2D.hpp"

namespace Xna {
	RenderTarget2D::RenderTarget2D(
		GraphicsDevice& graphicsDevice,
		size_t width,
		size_t height,
		bool mipMap,
		SurfaceFormat preferredFormat,
		DepthFormat preferredDepthFormat,
		size_t preferredMultiSampleCount,
		Xna::RenderTargetUsage usage
	) {
		
		impl = std::make_shared<Implementation>();
		impl->depthStencilFormat = preferredDepthFormat;
		impl->multiSampleCount = preferredMultiSampleCount;
		impl->usage = usage;
		impl->texture2D = Xna::Texture2D(graphicsDevice, width, height, mipMap, preferredFormat);
		Platform::RenderTarget2D_Init(*this, graphicsDevice, preferredDepthFormat, preferredMultiSampleCount, usage);
	}	
}