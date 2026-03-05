#ifndef XNA_FRAMEWORK_GRAPHICS_DEPTHSTENCILSTATE_HPP
#define XNA_FRAMEWORK_GRAPHICS_DEPTHSTENCILSTATE_HPP

#include "Shared.hpp"
#include <cstdint>
#include <optional>

namespace Xna {

	//Contains depth-stencil state for the device. 
	struct DepthStencilState {
	public:
		//Creates an instance of DepthStencilState with default values.
		constexpr DepthStencilState() = default;

		//A built-in state object with default settings for using a depth stencil buffer.
		static const DepthStencilState& Default() { 
			static const auto state = DepthStencilState(false, false);
			return state;
		}

		//A built-in state object with settings for enabling a read-only depth stencil buffer.
		static const DepthStencilState& DepthRead() {
			static const auto state = DepthStencilState(true, true);
			return state;
		}

		//A built-in state object with settings for not using a depth stencil buffer.
		static const DepthStencilState& None() { 
			static const auto state = DepthStencilState(true, false);
			return state;
		}

		//Gets or sets the stencil operation to perform if the stencil test passes and the depth-buffer test fails for a counterclockwise triangle.
		//The default is StencilOperation.Keep. 
		StencilOperation CounterClockwiseStencilDepthBufferFail{ StencilOperation::Keep };
		//Gets or sets the stencil operation to perform if the stencil test fails for a counterclockwise triangle. 
		//The default is StencilOperation.Keep.
		StencilOperation CounterClockwiseStencilFail{ StencilOperation::Keep };
		//Gets or sets the comparison function to use for counterclockwise stencil tests. The default is CompareFunction.Always. 
		CompareFunction CounterClockwiseStencilFunction{ CompareFunction::Always };
		//Gets or sets the stencil operation to perform if the stencil and depth-tests pass for a counterclockwise triangle. 
		//The default is StencilOperation.Keep. 
		StencilOperation CounterClockwiseStencilPass{ StencilOperation::Keep };
		//Enables or disables depth buffering. The default is true.
		bool DepthBufferEnable{ true };
		//Gets or sets the comparison function for the depth-buffer test. The default is CompareFunction.LessEqual
		CompareFunction DepthBufferFunction{ CompareFunction::LessEqual };
		//Enables or disables writing to the depth buffer. The default is true. 
		bool DepthBufferWriteEnable{ true };
		//Specifies a reference value to use for the stencil test. The default is 0.
		int32_t ReferenceStencil{ 0 };
		//Gets or sets stencil enabling. The default is false.
		bool StencilEnable{ false };
		//Gets or sets the stencil operation to perform if the stencil test fails. The default is StencilOperation.Keep. 
		StencilOperation StencilFail{ StencilOperation::Keep };
		//Gets or sets the comparison function for the stencil test. The default is CompareFunction.Always. 
		CompareFunction StencilFunction{ CompareFunction::Always };
		//Gets or sets the mask applied to the reference value and each stencil buffer entry to determine the significant bits for the stencil test.
		//The default mask is Int32.MaxValue. 
		int32_t StencilMask { 0x7FFFFFFF };
		//Gets or sets the write mask applied to values written into the stencil buffer. The default mask is Int32.MaxValue. 
		int32_t StencilWriteMask{ 0x7FFFFFFF };
		//Gets or sets the stencil operation to perform if the stencil test passes. The default is StencilOperation.Keep. 
		StencilOperation StencilPass{ StencilOperation::Keep };
		//Gets or sets the stencil operation to perform if the stencil test passes and the depth-test fails. The default is StencilOperation.Keep. 
		StencilOperation StencilDepthBufferFail{ StencilOperation::Keep };
		//Enables or disables two-sided stenciling. The default is false.
		bool TwoSidedStencilMode{ true };

		constexpr operator std::optional<DepthStencilState>() const noexcept { return std::make_optional<DepthStencilState>(*this); }
		constexpr operator std::optional<std::reference_wrapper<const DepthStencilState>>() const noexcept { return std::cref(*this); }

	private:
		constexpr DepthStencilState(bool dbEnable, bool dbwEnable)
			:DepthBufferEnable(dbEnable), DepthBufferWriteEnable(dbwEnable) {
		}		
	};
}

#endif