#ifndef XNA_BGFX_GRAPHICS
#define XNA_BGFX_GRAPHICS

#include "Xna/Framework/Graphics/Texture2D.hpp"
#include "Xna/Framework/Graphics/BlendState.hpp"
#include "Xna/Framework/Graphics/SamplerState.hpp"
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
		constexpr BgfxTextureAddressMode(SamplerState const& s) {
			switch (s.AddressU)
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
			case TextureAddressMode::MirrorOnce:
				//TODO: Não possui implementação
				break;
			default:
				break;
			}

			switch (s.AddressV)
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
			case TextureAddressMode::MirrorOnce:
				//TODO: Não possui implementação
				break;
			default:
				break;
			}

			switch (s.AddressW)
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
			case TextureAddressMode::MirrorOnce:
				//TODO: Não possui implementação
				break;
			default:
				break;
			}
		}		

		constexpr BgfxTextureAddressMode() = default;
		constexpr operator uint64_t() const noexcept { return state; }
		uint64_t state{ 0 };
	};

	struct BgfxTextureFilter {
		constexpr BgfxTextureFilter(TextureFilter const& f) {
			switch (f) {
			case TextureFilter::Point:
				state |= BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_MIP_POINT;
			case TextureFilter::Anisotropic:
				// Nota: No bgfx, o filtro anisotrópico geralmente assume linear como base
				state |= BGFX_SAMPLER_MIN_ANISOTROPIC | BGFX_SAMPLER_MAG_ANISOTROPIC;

			case TextureFilter::LinearMipPoint:
				// Min/Mag são lineares por padrão, forçamos apenas o Mip como Point
				state |= BGFX_SAMPLER_MIP_POINT;

			case TextureFilter::PointMipLinear:
				// Mip é linear por padrão, forçamos Min/Mag como Point
				state |= BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT;

			case TextureFilter::MinLinearMagPointMipLinear:
				state |= BGFX_SAMPLER_MAG_POINT;

			case TextureFilter::MinLinearMagPointMipPoint:
				state |= BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_MIP_POINT;

			case TextureFilter::MinPointMagLinearMipLinear:
				state |= BGFX_SAMPLER_MIN_POINT;

			case TextureFilter::MinPointMagLinearMipPoint:
				state |= BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MIP_POINT;

			case TextureFilter::Linear:
			default:
				state |= 0;
			}
		}

		constexpr BgfxTextureFilter() = default;
		constexpr operator uint64_t() const noexcept { return state; }
		uint64_t state{ 0 };
	};

	struct BgfxSamplerState {
		constexpr BgfxSamplerState(SamplerState const& s) {		
			uint32_t state = 0;
			state |= BgfxTextureAddressMode(s);
			state |= BgfxTextureFilter(s.Filter);
		}

		constexpr BgfxSamplerState() = default;
		constexpr operator uint64_t() const noexcept { return state; }
		uint64_t state{ 0 };
	};
}

#endif