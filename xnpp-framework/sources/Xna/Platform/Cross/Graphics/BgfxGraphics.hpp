#ifndef XNA_BGFX_GRAPHICS
#define XNA_BGFX_GRAPHICS

#include "Xna/Framework/Graphics/Texture2D.hpp"
#include "Xna/Framework/Graphics/BlendState.hpp"
#include "Xna/Framework/Graphics/SamplerState.hpp"
#include "Xna/Framework/Graphics/DepthStencilState.hpp"
#include "Xna/Framework/Graphics/RasterizerState.hpp"
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

	//Padrão do BlendOperation para Bgfx
	struct BgfxBlendOperation {
		static constexpr uint64_t Add = BGFX_STATE_BLEND_EQUATION_ADD;
		static constexpr uint64_t Subtract = BGFX_STATE_BLEND_EQUATION_SUB;
		static constexpr uint64_t ReverseSubtract = BGFX_STATE_BLEND_EQUATION_REVSUB;
		static constexpr uint64_t Min = BGFX_STATE_BLEND_EQUATION_MIN;
		static constexpr uint64_t Max = BGFX_STATE_BLEND_EQUATION_MAX;

		constexpr operator uint64_t() const noexcept { return state; }

		constexpr BgfxBlendOperation() = default;
		constexpr BgfxBlendOperation(BlendFunction op) {
			switch (op) {
			case BlendFunction::Add:
				state = BgfxBlendOperation::Add;
				break;
			case BlendFunction::Subtract:
				state = BgfxBlendOperation::Subtract;
				break;
			case BlendFunction::ReverseSubtract:
				state = BgfxBlendOperation::ReverseSubtract;
				break;
			case BlendFunction::Min:
				state = BgfxBlendOperation::Min;
				break;
			case BlendFunction::Max:
				state = BgfxBlendOperation::Max;
				break;
			}
		}

		uint64_t state{ 0 };
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
			if (channel == ColorWriteChannels::None)
				return;

			if (channel == ColorWriteChannels::All)
				state |= BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A;
			else {
				state |= ((int)channel & (int)ColorWriteChannels::Red) == (int)ColorWriteChannels::Red ? BGFX_STATE_WRITE_R : 0;
				state |= ((int)channel & (int)ColorWriteChannels::Green) == (int)ColorWriteChannels::Green ? BGFX_STATE_WRITE_G : 0;
				state |= ((int)channel & (int)ColorWriteChannels::Blue) == (int)ColorWriteChannels::Blue ? BGFX_STATE_WRITE_B : 0;
			}
		}

		constexpr operator uint64_t() const noexcept { return state; }

		uint64_t state{ 0 };
	};

	//Padrão do BlendState para Bgfx
	struct BgfxBlendState {
		static constexpr uint64_t OpaqueFlags = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A;

		static constexpr uint64_t AlphaBlendFlags = (BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A) |
			BGFX_STATE_BLEND_FUNC_SEPARATE(BGFX_STATE_BLEND_ONE, BGFX_STATE_BLEND_INV_SRC_ALPHA, BGFX_STATE_BLEND_ONE, BGFX_STATE_BLEND_INV_SRC_ALPHA);

		static constexpr uint64_t NonPremultipliedFlags = (BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A) |
			BGFX_STATE_BLEND_FUNC_SEPARATE(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA, BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);

		static constexpr uint64_t AdditiveFlags = (BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A) |
			BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_ONE, BGFX_STATE_BLEND_ONE);

		static constexpr BgfxBlendState Opaque() {
			BgfxBlendState b;
			b.state = OpaqueFlags;
			return b;
		}

		static constexpr BgfxBlendState AlphaBlend() {
			BgfxBlendState b;
			b.state = AlphaBlendFlags;
			return b;
		}

		static constexpr BgfxBlendState NonPremultiplied() {
			BgfxBlendState b;
			b.state = NonPremultipliedFlags;
			return b;
		}

		static constexpr BgfxBlendState Additive() {
			BgfxBlendState b;
			b.state = AdditiveFlags;
			return b;
		}

		constexpr BgfxBlendState(BlendState const& b) {
			const BgfxBlend srcRgb = b.ColorSourceBlend;
			const BgfxBlend destRgb = b.ColorDestinationBlend;
			const BgfxBlend srcAlp = b.AlphaSourceBlend;
			const BgfxBlend destAlp = b.AlphaDestinationBlend;

			state |= BGFX_STATE_BLEND_FUNC_SEPARATE(srcRgb, destRgb, srcAlp, destAlp);

			//XNA suporta ColorWriteChannels0..3
			//bgfx suporta apenas uma máscara por draw
			state |= BgfxColorWriteChannel(b.ColorWriteChannels);

			const BgfxBlendOperation rgbOp = b.ColorBlendFunction;
			const BgfxBlendOperation alphaOp = b.AlphaBlendFunction;

			// 3: Definir a operação
			if (rgbOp != alphaOp)
				state |= BGFX_STATE_BLEND_EQUATION_SEPARATE(rgbOp, alphaOp);
			else if (rgbOp != BgfxBlendOperation::Add) //Add é o padrão no bgfx
				state |= rgbOp;

			blendFactor = SwapXnaColor(b.BlendFactor);
		}

		constexpr BgfxBlendState() = default;
		constexpr operator uint64_t() const noexcept { return state; }

		uint64_t state{ 0 };
		uint64_t blendFactor{ 0 };
	};	

	struct BgfxTextureAddressMode {
		enum Mode {
			U, V, W
		};

		constexpr BgfxTextureAddressMode(TextureAddressMode address, Mode mode) {

			if (mode == U) {
				switch (address)
				{
				case TextureAddressMode::Wrap:
					//Default
					break;
				case TextureAddressMode::Mirror:
					state |= BGFX_SAMPLER_U_MIRROR;
					break;
				case TextureAddressMode::Clamp:
					state |= BGFX_SAMPLER_U_CLAMP;
					break;
				case TextureAddressMode::Border:
					state |= BGFX_SAMPLER_U_BORDER;
					break;
				default:
					break;
				}
			}

			if (mode == V) {
				switch (address)
				{
				case TextureAddressMode::Wrap:
					//Default
					break;
				case TextureAddressMode::Mirror:
					state |= BGFX_SAMPLER_V_MIRROR;
					break;
				case TextureAddressMode::Clamp:
					state |= BGFX_SAMPLER_V_CLAMP;
					break;
				case TextureAddressMode::Border:
					state |= BGFX_SAMPLER_V_BORDER;
					break;
				default:
					break;
				}
			}
			
			if (mode == W) {
				switch (address)
				{
				case TextureAddressMode::Wrap:
					//Default
					break;
				case TextureAddressMode::Mirror:
					state |= BGFX_SAMPLER_W_MIRROR;
					break;
				case TextureAddressMode::Clamp:
					state |= BGFX_SAMPLER_W_CLAMP;
					break;
				case TextureAddressMode::Border:
					state |= BGFX_SAMPLER_W_BORDER;
					break;
				default:
					break;
				}
			}			
		}		

		constexpr BgfxTextureAddressMode() = default;
		constexpr operator uint32_t() const noexcept { return state; }
		uint32_t state{ 0 };
	};

	struct BgfxTextureFilter {
		constexpr BgfxTextureFilter(TextureFilter const& f) {
			switch (f) {
			case TextureFilter::Point:
				state |= BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_MIP_POINT;
				break;
			case TextureFilter::Anisotropic:
				// Nota: No bgfx, o filtro anisotrópico geralmente assume linear como base
				state |= BGFX_SAMPLER_MIN_ANISOTROPIC | BGFX_SAMPLER_MAG_ANISOTROPIC;
				break;
			case TextureFilter::LinearMipPoint:
				// Min/Mag são lineares por padrão, forçamos apenas o Mip como Point
				state |= BGFX_SAMPLER_MIP_POINT;
				break;
			case TextureFilter::PointMipLinear:
				// Mip é linear por padrão, forçamos Min/Mag como Point
				state |= BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT;
				break;
			case TextureFilter::MinLinearMagPointMipLinear:
				state |= BGFX_SAMPLER_MAG_POINT;
				break;
			case TextureFilter::MinLinearMagPointMipPoint:
				state |= BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_MIP_POINT;
				break;
			case TextureFilter::MinPointMagLinearMipLinear:
				state |= BGFX_SAMPLER_MIN_POINT;
				break;
			case TextureFilter::MinPointMagLinearMipPoint:
				state |= BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MIP_POINT;
				break;
			case TextureFilter::Linear:
			default:
				state |= 0;
			}
		}

		constexpr BgfxTextureFilter() = default;
		constexpr operator uint32_t() const noexcept { return state; }
		uint32_t state{ 0 };
	};

	struct BgfxSamplerState {
		constexpr BgfxSamplerState(SamplerState const& s) {		
			uint32_t state = 0;
			
			state |= BgfxTextureAddressMode(s.AddressU, BgfxTextureAddressMode::U);
			state |= BgfxTextureAddressMode(s.AddressV, BgfxTextureAddressMode::V);
			state |= BgfxTextureAddressMode(s.AddressW, BgfxTextureAddressMode::W);
			state |= BgfxTextureFilter(s.Filter);
		}

		constexpr BgfxSamplerState() = default;
		constexpr operator uint64_t() const noexcept { return state; }
		uint64_t state{ 0 };
	};

	// Mapeamento de ComparisonFunction (XNA) para bgfx
	struct BgfxCompareFunction {

		constexpr BgfxCompareFunction(CompareFunction func) {
			switch (func) {
			case CompareFunction::Never:        state = BGFX_STATE_DEPTH_TEST_NEVER;
			case CompareFunction::Less:         state = BGFX_STATE_DEPTH_TEST_LESS;
			case CompareFunction::Equal:        state = BGFX_STATE_DEPTH_TEST_EQUAL;
			case CompareFunction::LessEqual:    state = BGFX_STATE_DEPTH_TEST_LEQUAL;
			case CompareFunction::Greater:      state = BGFX_STATE_DEPTH_TEST_GREATER;
			case CompareFunction::NotEqual:     state = BGFX_STATE_DEPTH_TEST_NOTEQUAL;
			case CompareFunction::GreaterEqual: state = BGFX_STATE_DEPTH_TEST_GEQUAL;
			case CompareFunction::Always:       state = BGFX_STATE_DEPTH_TEST_ALWAYS;
			default: state = BGFX_STATE_DEPTH_TEST_LEQUAL;
			}
		}

		constexpr BgfxCompareFunction() = default;
		constexpr operator uint64_t() const noexcept { return state; }
		uint64_t state{ 0 };
	};

	// Mapeamento de StencilOperation (XNA) para bgfx
	// O parâmetro 'shift' ajusta se a operação é para Fail, ZFail ou Pass
	struct BgfxStencilOperation {
		constexpr BgfxStencilOperation(StencilOperation op, uint32_t shift) {
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
			state = val << shift;
		}

		constexpr BgfxStencilOperation() = default;
		constexpr operator uint32_t() const noexcept { return state; }
		uint32_t state{ 0 };
	};

	struct BgfxDepthStencilState {
		constexpr BgfxDepthStencilState(DepthStencilState const& depth) {
			depthBuffer = 0 | BGFX_STATE_DEPTH_TEST_LESS;

			if (depth.DepthBufferEnable) {
				depthBuffer |= BgfxCompareFunction(depth.DepthBufferFunction);
				if (depth.DepthBufferWriteEnable) {
					depthBuffer |= BGFX_STATE_WRITE_Z;
				}
			}	

			// --- STENCIL SECTION ---
			if (depth.StencilEnable) {
				// Front Face
				uint32_t frontStencil = BGFX_STENCIL_FUNC_REF(depth.ReferenceStencil)
					| BGFX_STENCIL_FUNC_RMASK(depth.StencilMask)
					| (uint32_t)BgfxCompareFunction(depth.StencilFunction) // Reutiliza mapeamento de comparação
					| BgfxStencilOperation(depth.StencilFail, 0)             // SFail
					| BgfxStencilOperation(depth.StencilDepthBufferFail, 4)  // ZFail
					| BgfxStencilOperation(depth.StencilPass, 8);            // Pass

				// Back Face (CCW)
				uint32_t bStencil = BGFX_STENCIL_NONE;
				if (depth.TwoSidedStencilMode) {
					backStencil = BGFX_STENCIL_FUNC_REF(depth.ReferenceStencil)
						| BGFX_STENCIL_FUNC_RMASK(depth.StencilMask)
						| (uint32_t)BgfxCompareFunction(depth.CounterClockwiseStencilFunction)
						| BgfxStencilOperation(depth.CounterClockwiseStencilFail, 0)
						| BgfxStencilOperation(depth.CounterClockwiseStencilDepthBufferFail, 4)
						| BgfxStencilOperation(depth.CounterClockwiseStencilPass, 8);
				}				

				// Nota: O Write Mask é global para o stencil no bgfx
				// Talvez precisamos gerenciar o write mask via bgfx::setState se for complexo, 
				// mas geralmente setStencil deve resolver.
			}
		}

		constexpr BgfxDepthStencilState() = default;
		constexpr operator uint64_t() const noexcept { return depthBuffer; }
		
		uint64_t depthBuffer{ 0 };
		uint32_t frontStencil{ BGFX_STENCIL_NONE };
		uint32_t backStencil{ BGFX_STENCIL_NONE };
	};

	struct BgfxRasterizerState {
		constexpr BgfxRasterizerState(RasterizerState const& rasterizer) {
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

			scissorTestEnable = rasterizer.ScissorTestEnable;
			depthBias = rasterizer.DepthBias;
			slopeScaleDepthBias = rasterizer.SlopeScaleDepthBias;
		}

		constexpr BgfxRasterizerState() = default;
		constexpr operator uint64_t() const noexcept { return state; }

		uint64_t state{ 0 };
		bool scissorTestEnable{ false };

		//Não existe uma função no bgfx para aplicar o bias
		//1. aplicar o bias manualmente no Vertex Shader da passagem de sombra ou no Fragment Shader durante a comparação.
		float depthBias{ 0 };
		float slopeScaleDepthBias{ 0 };
	};
}

#endif