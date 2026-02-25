#include "Xna/Platform/_Platform.hpp"
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/debug.h>
#include <stdexcept>
#include <cstdint>

#include "Xna/Framework/Graphics/Texture2D.hpp"

namespace Xna {
	struct BgfxTexture2D final : PlatformNS::ITexture2D {
		bgfx::TextureHandle textureHandle{};
		size_t width{ 0 };
		size_t height{ 0 };
		bool mipMap{ false };

		void Initialize(size_t width, size_t height, bool mipMap, SurfaceFormat format) override;
		void SetData(size_t level, std::optional<Rectangle> const& rect, const void* data,
			size_t startIndex, size_t elementCount, bool hasMipMap, size_t sizeOfData) override;
		void GetData(size_t level, std::optional<Rectangle> const& rect, void* data,
			size_t startIndex, size_t elementCount, size_t sizeOfData) override;

		~BgfxTexture2D() override {
			if (bgfx::isValid(textureHandle))
				bgfx::destroy(textureHandle);
		}
	};

	std::unique_ptr<PlatformNS::ITexture2D> PlatformNS::ITexture2D::Create() {
		return std::make_unique<BgfxTexture2D>();
	}

	void BgfxTexture2D::Initialize(size_t width, size_t height, bool mipMap, SurfaceFormat format) {
		if (bgfx::isValid(textureHandle)) 
			bgfx::destroy(textureHandle);

		assert(format == SurfaceFormat::Color && "Image format not supported.");
		assert(width > 0 && "The textureHandle width must be greater than 0.");
		assert(height > 0 && "The textureHandle height must be greater than 0.");

		textureHandle = bgfx::createTexture2D(
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

		if (!bgfx::isValid(textureHandle))
			throw std::runtime_error("bgfx::createTexture2D failed.");

		this->width = width;
		this->height = height;
		this->mipMap = mipMap;
	}

	void BgfxTexture2D::SetData(size_t level, std::optional<Rectangle> const& rect, const void* data,
		size_t startIndex, size_t elementCount, bool hasMipMap, size_t sizeOfData) {			
		
		assert(data != nullptr && "data cannot be null.");
		assert(startIndex < elementCount && "startIndex >= elementCount");
		assert(sizeOfData > 0 && "sizeOfData == 0");

		int x = 0;
		int y = 0;
		int w = width;
		int h = height;

		if (rect) {
			assert((rect->Width >= 0 && rect->Width <= width) && "rect->Width > textureHandle width");
			assert((rect->Height >= 0 && rect->Height <= height) && "rect->Height > textureHandle height");
			assert((rect->X >= 0 && rect->X < width) && "rect->X < 0 || rect->X >= width");
			assert((rect->Y >= 0 && rect->Y < height) && "rect->Y < 0 || rect->Y >= height");

			x = static_cast<int>(rect->X);
			y = static_cast<int>(rect->Y);
			w = static_cast<int>(rect->Width);
			h = static_cast<int>(rect->Height);
		}		

		// O bgfx::copy cria uma cópia segura dos dados para a GPU
		const auto buffer = reinterpret_cast<const uint8_t*>(data);
		const bgfx::Memory* mem = bgfx::copy(buffer + startIndex, elementCount * sizeOfData);

		// 2. Atualize a textura
		bgfx::updateTexture2D(
			textureHandle,          // O handle da textura
			0,						// Camada 0 para texturas simples
			hasMipMap ? 1 : 0,		// Nível de Mip
			x, y,					// Coordenadas X e Y
			w, h,					// Largura e Altura da região
			mem						// O bloco de memória com os novos pixels
		);		
	}

	void BgfxTexture2D::GetData(size_t level, std::optional<Rectangle> const& rect, void* data,
		size_t startIndex, size_t elementCount, size_t sizeOfData) {

		//TODO: não implementado
	}
}