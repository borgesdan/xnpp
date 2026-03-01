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

namespace Xna {
	//Padrão do BlendState para Bgfx
	struct BgfxBlendState {
		static constexpr uint64_t Opaque = 0;
		static constexpr uint64_t AlphaBlend = BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);
		static constexpr uint64_t NonPremultiplied = AlphaBlend;
		static constexpr uint64_t Additive = BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_ONE, BGFX_STATE_BLEND_ONE);

		constexpr operator uint64_t() const noexcept { return value; }

		constexpr BgfxBlendState() = default;
		constexpr BgfxBlendState(BlendState const& blend) {
			if (blend.IsOpaque())
				value = BgfxBlendState::Opaque;
			else if (blend.IsAlphaBlend())
				value = BgfxBlendState::AlphaBlend;
			else if (blend.IsNonPremultiplied())
				value = BgfxBlendState::NonPremultiplied;
			else if (blend.IsAdditive())
				value = BgfxBlendState::Additive;
			else {
				throw std::runtime_error("Invalid BlendState");
			}
		}

		uint64_t value{ 0 };
	};

	//Padrão do BlendOperation para Bgfx
	struct BgfxBlendOperation {
		static constexpr uint64_t Add = BGFX_STATE_BLEND_EQUATION_ADD;
		static constexpr uint64_t Subtract = BGFX_STATE_BLEND_EQUATION_SUB;
		static constexpr uint64_t ReverseSubtract = BGFX_STATE_BLEND_EQUATION_REVSUB;
		static constexpr uint64_t Min = BGFX_STATE_BLEND_EQUATION_MIN;
		static constexpr uint64_t Max = BGFX_STATE_BLEND_EQUATION_MAX;

		constexpr operator uint64_t() const noexcept { return value; }

		constexpr BgfxBlendOperation() = default;
		constexpr BgfxBlendOperation(BlendFunction op) {
			switch (op) {
			case BlendFunction::Add:
				value = BgfxBlendOperation::Add;
				break;
			case BlendFunction::Subtract:
				value = BgfxBlendOperation::Subtract;
				break;
			case BlendFunction::ReverseSubtract:
				value = BgfxBlendOperation::ReverseSubtract;
				break;
			case BlendFunction::Min:
				value = BgfxBlendOperation::Min;
				break;
			case BlendFunction::Max:
				value = BgfxBlendOperation::Max;
				break;
			}
		}

		uint64_t value{ 0 };
	};

	//Padrão do ColorWriterChannel para Bgfx
	struct BgfxColorWriteChannel {
		static constexpr uint64_t Red = BGFX_STATE_WRITE_R;
		static constexpr uint64_t Green = BGFX_STATE_WRITE_G;
		static constexpr uint64_t Blue = BGFX_STATE_WRITE_B;
		static constexpr uint64_t Alpha = BGFX_STATE_WRITE_A;
		static constexpr uint64_t All = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A;

		constexpr BgfxColorWriteChannel() = default;

		constexpr BgfxColorWriteChannel(ColorWriteChannels channel) {
			switch (channel)
			{
			case ColorWriteChannels::Red:
				value = Red;
				break;
			case ColorWriteChannels::Green:
				value = Green;
				break;
			case ColorWriteChannels::Blue:
				value = Blue;
				break;
			case ColorWriteChannels::Alpha:
				value = Alpha;
				break;
			case ColorWriteChannels::All:
				value = All;
				break;
			default:
				break;
			}
		}

		constexpr operator uint64_t() const noexcept { return value; }

		uint64_t value{ 0 };
	};

	//Padrão do Blend para Bgfx
	struct BgfxBlend {
		static constexpr uint64_t One = BGFX_STATE_BLEND_ONE;
		static constexpr uint64_t SourceColor = BGFX_STATE_BLEND_SRC_COLOR;
		static constexpr uint64_t InverseSourceColor = BGFX_STATE_BLEND_INV_SRC_COLOR;
		static constexpr uint64_t SourceAlpha = BGFX_STATE_BLEND_SRC_ALPHA;
		static constexpr uint64_t InverseSourceAlpha = BGFX_STATE_BLEND_INV_SRC_ALPHA;
		static constexpr uint64_t DestinationAlpha = BGFX_STATE_BLEND_DST_ALPHA;
		static constexpr uint64_t InverseDestinationAlpha = BGFX_STATE_BLEND_INV_DST_ALPHA;
		static constexpr uint64_t DestinationColor = BGFX_STATE_BLEND_DST_COLOR;
		static constexpr uint64_t InverseDestinationColor = BGFX_STATE_BLEND_INV_DST_COLOR;
		static constexpr uint64_t SourceAlphaSaturation = BGFX_STATE_BLEND_SRC_ALPHA_SAT;
		static constexpr uint64_t BlendFactor = BGFX_STATE_BLEND_FACTOR;
		static constexpr uint64_t InverseBlendFactor = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A;

		constexpr BgfxBlend() = default;
		constexpr BgfxBlend(Blend blend) {
			switch (blend)
			{
			case Xna::Blend::One:
				value = One;
				break;
			case Xna::Blend::SourceColor:
				value = SourceColor;
				break;
			case Xna::Blend::InverseSourceColor:
				value = InverseSourceColor;
				break;
			case Xna::Blend::SourceAlpha:
				value = SourceAlpha;
				break;
			case Xna::Blend::InverseSourceAlpha:
				value = InverseSourceAlpha;
				break;
			case Xna::Blend::DestinationAlpha:
				value = DestinationAlpha;
				break;
			case Xna::Blend::InverseDestinationAlpha:
				value = InverseDestinationAlpha;
				break;
			case Xna::Blend::DestinationColor:
				value = DestinationColor;
				break;
			case Xna::Blend::InverseDestinationColor:
				value = InverseDestinationColor;
				break;
			case Xna::Blend::SourceAlphaSaturation:
				value = SourceAlphaSaturation;
				break;
			case Xna::Blend::BlendFactor:
				value = BlendFactor;
				break;
			case Xna::Blend::InverseBlendFactor:
				value = InverseBlendFactor;
				break;
			default:
				break;
			}
		}

		constexpr operator uint64_t() const noexcept { return value; }
		uint64_t value{ 0 };
	};
	
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
		uint32_t cachedFrontFaceStencil{0};
		uint32_t cachedBackFaceStencil{0};

		// Para habilitar (equivalente a ScissorTestEnable = true)
		//bgfx::setScissor(x, y, width, height);
		// Para desabilitar (equivalente a ScissorTestEnable = false)
		//bgfx::setScissor(); // Chama sem argumentos ou com zeros
		bool cachedScissorTestEnable{ false };

		//XNA: DepthBias = 0.001f, SlopeScaleDepthBias = 1.5f
		//bgfx::setDepthBias(0.001f, 1.5f);
		float depthBias{ 0 };
		float SlopeScaleDepthBias{ 0 };

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
		if(!SDL_GetWindowSize(window, &w, &h))
			throw std::runtime_error("Invalid game window.");

		bgfx::Init init{};
		init.type = bgfx::RendererType::Count;
		init.resolution.width = static_cast<uint32_t>(w);
		init.resolution.height = static_cast<uint32_t>(h);
		init.resolution.reset = BGFX_RESET_VSYNC;
		init.platformData = pd;

		if (!bgfx::init(init))
			throw std::runtime_error("bgfx init failed");

		// Definindo o View 0 (o "canvas" principal)
		bgfx::setViewRect(0, 0, 0, static_cast<uint16_t>(w), static_cast<uint16_t>(h));

		//Cornflower Blue (RGBA)		
		//uint32_t clearColor = 0x6495EDFF;
		const auto clearColor = SwapXnaColor(Colors::CornflowerBlue);
		bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, clearColor, 1.0f, 0);		
	}

	void BgfxGraphicsDevice::ApplyBlendState(BlendState const& blend) {
		// 1: Adicionar o estado padrão
		uint64_t state = 0
			| BGFX_STATE_WRITE_RGB
			| BGFX_STATE_WRITE_A
			| BGFX_STATE_DEPTH_TEST_LESS;

		// 2: Adicionar o bit específico de Blend
		const BgfxBlendState blendState = blend;
		state |= blendState;

		const BgfxBlendOperation rgbOp = blend.ColorBlendFunction;
		const BgfxBlendOperation alphaOp = blend.AlphaBlendFunction;

		// 3: Definir a operação
		if (rgbOp != alphaOp)
			state |= BGFX_STATE_BLEND_EQUATION_SEPARATE(rgbOp, alphaOp);
		else if (rgbOp != BgfxBlendOperation::Add) //Add é o padrão no bgfx
			state |= rgbOp;

		// 4: Aplicar canais de cores

		const BgfxColorWriteChannel channel0 = blend.ColorWriteChannels;

		//TODO: O state é global por draw
		//XNA suporta ColorWriteChannels0..3
		//bgfx suporta apenas uma máscara por draw
		//
		//MRT: Multiple Render Targets.
		//XNA máscara por render target.
		//bgfx é global por draw, não por render target.
		state |= channel0;

		BX_TRACE("BlendState: ColorWriteChannels1 ignored.");
		BX_TRACE("BlendState: ColorWriteChannels2 ignored.");
		BX_TRACE("BlendState: ColorWriteChannels3 ignored.");
		BX_TRACE("BlendState: MultiSampleMask ignored.");

		// 6: Aplicar o estado + blendFactor
		uint32_t blendFactor = 0U;

		if ((state & BGFX_STATE_BLEND_FACTOR) == BGFX_STATE_BLEND_FACTOR
			|| (state & BGFX_STATE_BLEND_INV_FACTOR) == BGFX_STATE_BLEND_INV_FACTOR) {
			blendFactor = blend.BlendFactor.PackedValue();
		}

		cachedBlendState = state;
		cachedBlendFactor = blendFactor;
	}

	// Mapeamento de ComparisonFunction (XNA) para bgfx
	static uint64_t toBgfxDepthFunc(CompareFunction func) {
		switch (func) {
		case CompareFunction::Never:        return BGFX_STATE_DEPTH_TEST_NEVER;
		case CompareFunction::Less:         return BGFX_STATE_DEPTH_TEST_LESS;
		case CompareFunction::Equal:        return BGFX_STATE_DEPTH_TEST_EQUAL;
		case CompareFunction::LessEqual:    return BGFX_STATE_DEPTH_TEST_LEQUAL;
		case CompareFunction::Greater:      return BGFX_STATE_DEPTH_TEST_GREATER;
		case CompareFunction::NotEqual:     return BGFX_STATE_DEPTH_TEST_NOTEQUAL;
		case CompareFunction::GreaterEqual: return BGFX_STATE_DEPTH_TEST_GEQUAL;
		case CompareFunction::Always:       return BGFX_STATE_DEPTH_TEST_ALWAYS;
		default: return BGFX_STATE_DEPTH_TEST_LEQUAL;
		}
	}

	// Mapeamento de StencilOperation (XNA) para bgfx
	// O parâmetro 'shift' ajusta se a operação é para Fail, ZFail ou Pass
	static uint32_t toBgfxStencilOp(StencilOperation op, uint32_t shift) {
		uint32_t val = 0;
		switch (op) {
		case StencilOperation::Keep:                val = BGFX_STENCIL_OP_FAIL_S_KEEP; break;
		case StencilOperation::Zero:                val = BGFX_STENCIL_OP_FAIL_S_ZERO; break;
		case StencilOperation::Replace:             val = BGFX_STENCIL_OP_FAIL_S_REPLACE; break;
		case StencilOperation::IncrementSaturation: val = BGFX_STENCIL_OP_FAIL_S_INCRSAT; break;
		case StencilOperation::DecrementSaturation: val = BGFX_STENCIL_OP_FAIL_S_DECRSAT; break;
		case StencilOperation::Invert:              val = BGFX_STENCIL_OP_FAIL_S_INVERT; break;
		case StencilOperation::Increment:           val = BGFX_STENCIL_OP_FAIL_S_INCR; break;
		case StencilOperation::Decrement:           val = BGFX_STENCIL_OP_FAIL_S_DECR; break;
		}
		return val << shift;
	}

	void BgfxGraphicsDevice::ApplyDepthStencilState(DepthStencilState const& depth) {
		uint64_t outStateBits = 0;

		if (depth.DepthBufferEnable) {
			outStateBits |= toBgfxDepthFunc(depth.DepthBufferFunction);
			if (depth.DepthBufferWriteEnable) {
				outStateBits |= BGFX_STATE_WRITE_Z;
			}
		}

		cachedDepthBuffer = outStateBits;

		// --- STENCIL SECTION ---
		if (depth.StencilEnable) {
			// Front Face
			uint32_t fStencil = BGFX_STENCIL_FUNC_REF(depth.ReferenceStencil)
				| BGFX_STENCIL_FUNC_RMASK(depth.StencilMask)
				| (uint32_t)toBgfxDepthFunc(depth.StencilFunction) // Reutiliza mapeamento de comparação
				| toBgfxStencilOp(depth.StencilFail, 0)             // SFail
				| toBgfxStencilOp(depth.StencilDepthBufferFail, 4)  // ZFail
				| toBgfxStencilOp(depth.StencilPass, 8);            // Pass

			// Back Face (CCW)
			uint32_t bStencil = BGFX_STENCIL_NONE;
			if (depth.TwoSidedStencilMode) {
				bStencil = BGFX_STENCIL_FUNC_REF(depth.ReferenceStencil)
					| BGFX_STENCIL_FUNC_RMASK(depth.StencilMask)
					| (uint32_t)toBgfxDepthFunc(depth.CounterClockwiseStencilFunction)
					| toBgfxStencilOp(depth.CounterClockwiseStencilFail, 0)
					| toBgfxStencilOp(depth.CounterClockwiseStencilDepthBufferFail, 4)
					| toBgfxStencilOp(depth.CounterClockwiseStencilPass, 8);
			}

			cachedFrontFaceStencil = fStencil;
			cachedBackFaceStencil = bStencil;
			
			// Nota: O Write Mask é global para o stencil no bgfx
			// Talvez precisamos gerenciar o write mask via bgfx::setState se for complexo, 
			// mas geralmente setStencil deve resolver.
		}
		else {
			bgfx::setStencil(BGFX_STENCIL_NONE);
		}
	}

	void BgfxGraphicsDevice::ApplyRasterizerState(RasterizerState const& rasterizer) {
		uint64_t state = 0;

		if (rasterizer.MultiSampleAntiAlias)
			state |= BGFX_STATE_MSAA;

		const auto cullMode = rasterizer.CullMode;

		//None: Comportamento padrão do bgfx.
		if (cullMode == CullMode::CullClockwiseFace)
			state |= BGFX_STATE_CULL_CW;
		else if (cullMode == CullMode::CullCounterClockwiseFace)
			state |= BGFX_STATE_CULL_CCW;

		const auto fillMode = rasterizer.FillMode;

		//Solid: Comportamento padrão do bgfx.
		if (fillMode == FillMode::WireFrame)
			state |= BGFX_STATE_PT_LINES;
		

		cachedScissorTestEnable = rasterizer.ScissorTestEnable;
		cachedRasterizerState = state;
	}

	std::unique_ptr<PlatformNS::IGraphicsDevice> PlatformNS::IGraphicsDevice::Create() {
		return std::make_unique<BgfxGraphicsDevice>();
	}

	size_t PlatformNS::Graphics_GetMaxSamplerStates() {
		return 16;
	}
}