#ifndef XNA_FRAMEWORK_GRAPHICS_BLENDSTATE_HPP
#define XNA_FRAMEWORK_GRAPHICS_BLENDSTATE_HPP

#include <cstdint>
#include <optional>
#include "Shared.hpp"
#include "Xna/Framework/Color.hpp"

namespace Xna {
	//Contains blend state for the device. 
	struct BlendState {
	public:
		//Creates an instance of the BlendState class with default values, using additive color and alpha blending.
		constexpr BlendState() = default;

		//A built-in state object with settings for opaque blend,
		//that is overwriting the source with the destination data.
		static inline const BlendState& Opaque() {
			static const auto blend = BlendState(Blend::One, Blend::Zero, Blend::One, Blend::Zero);
			return blend;
		}
		//A built-in state object with settings for alpha blend, 
		//that is blending the source and destination data using alpha.
		static inline const BlendState& AlphaBlend() {
			static const auto blend = BlendState(Blend::One, Blend::InverseSourceAlpha, Blend::One, Blend::InverseSourceAlpha);
			return blend;
		}
		//A built-in state object with settings for additive blend, 
		//that is adding the destination data to the source data without using alpha.
		static inline const BlendState& Additive() {
			static const auto blend = BlendState(Blend::SourceAlpha, Blend::One, Blend::SourceAlpha, Blend::One);
			return blend;
		}
		//A built-in state object with settings for blending with non-premultipled alpha, 
		//that is blending source and destination data using alpha while assuming the color data contains no alpha information.
		static inline const BlendState& NonPremultiplied() {
			static const auto blend = BlendState(Blend::SourceAlpha, Blend::InverseSourceAlpha, Blend::SourceAlpha, Blend::InverseSourceAlpha);
			return blend;
		}

		//Gets or sets the arithmetic operation when blending alpha values. The default is BlendFunction.Add. 
		BlendFunction AlphaBlendFunction{ BlendFunction::Add };
		//Gets or sets the blend factor for the destination alpha, which is the percentage of the destination alpha included
		//in the blended result. The default is Blend.One. 
		Blend AlphaDestinationBlend{ Blend::One };
		//Gets or sets the alpha blend factor. The default is Blend.One. 
		Blend AlphaSourceBlend{ Blend::One };
		//Gets or sets the arithmetic operation when blending color values. The default is BlendFunction.Add. 
		BlendFunction ColorBlendFunction{ BlendFunction::Add };
		//Gets or sets the blend factor for the destination color. The default is Blend.One. 
		Blend ColorDestinationBlend{ Blend::One };
		//Gets or sets the blend factor for the source color. The default is Blend.One. 
		Blend ColorSourceBlend{ Blend::One };
		//Gets or sets which color channels (RGBA) are enabled for writing during color blending. 
		// The default value is ColorWriteChannels.All.
		Xna::ColorWriteChannels ColorWriteChannels{ ColorWriteChannels::All };
		//Gets or sets which color channels (RGBA) are enabled for writing during color blending. 
		// The default value is ColorWriteChannels.All.
		Xna::ColorWriteChannels ColorWriteChannels1{ ColorWriteChannels::All };
		//Gets or sets which color channels (RGBA) are enabled for writing during color blending. 
		// The default value is ColorWriteChannels.All.
		Xna::ColorWriteChannels ColorWriteChannels2{ ColorWriteChannels::All };
		//Gets or sets which color channels (RGBA) are enabled for writing during color blending. 
		// The default value is ColorWriteChannels.All.
		Xna::ColorWriteChannels ColorWriteChannels3{ ColorWriteChannels::All };
		//Gets or sets a bitmask which defines which samples can be written during multisampling. The default is 0xffffffff.
		int32_t MultiSampleMask{ static_cast<int32_t>(0xffffffff) };
		//Gets or sets the four-component (RGBA) blend factor for alpha blending.
		Color BlendFactor{ Color::White() };

		constexpr bool operator==(BlendState const& other) const noexcept = default;

		constexpr operator std::optional<BlendState>() const noexcept { return std::make_optional<BlendState>(*this); }
		constexpr operator std::optional<std::reference_wrapper<const BlendState>>() const noexcept { return std::cref(*this); }

		constexpr bool IsOpaque() const noexcept {
			return ColorSourceBlend == Blend::One
				&& ColorDestinationBlend == Blend::Zero
				&& AlphaSourceBlend == Blend::One
				&& AlphaDestinationBlend == Blend::Zero;
		}

		constexpr bool IsAlphaBlend() const noexcept {
			return ColorSourceBlend == Blend::One
				&& ColorDestinationBlend == Blend::InverseSourceAlpha
				&& AlphaSourceBlend == Blend::One
				&& AlphaDestinationBlend == Blend::InverseSourceAlpha;
		}

		constexpr bool IsAdditive() const noexcept {
			return ColorSourceBlend == Blend::SourceAlpha
				&& ColorDestinationBlend == Blend::One
				&& AlphaSourceBlend == Blend::SourceAlpha
				&& AlphaDestinationBlend == Blend::One;
		}

		constexpr bool IsNonPremultiplied() const noexcept {
			return ColorSourceBlend == Blend::SourceAlpha
				&& ColorDestinationBlend == Blend::InverseSourceAlpha
				&& AlphaSourceBlend == Blend::SourceAlpha
				&& AlphaDestinationBlend == Blend::InverseSourceAlpha;
		}

	private:
		constexpr BlendState(Blend colorSrcBlend, Blend colorDestBlend, Blend alphaSrcBlend, Blend alphaDestBlend)
			: ColorSourceBlend(colorSrcBlend), ColorDestinationBlend(colorDestBlend),
			AlphaSourceBlend(alphaSrcBlend), AlphaDestinationBlend(alphaDestBlend) {
		}
	};
}

#endif