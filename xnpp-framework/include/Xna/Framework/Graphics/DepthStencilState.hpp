#ifndef XNA_FRAMEWORK_GRAPHICS_DEPTHSTENCILSTATE_HPP
#define XNA_FRAMEWORK_GRAPHICS_DEPTHSTENCILSTATE_HPP

#include "Shared.hpp"
#include "GraphicsResource.hpp"
#include <cstdint>
#include <memory>
#include "Xna/Platform/Platform.hpp"
#include "Xna/Internal/Export.hpp"

namespace Xna {


	//Contains depth-stencil state for the device. 
	class DepthStencilState final : public GraphicsResource {
	public:
		//Creates an instance of DepthStencilState with default values.
		inline DepthStencilState() { impl = std::make_shared<DepthStencilState::Implementation>(); }
	
		~DepthStencilState() override {}

		//A built-in state object with default settings for using a depth stencil buffer.
		XNPP_API static DepthStencilState Default();
		//A built-in state object with settings for enabling a read-only depth stencil buffer.
		XNPP_API static DepthStencilState DepthRead();
		//A built-in state object with settings for not using a depth stencil buffer.
		XNPP_API static DepthStencilState None();

		//Gets or sets the stencil operation to perform if the stencil test passes and the depth-buffer test fails for a counterclockwise triangle.
		//The default is StencilOperation.Keep. 
		inline void CounterClockwiseStencilDepthBufferFail(StencilOperation value) { impl->counterClockwiseStencilDepthBufferFail = value; }
		//Gets or sets the stencil operation to perform if the stencil test passes and the depth-buffer test fails for a counterclockwise triangle.
		//The default is StencilOperation.Keep. 
		inline StencilOperation CounterClockwiseStencilDepthBufferFail() const { return impl->counterClockwiseStencilDepthBufferFail; }
		//Gets or sets the stencil operation to perform if the stencil test fails for a counterclockwise triangle. 
		//The default is StencilOperation.Keep.
		inline void CounterClockwiseStencilFail(StencilOperation value) { impl->counterClockwiseStencilDepthBufferFail = value; }
		//Gets or sets the stencil operation to perform if the stencil test fails for a counterclockwise triangle. 
		//The default is StencilOperation.Keep.
		inline StencilOperation CounterClockwiseStencilFail() const { return impl->counterClockwiseStencilDepthBufferFail; }
		//Gets or sets the comparison function to use for counterclockwise stencil tests. The default is CompareFunction.Always. 
		inline void CounterClockwiseStencilFunction(CompareFunction value) { impl->counterClockwiseStencilFunction = value; }
		//Gets or sets the comparison function to use for counterclockwise stencil tests. The default is CompareFunction.Always. 
		inline CompareFunction CounterClockwiseStencilFunction() const { return impl->counterClockwiseStencilFunction; }
		//Gets or sets the stencil operation to perform if the stencil and depth-tests pass for a counterclockwise triangle. 
		//The default is StencilOperation.Keep. 
		inline void CounterClockwiseStencilPass(StencilOperation value) { impl->counterClockwiseStencilPass = value; }
		//Gets or sets the stencil operation to perform if the stencil and depth-tests pass for a counterclockwise triangle. 
		//The default is StencilOperation.Keep. 
		inline StencilOperation CounterClockwiseStencilPass() const { return impl->counterClockwiseStencilPass; }

		//Enables or disables depth buffering. The default is true.
		inline void DepthBufferEnable(bool value) { impl->depthBufferEnable = value; }
		//Enables or disables depth buffering. The default is true.
		inline bool DepthBufferEnable() const { return impl->depthBufferEnable; }
		//Gets or sets the comparison function for the depth-buffer test. The default is CompareFunction.LessEqual
		inline void DepthBufferFunction(CompareFunction value) { impl->depthBufferFunction = value; }
		//Gets or sets the comparison function for the depth-buffer test. The default is CompareFunction.LessEqual
		inline CompareFunction DepthBufferFunction() const { return impl->depthBufferFunction; }
		//Enables or disables writing to the depth buffer. The default is true. 
		inline void DepthBufferWriteEnable(bool value) { impl->depthBufferWriteEnable = value; }
		//Enables or disables writing to the depth buffer. The default is true. 
		inline bool DepthBufferWriteEnable() const { return impl->depthBufferWriteEnable; }

		//Specifies a reference value to use for the stencil test. The default is 0.
		inline int32_t ReferenceStencil() const { return impl->referenceStencil; }
		//Specifies a reference value to use for the stencil test. The default is 0.
		inline void ReferenceStencil(int32_t value) { impl->referenceStencil = value; }

