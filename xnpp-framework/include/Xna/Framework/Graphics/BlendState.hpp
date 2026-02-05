#ifndef XNA_FRAMEWORK_GRAPHICS_BLENDSTATE_HPP
#define XNA_FRAMEWORK_GRAPHICS_BLENDSTATE_HPP

#include <memory>
#include <cstdint>
#include "Shared.hpp"
#include "GraphicsResource.hpp"
#include "Xna/Framework/Color.hpp"
#include "Xna/Platform/Platform.hpp"
#include "Xna/Internal/Export.hpp"

namespace Xna {
	//Contains blend state for the device. 
	class BlendState final : public GraphicsResource {
	public:
		//Creates an instance of the BlendState class with default values, using additive color and alpha blending.
		inline BlendState() { impl = std::make_shared<BlendState::Implementation>(); }

		virtual ~BlendState() override {}

		//A built-in state object with settings for opaque blend,
		//that is overwriting the source with the destination data.
		XNPP_API static BlendState Opaque();
		//A built-in state object with settings for alpha blend, 
		//that is blending the source and destination data using alpha.
		XNPP_API static BlendState AlphaBlend();
		//A built-in state object with settings for additive blend, 
		//that is adding the destination data to the source data without using alpha.
		XNPP_API static BlendState Additive();
		//A built-in state object with settings for blending with non-premultipled alpha, 
		//that is blending source and destination data using alpha while assuming the color data contains no alpha information.
		XNPP_API static BlendState NonPremultiplied();

