#ifndef XNA_BGFX_GRAPHICS
#define XNA_BGFX_GRAPHICS

#include "Xna/Framework/Graphics/Texture2D.hpp"
#include "Xna/Platform/_Platform.hpp"
#include "Xna/Framework/Color.hpp"
#include <bgfx/bgfx.h>
#include <cstdint>

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

	constexpr uint32_t SwapXnaColor(Color const& color) {
		uint32_t xnaColor = color.PackedValue();
		uint32_t bgfxColor = ((xnaColor & 0x000000FF) << 24) | // R
			((xnaColor & 0x0000FF00) << 8) | // G
			((xnaColor & 0x00FF0000) >> 8) | // B
			((xnaColor & 0xFF000000) >> 24);  // A

		return bgfxColor;
	}
}

#endif