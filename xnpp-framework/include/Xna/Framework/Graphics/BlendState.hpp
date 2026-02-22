#ifndef XNA_FRAMEWORK_GRAPHICS_BLENDSTATE_HPP
#define XNA_FRAMEWORK_GRAPHICS_BLENDSTATE_HPP

#include <cstdint>
#include "Shared.hpp"
#include "Xna/Framework/Color.hpp"
#include "Xna/Platform/Platform.hpp"

namespace Xna {
	//Contains blend state for the device. 
	class BlendState {
	public:
		//Creates an instance of the BlendState class with default values, using additive color and alpha blending.
		constexpr BlendState() = default;

		//A built-in state object with settings for opaque blend,
		//that is overwriting the source with the destination data.
		static constexpr BlendState Opaque() {
			return BlendState(
				Blend::One,
				Blend::Zero,
				Blend::One,
				Blend::Zero);
		}
		//A built-in state object with settings for alpha blend, 
		//that is blending the source and destination data using alpha.
		static BlendState AlphaBlend() {
			return BlendState(
				Blend::One,
				Blend::InverseSourceAlpha,
				Blend::One,
				Blend::InverseSourceAlpha);
		}
		//A built-in state object with settings for additive blend, 
		//that is adding the destination data to the source data without using alpha.
		static BlendState Additive() {
			return BlendState(
				Blend::SourceAlpha,
				Blend::One,
				Blend::SourceAlpha,
				Blend::One);
		}
		//A built-in state object with settings for blending with non-premultipled alpha, 
		//that is blending source and destination data using alpha while assuming the color data contains no alpha information.
		static BlendState NonPremultiplied() {
			return BlendState(
				Blend::SourceAlpha,
				Blend::InverseSourceAlpha,
				Blend::SourceAlpha,
				Blend::InverseSourceAlpha);
		}

		//Gets or sets the arithmetic operation when blending alpha values. The default is BlendFunction.Add. 
		constexpr BlendOperation AlphaBlendFunction() const { return alphaBlendFunction; }
		//Gets or sets the arithmetic operation when blending alpha values. The default is BlendFunction.Add. 
		constexpr void AlphaBlendFunction(BlendOperation value) { alphaBlendFunction = value; }
		//Gets or sets the blend factor for the destination alpha, which is the percentage of the destination alpha included in the blended result. The default is Blend.One. 
		constexpr Blend AlphaDestinationBlend() const { return alphaDestinationBlend; }
		//Gets or sets the blend factor for the destination alpha, which is the percentage of the destination alpha included in the blended result. The default is Blend.One. 
		constexpr void AlphaDestinationBlend(Blend value) { alphaDestinationBlend = value; }
		//Gets or sets the alpha blend factor. The default is Blend.One. 
		constexpr Blend AlphaSourceBlend() const { return alphaSourceBlend; }
		//Gets or sets the alpha blend factor. The default is Blend.One. 
		constexpr void AlphaSourceBlend(Blend value) { alphaSourceBlend = value; }
		//Gets or sets the arithmetic operation when blending color values. The default is BlendFunction.Add. 
		constexpr BlendOperation ColorBlendFunction() const { return colorBlendFunction; }
		//Gets or sets the arithmetic operation when blending color values. The default is BlendFunction.Add. 
		constexpr void ColorBlendFunction(BlendOperation value) {  colorBlendFunction = value;  }
		//Gets or sets the blend factor for the destination color. The default is Blend.One. 
		constexpr Blend ColorDestinationBlend() const { return colorDestinationBlend; }
		//Gets or sets the blend factor for the destination color. The default is Blend.One. 
		constexpr void ColorDestinationBlend(Blend value) { colorDestinationBlend = value; }
		//Gets or sets the blend factor for the source color. The default is Blend.One. 
		constexpr Blend ColorSourceBlend() const { return colorSourceBlend; }
		//Gets or sets the blend factor for the source color. The default is Blend.One. 
		constexpr void ColorSourceBlend(Blend value) { colorSourceBlend = value; }
		//Gets or sets which color channels (RGBA) are enabled for writing during color blending. 
		// The default value is ColorWriteChannels.None.
		constexpr Xna::ColorWriteChannels ColorWriteChannels() const { return colorWriteChannels0; }
		//Gets or sets which color channels (RGBA) are enabled for writing during color blending. 
		// The default value is ColorWriteChannels.None.
		constexpr void ColorWriteChannels(Xna::ColorWriteChannels const& value) { colorWriteChannels0 = value; }
		//Gets or sets which color channels (RGBA) are enabled for writing during color blending. 
		// The default value is ColorWriteChannels.None.
		constexpr Xna::ColorWriteChannels ColorWriteChannels1() const { return colorWriteChannels1; }
		//Gets or sets which color channels (RGBA) are enabled for writing during color blending. 
		// The default value is ColorWriteChannels.None.
		constexpr void ColorWriteChannels1(Xna::ColorWriteChannels const& value) { colorWriteChannels1 = value; }
		//Gets or sets which color channels (RGBA) are enabled for writing during color blending. 
		// The default value is ColorWriteChannels.None.
		constexpr Xna::ColorWriteChannels ColorWriteChannels2() const { return colorWriteChannels2; }
		//Gets or sets which color channels (RGBA) are enabled for writing during color blending. 
		// The default value is ColorWriteChannels.None.
		constexpr void ColorWriteChannels2(Xna::ColorWriteChannels const& value) { colorWriteChannels2 = value; }
		//Gets or sets which color channels (RGBA) are enabled for writing during color blending. 
		// The default value is ColorWriteChannels.None.
		constexpr Xna::ColorWriteChannels ColorWriteChannels3() const { return colorWriteChannels3; }
		//Gets or sets which color channels (RGBA) are enabled for writing during color blending. 
		// The default value is ColorWriteChannels.None.
		constexpr void ColorWriteChannels3(Xna::ColorWriteChannels const& value) { colorWriteChannels3 = value; }
		//Gets or sets a bitmask which defines which samples can be written during multisampling. The default is 0xffffffff.
		constexpr int32_t MultiSampleMask() const { return multiSampleMask; }
		//Gets or sets a bitmask which defines which samples can be written during multisampling. The default is 0xffffffff.
		constexpr void MultiSampleMask(int32_t value) { multiSampleMask = value; }
		//Gets or sets the four-component (RGBA) blend factor for alpha blending.
		constexpr Color BlendFactor() const { return blendFactor; }
		//Gets or sets the four - component(RGBA) blend factor for alpha blending.
		constexpr void BlendFactor(Color const& value) { blendFactor = value; }		

