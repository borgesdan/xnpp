#ifndef XNA_FRAMEWORK_GRAPHICS_PRESENTATIOPARAMETERS_HPP
#define XNA_FRAMEWORK_GRAPHICS_PRESENTATIOPARAMETERS_HPP

#include <cstdint>
#include "Shared.hpp"

namespace Xna {
	//Contains presentation parameters. 
	struct PresentationParameters {
		//Gets or sets a value indicating the width of the new swap chain's back buffer.
		int32_t BackBufferWidth{ 0 };
		//Gets or sets a value indicating the height of the new swap chain's back buffer.
		int32_t BackBufferHeight{ 0 };
		//Gets or sets the format of the back buffer.
		SurfaceFormat BackBufferFormat{ SurfaceFormat::Color };
		//Gets or sets the handle to the device window. 
		intptr_t DeviceWindowHandle{ 0 };
		//Gets or sets a value indicating whether the application is in full screen mode.
		bool IsFullScreen{ false };
		//Gets or sets a value indicating the number of sample locations during multisampling.
		int32_t MultiSampleCount{ 0 };
		//Gets or sets the maximum rate at which the swap chain's back buffers can be presented to the front buffer.
		PresentInterval PresentationInterval{ PresentInterval::Default };
		//Gets or sets the depth stencil data format.
		DepthFormat DepthStencilFormat{ DepthFormat::None };

		constexpr bool operator==(const PresentationParameters& other) const {
			return BackBufferWidth == other.BackBufferWidth 
				&& BackBufferHeight == other.BackBufferHeight 
				&& BackBufferFormat == other.BackBufferFormat 
				&& DeviceWindowHandle == other.DeviceWindowHandle 
				&& IsFullScreen == other.IsFullScreen 
				&& MultiSampleCount == other.MultiSampleCount 
				&& PresentationInterval == other.PresentationInterval 
				&& DepthStencilFormat == other.DepthStencilFormat;
		}
	};
}

#endif