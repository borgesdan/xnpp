#include "Xna/Framework/Graphics/Texture2D.hpp"

namespace Xna {
	Texture2D::Texture2D(GraphicsDevice const& device, size_t width, size_t height) : Texture2D(device, width, height, false, SurfaceFormat::Color){}

	Texture2D::Texture2D(GraphicsDevice const& device, size_t width, size_t height, bool mipMap, SurfaceFormat format) {
		impl = std::make_shared<Implementation>();
		impl->hasMipmaps = mipMap;
		impl->width = width;
		impl->height = height;
		impl->format = format;
		Platform::Texture2D_Init(*this, device, width, height, mipMap, format);
	}

	Texture2D Texture2D::FromStream(GraphicsDevice const& device, CSharp::Stream& stream, size_t width, size_t height, bool zoom) {
		return Platform::Texture2D_FromStream(device, stream, width, height, zoom);
	}

	void Texture2D::SaveAsPng(CSharp::Stream& stream, size_t width, size_t height) {
		Platform::Texture2D_SaveFile(*this, stream, width, height, Platform::Texture2D_SaveFileType::Png);
	}

	void Texture2D::SaveAsJpeg(CSharp::Stream& stream, size_t width, size_t height) {
		Platform::Texture2D_SaveFile(*this, stream, width, height, Platform::Texture2D_SaveFileType::Jpeg);
	}
	
	int32_t Texture2D::LevelCount() const {
		return Platform::Texture2D_LevelCount(*this);
	}
}