		//Gets or sets the arithmetic operation when blending alpha values. The default is BlendFunction.Add. 
		inline BlendOperation AlphaBlendFunction() const { return impl->alphaBlendFunction; }
		//Gets or sets the arithmetic operation when blending alpha values. The default is BlendFunction.Add. 
		inline void AlphaBlendFunction(BlendOperation value) { impl->alphaBlendFunction = value; }
		//Gets or sets the blend factor for the destination alpha, which is the percentage of the destination alpha included in the blended result. The default is Blend.One. 
		inline Blend AlphaDestinationBlend() const { return impl->alphaDestinationBlend; }
		//Gets or sets the blend factor for the destination alpha, which is the percentage of the destination alpha included in the blended result. The default is Blend.One. 
		inline void AlphaDestinationBlend(Blend value) { impl->alphaDestinationBlend = value; }
		//Gets or sets the alpha blend factor. The default is Blend.One. 
		inline Blend AlphaSourceBlend() const { return impl->alphaSourceBlend; }
		//Gets or sets the alpha blend factor. The default is Blend.One. 
		inline void AlphaSourceBlend(Blend value) { impl->alphaSourceBlend = value; }
		//Gets or sets the arithmetic operation when blending color values. The default is BlendFunction.Add. 
		inline BlendOperation ColorBlendFunction() const { return impl->colorBlendFunction; }
		//Gets or sets the arithmetic operation when blending color values. The default is BlendFunction.Add. 
		inline void ColorBlendFunction(BlendOperation value) {  impl->colorBlendFunction = value;  }
		//Gets or sets the blend factor for the destination color. The default is Blend.One. 
		inline Blend ColorDestinationBlend() const { return impl->colorDestinationBlend; }
		//Gets or sets the blend factor for the destination color. The default is Blend.One. 
		inline void ColorDestinationBlend(Blend value) { impl->colorDestinationBlend = value; }
		//Gets or sets the blend factor for the source color. The default is Blend.One. 
		inline Blend ColorSourceBlend() const { return impl->colorSourceBlend; }
		//Gets or sets the blend factor for the source color. The default is Blend.One. 
		inline void ColorSourceBlend(Blend value) { impl->colorSourceBlend = value; }
		//Gets or sets which color channels (RGBA) are enabled for writing during color blending. 
		// The default value is ColorWriteChannels.None.
		inline Xna::ColorWriteChannels ColorWriteChannels() const { return impl->colorWriteChannels0; }
		//Gets or sets which color channels (RGBA) are enabled for writing during color blending. 
		// The default value is ColorWriteChannels.None.
		inline void ColorWriteChannels(Xna::ColorWriteChannels const& value) { impl->colorWriteChannels0 = value; }
		//Gets or sets which color channels (RGBA) are enabled for writing during color blending. 
		// The default value is ColorWriteChannels.None.
		inline Xna::ColorWriteChannels ColorWriteChannels1() const { return impl->colorWriteChannels1; }
		//Gets or sets which color channels (RGBA) are enabled for writing during color blending. 
		// The default value is ColorWriteChannels.None.
		inline void ColorWriteChannels1(Xna::ColorWriteChannels const& value) { impl->colorWriteChannels1 = value; }
		//Gets or sets which color channels (RGBA) are enabled for writing during color blending. 
		// The default value is ColorWriteChannels.None.
		inline Xna::ColorWriteChannels ColorWriteChannels2() const { return impl->colorWriteChannels2; }
		//Gets or sets which color channels (RGBA) are enabled for writing during color blending. 
		// The default value is ColorWriteChannels.None.
		inline void ColorWriteChannels2(Xna::ColorWriteChannels const& value) { impl->colorWriteChannels2 = value; }
		//Gets or sets which color channels (RGBA) are enabled for writing during color blending. 
		// The default value is ColorWriteChannels.None.
		inline Xna::ColorWriteChannels ColorWriteChannels3() const { return impl->colorWriteChannels3; }
		//Gets or sets which color channels (RGBA) are enabled for writing during color blending. 
		// The default value is ColorWriteChannels.None.
		inline void ColorWriteChannels3(Xna::ColorWriteChannels const& value) { impl->colorWriteChannels3 = value; }
		//Gets or sets a bitmask which defines which samples can be written during multisampling. The default is 0xffffffff.
		inline int32_t MultiSampleMask() const { return impl->multiSampleMask; }
		//Gets or sets a bitmask which defines which samples can be written during multisampling. The default is 0xffffffff.
		inline void MultiSampleMask(int32_t value) { impl->multiSampleMask = value; }
		//Gets or sets the four-component (RGBA) blend factor for alpha blending.
		inline Color BlendFactor() const { return impl->blendFactor; }
		//Gets or sets the four - component(RGBA) blend factor for alpha blending.
		inline void BlendFactor(Color const& value) { impl->blendFactor = value; }

		inline BlendState(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(BlendState const& other) const noexcept { return impl == other.impl;	}
		inline bool operator==(std::nullptr_t) const noexcept {	return impl == nullptr;	}
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		inline void Apply(GraphicsDevice& graphicsDevice) { Platform::BlendState_Apply(*this, graphicsDevice); }
	
		struct Implementation {
			BlendOperation alphaBlendFunction{ BlendOperation::Add };
			Blend alphaDestinationBlend{ Blend::One };
			Blend alphaSourceBlend{ Blend::One };
			BlendOperation colorBlendFunction{ BlendOperation::Add };
			Blend colorDestinationBlend{ Blend::One };
			Blend colorSourceBlend{ Blend::One };
			int32_t multiSampleMask{ -1 };
			Color blendFactor{ Colors::White };
			Xna::ColorWriteChannels colorWriteChannels0{ ColorWriteChannels::None };
			Xna::ColorWriteChannels colorWriteChannels1{ ColorWriteChannels::None };
			Xna::ColorWriteChannels colorWriteChannels2{ ColorWriteChannels::None };
			Xna::ColorWriteChannels colorWriteChannels3{ ColorWriteChannels::None };

			PlatformImpl::BlendStateImpl platformImpl;
		};
	
		std::shared_ptr<Implementation> impl;
	
		friend class GraphicsDevice;
		friend struct Platform;
	};
}

#endif