#include "Xna/Framework/Graphics/Texture2D.hpp"

namespace Xna {
	Texture2D::Texture2D(GraphicsDevice const& device, size_t width, size_t height) : Texture2D(device, width, height, false, SurfaceFormat::Color){}

	Texture2D::Texture2D(GraphicsDevice const& device, size_t width, size_t height, bool mipMap, SurfaceFormat format) {
		impl = std::make_shared<Implementation>();
		impl->hasMipmaps = mipMap;
		impl->width = width;
		impl->height = height;
		impl->format = format;
		impl->backend = Platform::ITexture2D::Create();		

		impl->backend->Initialize(width, height, mipMap, format);
	}

	Texture2D Texture2D::FromStream(GraphicsDevice const& device, Stream& stream, size_t width, size_t height, bool zoom) {
		//TODO: não implementado
		return nullptr;
	}

	void Texture2D::SaveAsPng(Stream& stream, size_t width, size_t height) {
		//TODO: não implementado
	}

	void Texture2D::SaveAsJpeg(Stream& stream, size_t width, size_t height) {
		//TODO: não implementado
	}
	
	int32_t Texture2D::LevelCount() const {
		//TODO: não implementado
		return -1;
	}
}