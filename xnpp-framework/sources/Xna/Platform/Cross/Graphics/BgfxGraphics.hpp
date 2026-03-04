#ifndef XNA_BGFX_GRAPHICS
#define XNA_BGFX_GRAPHICS

#include "Xna/Framework/Graphics/Texture2D.hpp"
#include "Xna/Framework/Graphics/BlendState.hpp"
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

	//Padrão do BlendState para Bgfx
	struct BgfxBlendState {
		static constexpr uint64_t Opaque = 0;
		static constexpr uint64_t AlphaBlend = BGFX_STATE_BLEND_FUNC_SEPARATE(BGFX_STATE_BLEND_ONE, BGFX_STATE_BLEND_INV_SRC_ALPHA, BGFX_STATE_BLEND_ONE, BGFX_STATE_BLEND_INV_SRC_ALPHA);
		static constexpr uint64_t NonPremultiplied = BGFX_STATE_BLEND_FUNC_SEPARATE(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA, BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);
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



	inline uint32_t BgfxConvertBlendState(BlendState const& blend) {
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

		return state;
	}
}

#endif