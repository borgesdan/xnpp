#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/macros.h>
#include <bx/debug.h>
#include <cstdint>
#include <stdexcept>
#include "SDL3/SDL.h"
#include "Xna/Platform/_Platform.hpp"

#include "Xna/Framework/Graphics/BlendState.hpp"
#include "Xna/Framework/Graphics/DepthStencilState.hpp"
#include "Xna/Framework/Graphics/GraphicsDevice.hpp"

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
		constexpr BgfxBlendOperation(BlendOperation op) {
			switch (op) {
			case BlendOperation::Add:
				value = BgfxBlendOperation::Add;
				break;
			case BlendOperation::Subtract:
				value = BgfxBlendOperation::Subtract;
				break;
			case BlendOperation::ReverseSubtract:
				value = BgfxBlendOperation::ReverseSubtract;
				break;
			case BlendOperation::Min:
				value = BgfxBlendOperation::Min;
				break;
			case BlendOperation::Max:
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

	//Padrão do CompareFunction para Bgfx
	struct BgfxCompareFunction {
		static constexpr uint64_t Never = BGFX_STENCIL_TEST_NEVER;
		static constexpr uint64_t Less = BGFX_STENCIL_TEST_LESS;
		static constexpr uint64_t Equal = BGFX_STENCIL_TEST_EQUAL;
		static constexpr uint64_t LessEqual = BGFX_STENCIL_TEST_LEQUAL;
		static constexpr uint64_t Greater = BGFX_STENCIL_TEST_GREATER;
		static constexpr uint64_t NotEqual = BGFX_STENCIL_TEST_NOTEQUAL;
		static constexpr uint64_t GreaterEqual = BGFX_STENCIL_TEST_GEQUAL;
		static constexpr uint64_t Always = BGFX_STENCIL_TEST_ALWAYS;

		constexpr BgfxCompareFunction(CompareFunction func) {
			switch (func)
			{
			case Xna::CompareFunction::Never:
				value = Never;
				break;
			case Xna::CompareFunction::Less:
				value = Less;
				break;
			case Xna::CompareFunction::Equal:
				value = Equal;
				break;
			case Xna::CompareFunction::LessEqual:
				value = LessEqual;
				break;
			case Xna::CompareFunction::Greater:
				value = Greater;
				break;
			case Xna::CompareFunction::NotEqual:
				value = NotEqual;
				break;
			case Xna::CompareFunction::GreaterEqual:
				value = GreaterEqual;
				break;
			case Xna::CompareFunction::Always:
				value = Always;
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
		uint64_t cacheBlendState{ 0 };
		uint64_t cacheBlendFactor{ 0 };

		void LazyInitialization1(intptr_t windowHandle) override;
		void ApplyBlendState(BlendState const& blend) override;
		void ApplyDepthStencilState(DepthStencilState const& depth) override;
	};

	//Com DirectX11 inicializamos o Swapchain após a criação da janela no GameHost.
	//O dispositivo gráfico é criado antes, após o GameHost criar a janela e depois o Swapchain é criado.
	//Com o bgfx precisamos da informação da janela para inicializá-lo.
	void BgfxGraphicsDevice::LazyInitialization1(intptr_t windowHandle) {
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

		bgfx::Init init{};
		init.platformData = pd;
		init.type = bgfx::RendererType::Count;

		if (!bgfx::init(init))
			throw std::runtime_error("bgfx init failed");
	}

	void BgfxGraphicsDevice::ApplyBlendState(BlendState const& blend) {
		// 1: Adicionar o estado padrão
		uint64_t state = 0
			| BGFX_STATE_WRITE_RGB
			| BGFX_STATE_WRITE_A
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_MSAA;

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

		cacheBlendState = state;
		cacheBlendFactor = blendFactor;
	}

	void BgfxGraphicsDevice::ApplyDepthStencilState(DepthStencilState const& depth) {
		uint64_t state = 0;	
		
	}
}