#ifndef XNA_FRAMEWORK_RASTERIZERSTATE_HPP
#define XNA_FRAMEWORK_RASTERIZERSTATE_HPP

#include "Shared.hpp"
#include "GraphicsResource.hpp"
#include <memory>
#include "Xna/Platform/Platform.hpp"

namespace Xna {
	//Contains rasterizer state, which determines how to convert vector data (shapes) into raster data (pixels). 
	class RasterizerState final : public GraphicsResource {
	public:
		inline RasterizerState() { impl = std::make_shared<Implementation>(); }

		~RasterizerState() = default;

		//Specifies the conditions for culling or removing triangles. The default value is CullMode.CounterClockwise. 
		inline Xna::CullMode CullMode() const { return impl->cullMode; }
		//Specifies the conditions for culling or removing triangles. The default value is CullMode.CounterClockwise. 
		inline void CullMode(Xna::CullMode value) { impl->cullMode = value; }
		//The fill mode, which defines how a triangle is filled during rendering. The default is FillMode.Solid. 
		inline Xna::FillMode FillMode() const {return impl->fillMode;}
		//The fill mode, which defines how a triangle is filled during rendering. The default is FillMode.Solid. 
		inline void FillMode(Xna::FillMode value) { impl->fillMode = value; }

		//Enables or disables multisample antialiasing. The default is true. 
		inline bool MultiSampleAntiAlias() const{ return impl->multiSampleAntiAlias; }
		//Enables or disables multisample antialiasing. The default is true. 
		inline void MultiSampleAntiAlias(bool value) { impl->multiSampleAntiAlias = value; }

		//Sets or retrieves the depth bias for polygons, which is the amount of bias to apply to the depth of a primitive
		//to alleviate depth testing problems for primitives of similar depth. The default value is 0. 
		inline float DepthBias() const { return impl->depthBias; }
		//Sets or retrieves the depth bias for polygons, which is the amount of bias to apply to the depth of a primitive
		//to alleviate depth testing problems for primitives of similar depth. The default value is 0. 
		inline void DepthBias(float value) { impl->depthBias = value; }

		//Gets or sets a bias value that takes into account the slope of a polygon. This bias value is applied to coplanar primitives 
		// to reduce aliasing and other rendering artifacts caused by z-fighting. The default is 0. 
		inline float SlopeScaleDepthBias() const { impl->slopeScaleDepthBias; }
		//Gets or sets a bias value that takes into account the slope of a polygon. This bias value is applied to coplanar primitives 
		// to reduce aliasing and other rendering artifacts caused by z-fighting. The default is 0. 
		inline void SlopeScaleDepthBias(float value) { impl->slopeScaleDepthBias = value; }

		//Enables or disables scissor testing. The default is false. 
		inline bool ScissorTestEnable() const { return impl->scissorTestEnable; }
		//Enables or disables scissor testing. The default is false. 
		inline void ScissorTestEnable(bool value) { impl->scissorTestEnable = value; }

		//A built-in state object with settings for not culling any primitives.
		static RasterizerState CullNone();
		//A built-in state object with settings for culling primitives with clockwise winding order.
		static RasterizerState CullClockwise();
		//A built-in state object with settings for culling primitives with counter-clockwise winding order.
		static RasterizerState CullCounterClockwise();		

		inline RasterizerState(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(RasterizerState const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		struct Implementation {
			Xna::CullMode cullMode{ Xna::CullMode::None };
			Xna::FillMode fillMode{ Xna::FillMode::Solid};
			bool multiSampleAntiAlias{true};
			float depthBias{0};
			float slopeScaleDepthBias{0};
			bool scissorTestEnable{false};

			PlatformImpl::RasterizerStateImpl platformImpl;
		};
	
		inline void Apply(GraphicsDevice& device) { Platform::RasterizerState_Apply(*this, device); }
		std::shared_ptr<Implementation> impl;

		friend class GraphicsDevice;
		friend struct Platform;
	};
}

#endif