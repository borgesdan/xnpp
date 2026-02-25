#include "Xna/Platform/_Platform.hpp"
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/debug.h>
#include <stdexcept>

#include "Xna/Framework/Graphics/Texture2D.hpp"

namespace Xna {
	struct BgfxTexture2D final : PlatformNS::ITexture {
		bgfx::TextureHandle texture{};

		void Texture2D(size_t width, size_t height, bool mipMap, SurfaceFormat format) override;

		~BgfxTexture2D() override {
			if (bgfx::isValid(texture))
				bgfx::destroy(texture);
		}
	};

	std::unique_ptr<PlatformNS::ITexture> PlatformNS::ITexture::Create() {
		return std::make_unique<BgfxTexture2D>();
	}

	void BgfxTexture2D::Texture2D(size_t width, size_t height, bool mipMap, SurfaceFormat format) {
		if (bgfx::isValid(texture)) 
			bgfx::destroy(texture);

		assert(format == SurfaceFormat::Color && "Image format not supported.");
		assert(width > 0 && "The texture width must be greater than 0.");
		assert(height > 0 && "The texture height must be greater than 0.");
		
		texture = bgfx::createTexture2D(
			//As dimensões da textura em pixels.
			width, height,
			//Indica se a textura terá Mipmaps (versões menores da imagem para quando ela estiver longe da câmera).
			mipMap, 
			//Define se isso é uma única textura ou um Texture Array.
			1,
			//O layout dos bits na memória.
			bgfx::TextureFormat::RGBA8,
			//Flags de Recurso e Sampler
			BGFX_TEXTURE_NONE | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP,
			//O ponteiro para os dados da imagem.
			NULL // Dados da imagem
		);
	}
}