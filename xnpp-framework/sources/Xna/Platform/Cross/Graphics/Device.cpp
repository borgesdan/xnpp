#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/debug.h>
#include <cstdint>
#include <stdexcept>
#include "SDL3/SDL.h"
#include "Xna/Platform/_Platform.hpp"

#include "BgfxGraphics.hpp"

#include "Xna/Framework/Graphics/BlendState.hpp"
#include "Xna/Framework/Graphics/DepthStencilState.hpp"
#include "Xna/Framework/Graphics/GraphicsDevice.hpp"
#include "Xna/Framework/Graphics/RasterizerState.hpp"
#include <bx/math.h>

namespace Xna {
	// Bgfx: Stateless por design
	// Ao contrário do XNA o bgfx limpa o estado configurado após cada bgfx::submit().
	// Necessário chamar bgfx::setState (funções set/apply) em cada frame para cada objeto de desenho.
	struct BgfxGraphicsDevice final : public PlatformNS::IGraphicsDevice {
		//O program é o handle de um par de shaders compilados e vinculados: o Vertex Shader e o Fragment Shader.
		//usa a função bgfx::createProgram.
		bgfx::ProgramHandle program{};

		//state = cacheBlendState | cacheDepthBuffer | cachedRasterizerState
		//bgfx::setState(state, blendFactor);
		uint64_t cachedBlendState{ 0 };
		uint64_t cachedBlendFactor{ 0 };
		uint64_t cachedDepthBuffer{ 0 };
		uint64_t cachedRasterizerState{ 0 };

		//fStencil = Front Face
		//bStencil = Back Face (CounterClockWise)
		//bgfx::setStencil(fStencil, bStencil);
		uint32_t cachedFrontFaceStencil{ 0 };
		uint32_t cachedBackFaceStencil{ 0 };

		// Para habilitar (equivalente a ScissorTestEnable = true)
		//bgfx::setScissor(x, y, width, height);
		// Para desabilitar (equivalente a ScissorTestEnable = false)
		//bgfx::setScissor(); // Chama sem argumentos ou com zeros
		bool cachedScissorTestEnable{ false };		

		void CreateDevice(GraphicsAdapter const& adapter, Xna::PresentationParameters const& presentationParameters) override {
			//Nada a fazer
			//Diferente do DirectX11, não necessitamos criar o dispositivo gráfico aqui
			//A criação será feita no LazyInitialization onde receberemos a janela de jogo
		}

		void Present(std::optional<Rectangle> const& rec, std::optional<Rectangle> const& destination, intptr_t overrideWindowHandle) override {
			// 2. Avança o frame e renderiza o que foi enviado
			bgfx::frame();
		}

		void SetViewport(Viewport const& viewport) override {

		}

		void MakeWindowAssociation(PresentationParameters const& pp) override {}
		void Reset(Xna::PresentationParameters const& presentationParameters, GraphicsAdapter const& graphicsAdapter) {}
		void LazyInitialization(intptr_t windowHandle) override;
		void ApplyBlendState(BlendState const& blend) override;
		void ApplyDepthStencilState(DepthStencilState const& depth) override;
		void ApplyRasterizerState(RasterizerState const& rasterizer) override;
		void Clear(ClearOptions options, Color const& color, float depth, int32_t stencil) override {
			// 1. Garante que o View 0 seja processado (mesmo sem draw calls)
			//bgfx::touch(0);		
			const auto clearColor = SwapXnaColor(color);
			bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, clearColor, 1.0f, 0);

			bgfx::touch(0);
		}

		~BgfxGraphicsDevice() override = default;
	};

	//Com DirectX11 inicializamos o Swapchain após a criação da janela no GameHost.
	//O dispositivo gráfico é criado antes, após o GameHost criar a janela e depois o Swapchain é criado.
	//Com o bgfx precisamos da informação da janela para inicializá-lo.
	void BgfxGraphicsDevice::LazyInitialization(intptr_t windowHandle) {
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
		const BgfxBlendState b = blend;

		cachedBlendState = b.state;
		cachedBlendFactor = b.blendFactor;
	}	

	void BgfxGraphicsDevice::ApplyDepthStencilState(DepthStencilState const& depth) {
		const BgfxDepthStencilState state = depth;
		cachedDepthBuffer = state.depthBuffer;
		cachedFrontFaceStencil = state.frontStencil;
		cachedBackFaceStencil = state.backStencil;		
	}

	void BgfxGraphicsDevice::ApplyRasterizerState(RasterizerState const& rasterizer) {
		const BgfxRasterizerState state = rasterizer;

		cachedRasterizerState = state;
		cachedScissorTestEnable = rasterizer.ScissorTestEnable;
	}

	std::unique_ptr<PlatformNS::IGraphicsDevice> PlatformNS::IGraphicsDevice::Create() {
		return std::make_unique<BgfxGraphicsDevice>();
	}

	size_t PlatformNS::Graphics_GetMaxSamplerStates() {
		return 16;
	}
}