		//Gets or sets stencil enabling. The default is false.
		inline void StencilEnable(bool value) { impl->stencilEnable = value; }
		//Gets or sets stencil enabling. The default is false. 
		inline bool StencilEnable() const { return impl->stencilEnable; }
		//Gets or sets the stencil operation to perform if the stencil test fails. The default is StencilOperation.Keep. 
		inline void StencilFail(StencilOperation value) { impl->stencilFail = value; }
		//Gets or sets the stencil operation to perform if the stencil test fails. The default is StencilOperation.Keep. 
		inline StencilOperation StencilFail() const { return impl->counterClockwiseStencilDepthBufferFail; }
		//Gets or sets the comparison function for the stencil test.The default is CompareFunction.Always.
		inline void StencilFunction(CompareFunction value) { impl->stencilFunction = value; }
		//Gets or sets the comparison function for the stencil test. The default is CompareFunction.Always. 
		inline CompareFunction StencilFunction() const { return impl->stencilFunction; }
		//Gets or sets the mask applied to the reference value and each stencil buffer entry to determine the significant bits for the stencil test.
		//The default mask is Int32.MaxValue. 
		inline void StencilMask(int32_t value) { impl->stencilMask = value; }
		//Gets or sets the mask applied to the reference value and each stencil buffer entry to determine the significant bits for the stencil test.
		//The default mask is Int32.MaxValue. 
		inline int32_t StencilMask() const { return impl->stencilMask; }
		//Gets or sets the write mask applied to values written into the stencil buffer. The default mask is Int32.MaxValue. 
		inline void StencilWriteMask(int32_t value) { impl->stencilWriteMask = value; }
		//Gets or sets the write mask applied to values written into the stencil buffer. The default mask is Int32.MaxValue. 
		inline int32_t StencilWriteMask() const { return impl->stencilWriteMask; }
		//Gets or sets the stencil operation to perform if the stencil test passes. The default is StencilOperation.Keep. 
		inline void StencilPass(StencilOperation value) { impl->stencilPass = value; }
		//Gets or sets the stencil operation to perform if the stencil test passes. The default is StencilOperation.Keep. 
		inline StencilOperation StencilPass() const { return impl->stencilPass; }
		//Gets or sets the stencil operation to perform if the stencil test passes and the depth-test fails. The default is StencilOperation.Keep. 
		inline void StencilDepthBufferFail(StencilOperation value) { impl->stencilDepthBufferFail = value; }
		//Gets or sets the stencil operation to perform if the stencil test passes and the depth-test fails. The default is StencilOperation.Keep. 
		inline StencilOperation StencilDepthBufferFail() const { return impl->stencilDepthBufferFail; }

		//Enables or disables two-sided stenciling. The default is false.
		inline bool TwoSidedStencilMode() const { return impl->twoSidedStencilMode; }
		//Enables or disables two-sided stenciling. The default is false.
		inline void TwoSidedStencilMode(bool value) { impl->twoSidedStencilMode = value; }

		inline DepthStencilState(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(DepthStencilState const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		inline void Apply(GraphicsDevice& graphicsDevice) { Platform::DepthStencilState_Apply(*this, graphicsDevice); }
	
		struct Implementation {
			StencilOperation counterClockwiseStencilDepthBufferFail{ StencilOperation::Keep };
			StencilOperation counterClockwiseStencilFail{ StencilOperation::Keep };
			StencilOperation counterClockwiseStencilPass{ StencilOperation::Keep };
			CompareFunction counterClockwiseStencilFunction{ CompareFunction::Always };

			bool depthBufferEnable{ true };
			bool depthBufferWriteEnable{ true };
			CompareFunction depthBufferFunction{ CompareFunction::LessEquals };

			bool stencilEnable{ false };
			StencilOperation stencilFail{ StencilOperation::Keep };
			StencilOperation stencilPass{ StencilOperation::Keep };
			StencilOperation stencilDepthBufferFail{ StencilOperation::Keep };
			CompareFunction stencilFunction{ CompareFunction::Always };

			int32_t stencilMask{ 255 };
			int32_t stencilWriteMask{ 255 };

			bool twoSidedStencilMode{true};
			int32_t referenceStencil{ 0 };

			PlatformImpl::DepthStencilStateImpl platformImpl;
		};
	
		std::shared_ptr<Implementation> impl;

		friend class GraphicsDevice;
		friend struct Platform;
	};
}

#endif