		constexpr bool operator==(BlendState const& other) const noexcept = default;		

		constexpr bool IsOpaque() const noexcept {
			return colorSourceBlend == Blend::One
				&& colorDestinationBlend == Blend::Zero
				&& alphaSourceBlend == Blend::One
				&& alphaDestinationBlend == Blend::Zero;
		}

		constexpr bool IsAlphaBlend() const noexcept {
			return colorSourceBlend == Blend::One
				&& colorDestinationBlend == Blend::InverseSourceAlpha
				&& alphaSourceBlend == Blend::One
				&& alphaDestinationBlend == Blend::InverseSourceAlpha;
		}

		constexpr bool IsAdditive() const noexcept {
			return colorSourceBlend == Blend::SourceAlpha
				&& colorDestinationBlend == Blend::One
				&& alphaSourceBlend == Blend::SourceAlpha
				&& alphaDestinationBlend == Blend::One;
		}

		constexpr bool IsNonPremultiplied() const noexcept {
			return colorSourceBlend == Blend::SourceAlpha
				&& colorDestinationBlend == Blend::InverseSourceAlpha
				&& alphaSourceBlend == Blend::SourceAlpha
				&& alphaDestinationBlend == Blend::InverseSourceAlpha;
		}		

	private:
		constexpr BlendState(
			Blend colorSrcBlend,
			Blend colorDestBlend,
			Blend alphaSrcBlend,
			Blend alphaDestBlend) {

			colorSourceBlend = colorSrcBlend;
			colorDestinationBlend = colorDestBlend;
			alphaSourceBlend = alphaSrcBlend;
			alphaDestinationBlend = alphaDestBlend;
		}

		inline void Apply(GraphicsDevice& graphicsDevice) { 
			PlatformNS::GraphicsDevice_Apply_BlendState(graphicsDevice, *this);
		}

		BlendOperation alphaBlendFunction{ BlendOperation::Add };
		Blend alphaDestinationBlend{ Blend::One };
		Blend alphaSourceBlend{ Blend::One };
		BlendOperation colorBlendFunction{ BlendOperation::Add };
		Blend colorDestinationBlend{ Blend::One };
		Blend colorSourceBlend{ Blend::One };
		int32_t multiSampleMask{ 0xffffffff };
		Color blendFactor{ Colors::White };
		Xna::ColorWriteChannels colorWriteChannels0{ ColorWriteChannels::None };
		Xna::ColorWriteChannels colorWriteChannels1{ ColorWriteChannels::None };
		Xna::ColorWriteChannels colorWriteChannels2{ ColorWriteChannels::None };
		Xna::ColorWriteChannels colorWriteChannels3{ ColorWriteChannels::None };		

		friend class GraphicsDevice;
		friend struct Platform;
	};
}

#endif