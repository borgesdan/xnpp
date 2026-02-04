#ifndef XNA_FRAMEWORK_CURVEKEY_HPP
#define XNA_FRAMEWORK_CURVEKEY_HPP

#include "Shared.hpp"

namespace Xna {
	//Represents a point in a multi-point curve.
	struct CurveKey final {
		//Initializes a new instance of CurveKey.
		constexpr CurveKey() = default;

		//Initializes a new instance of CurveKey.
		constexpr CurveKey(CurveKey const& value) :
			Position(value.Position), Value(value.Value), TangentIn(value.TangentIn), TangentOut(value.TangentOut), Continuity(value.Continuity) {
		}

		//Initializes a new instance of CurveKey.
		constexpr CurveKey(float position, float value, float tangentIn, float tangentOut, CurveContinuity continuity) :
			Position(position), Value(value), TangentIn(tangentIn), TangentOut(tangentOut), Continuity(continuity) {
		}

		//Position of the CurveKey in the curve.
		float Position{ 0 };
		//Describes the value of this point.
		float Value{ 0 };
		//Describes the tangent when leaving this point to the next point in the curve.
		float TangentOut{ 0 };
		//Describes the tangent when approaching this point from the previous point in the curve
		float TangentIn{ 0 };
		//Describes whether the segment between this point and the next point in the curve is discrete or continuous.
		CurveContinuity Continuity{ CurveContinuity::Smooth };

		constexpr bool operator==(const CurveKey& other) const {
			return Position == other.Position
				&& Value == other.Value
				&& TangentOut == other.TangentOut
				&& TangentIn == other.TangentIn
				&& Continuity == other.Continuity;
		}

		constexpr bool operator<(CurveKey const& other) const {
			return Position < other.Position;
		}
	};
}

#endif