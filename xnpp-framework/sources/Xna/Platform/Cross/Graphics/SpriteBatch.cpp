#include "Xna/Platform/_Platform.hpp"
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/debug.h>
#include <stdexcept>

#include "Xna/Framework/Graphics/SpriteBatch.hpp"

namespace Xna {
    struct SpriteVertex {
        float x, y, z;
        uint32_t abgr; // Cor no formato que o bgfx/DirectX gosta
        float u, v;

        static void init() {
            ms_layout
                .begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
                .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
                .end();
        }
        static bgfx::VertexLayout ms_layout;
    };
    bgfx::VertexLayout SpriteVertex::ms_layout;

	struct BgfxSpriteBatch final : public PlatformNS::ISpriteBatch {
		~BgfxSpriteBatch() override = default;
	};
}