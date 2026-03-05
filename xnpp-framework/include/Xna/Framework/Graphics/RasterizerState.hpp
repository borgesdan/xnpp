#ifndef XNA_FRAMEWORK_RASTERIZERSTATE_HPP
#define XNA_FRAMEWORK_RASTERIZERSTATE_HPP

#include "Shared.hpp"
#include <optional>

namespace Xna {
	//Contains rasterizer state, which determines how to convert vector data (shapes) into raster data (pixels). 
	class RasterizerState {
	public:
		constexpr RasterizerState() = default;

		//Specifies the conditions for culling or removing triangles. The default value is CullMode.CounterClockwise. 
		Xna::CullMode CullMode{ Xna::CullMode::None };
		//The fill mode, which defines how a triangle is filled during rendering. The default is FillMode.Solid. 
		Xna::FillMode FillMode{ Xna::FillMode::Solid };
		//Enables or disables multisample antialiasing. The default is true. 
		bool MultiSampleAntiAlias{ true };
		//Sets or retrieves the depth bias for polygons, which is the amount of bias to apply to the depth of a primitive
		//to alleviate depth testing problems for primitives of similar depth. The default value is 0. 
		float DepthBias{ 0 };
		//Gets or sets a bias value that takes into account the slope of a polygon. This bias value is applied to coplanar primitives 
		// to reduce aliasing and other rendering artifacts caused by z-fighting. The default is 0. 
		float SlopeScaleDepthBias{ 0 };
		//Enables or disables scissor testing. The default is false. 
		bool ScissorTestEnable{ false };

		//A built-in state object with settings for not culling any primitives.
		static constexpr RasterizerState CullNone() { return RasterizerState(Xna::CullMode::None, Xna::FillMode::Solid); }
		//A built-in state object with settings for culling primitives with clockwise winding order.
		static constexpr RasterizerState CullClockwise() { return RasterizerState(Xna::CullMode::CullClockwiseFace, Xna::FillMode::Solid); }
		//A built-in state object with settings for culling primitives with counter-clockwise winding order.
		static constexpr RasterizerState CullCounterClockwise() { return RasterizerState(Xna::CullMode::CullCounterClockwiseFace, Xna::FillMode::Solid); }

		constexpr bool operator==(RasterizerState const& other) const noexcept = default;

		constexpr operator std::optional<RasterizerState>() const noexcept { return std::make_optional<RasterizerState>(*this); }
		constexpr operator std::optional<std::reference_wrapper<const RasterizerState>>() const noexcept { return std::cref(*this); }

	private:
		constexpr RasterizerState(Xna::CullMode cullMode, Xna::FillMode fillMode)
			: CullMode(cullMode), FillMode(fillMode) {
		}
	};
}

#endif