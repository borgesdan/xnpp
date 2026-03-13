#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/debug.h>
#include <cstdint>
#include <stdexcept>
#include "SDL3/SDL.h"
#include "Xna/Platform/Platform.hpp"
#include "../Internal.hpp"

#include "BgfxGraphics.hpp"

#include "Xna/Framework/Graphics/BlendState.hpp"
#include "Xna/Framework/Graphics/DepthStencilState.hpp"
#include "Xna/Framework/Graphics/GraphicsDevice.hpp"
#include "Xna/Framework/Graphics/RasterizerState.hpp"
#include <bx/math.h>

namespace Xna {
	struct GraphicsDeviceCache {
		BlendState blendState;
		DepthStencilState depthStencilState;
		RasterizerState rasterizerState;
		Color blendFactor;
	};

	// Bgfx: Stateless por design
	// Ao contrário do XNA o bgfx limpa o estado configurado após cada bgfx::submit().
	// Necessário chamar bgfx::setState (funções set/apply) em cada frame para cada objeto de desenho.
	struct BgfxGraphicsDevice final : public PlatformNS::IGraphicsDevice {
		//state = currentBlendState | currentDepthBuffer | currentRasterizerState
		//bgfx::setState(state, blendFactor);
		BgfxBlendState currentBlendState{ BlendState::Opaque() };
		BgfxDepthStencilState currentDepthStencilState{ DepthStencilState::Default() };
		BgfxRasterizerState currentRasterizerState{ RasterizerState::CullCounterClockwise() };
		uint64_t blendFactor{ SwapXnaColor(Color::White()) };

		GraphicsDeviceCache cache;

		void CreateDevice(GraphicsAdapter const& adapter, Xna::PresentationParameters const& presentationParameters) override {
			//Nada a fazer
			//Diferente do DirectX11, não necessitamos criar o dispositivo gráfico aqui
			//A criação será feita no Initialize onde receberemos a janela de jogo
		}

		void Present(std::optional<Rectangle> const& rec, std::optional<Rectangle> const& destination, intptr_t overrideWindowHandle) override {
			const uint64_t state = currentBlendState | currentDepthStencilState | currentRasterizerState;
			bgfx::setState(state, currentBlendState.blendFactor);

			bgfx::frame();
		}

		void SetViewport(Viewport const& viewport) override {

		}

		void MakeWindowAssociation(PresentationParameters const& pp) override {}
		void Reset(Xna::PresentationParameters const& presentationParameters, GraphicsAdapter const& graphicsAdapter) {}
		void Initialize(intptr_t windowHandle) override;
		void ApplyBlendState(BlendState const& blend) override;
		void ApplyDepthStencilState(DepthStencilState const& depth) override;
		void ApplyRasterizerState(RasterizerState const& rasterizer) override;
		
		void ApplyBlendFactor(Color const& color) override {
			blendFactor = SwapXnaColor(color);
			cache.blendFactor = color;
		}

		void Clear(ClearOptions options, Color const& color, float depth, int32_t stencil) override {
			// 1. Garante que o View 0 seja processado (mesmo sem draw calls)
			//bgfx::touch(0);		
			const auto clearColor = SwapXnaColor(color);
			bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, clearColor, 1.0f, 0);

			bgfx::touch(0);
		}

		const BlendState& GetBlendState() const override { return cache.blendState; }
		const DepthStencilState& GetDepthStencilState() const override { return cache.depthStencilState; }
		const RasterizerState& GetRasterizerState() const override { return cache.rasterizerState; }
		const Color GetBlendFactor() const override { return cache.blendFactor; }



		~BgfxGraphicsDevice() override = default;
	};

	//Com DirectX11 inicializamos o Swapchain após a criação da janela no GameHost.
	//O dispositivo gráfico é criado antes, após o GameHost criar a janela e depois o Swapchain é criado.
	//Com o bgfx precisamos da informação da janela para inicializá-lo.
	void BgfxGraphicsDevice::Initialize(intptr_t windowHandle) {
		auto window = reinterpret_cast<SDL_Window*>(windowHandle);

		bgfx::PlatformData pd{};

#ifdef PLATFORM_WINDOWS
		pd.nwh = SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
#elif PLATFORM_LINUX
		const auto props = SDL_GetWindowProperties(window);
		const auto driver = SDL_GetCurrentVideoDriver();
		std::string driver_name = driver;

		if (driver_name == "wayland") {
			pd.ndt = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, NULL);
			pd.nwh = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, NULL);
		}
		else {
			pd.nwh = SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_WINDOW_NUMBER, NULL);
			pd.ndt = SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_DISPLAY_POINTER, NULL);
		}
#elif PLATFORM_MACOS
		pd.nwh = (void*)SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, NULL);
#endif	

		int w, h;
		if (!SDL_GetWindowSize(window, &w, &h))
			throw std::runtime_error("Invalid game window.");

		bgfx::Init init{};
		init.type = bgfx::RendererType::Count;
		init.resolution.width = static_cast<uint32_t>(w);
		init.resolution.height = static_cast<uint32_t>(h);
		init.resolution.reset = BGFX_RESET_VSYNC;
		init.platformData = pd;

		if (!bgfx::init(init))
			throw std::runtime_error("bgfx init failed");

		float ortho[16];
		bx::mtxOrtho(ortho, 0.0f, w, h, 0.0f, 0.0f, 1.0f, 0.0f, bgfx::getCaps()->homogeneousDepth);
		bgfx::setViewTransform(0, NULL, ortho);
		bgfx::setViewRect(0, 0, 0, w, h);

		const auto clearColor = SwapXnaColor(Colors::CornflowerBlue);
		bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, clearColor, 1.0f, 0);
	}

	void BgfxGraphicsDevice::ApplyBlendState(BlendState const& blend) {
		currentBlendState = blend;
		blendFactor = SwapXnaColor(blend.BlendFactor);
		
		cache.blendState = blend;
		cache.blendFactor = blend.BlendFactor;
	}

	void BgfxGraphicsDevice::ApplyDepthStencilState(DepthStencilState const& depth) {
		currentDepthStencilState = depth;
		cache.depthStencilState = depth;
	}

	void BgfxGraphicsDevice::ApplyRasterizerState(RasterizerState const& rasterizer) {
		currentRasterizerState = rasterizer;
		cache.rasterizerState = rasterizer;
	}

	std::unique_ptr<PlatformNS::IGraphicsDevice> PlatformNS::IGraphicsDevice::Create() {
		return std::make_unique<BgfxGraphicsDevice>();
	}

	size_t PlatformNS::Graphics_GetMaxSamplerStates() {
		return 16;
	}

	void Cross::System::DisposeVideo() {
		//bgfx::shutdown();
	}
}