#ifndef XNA_FRAMEWORK_POINT_HPP
#define XNA_FRAMEWORK_POINT_HPP

#include <cstdint>
#include <optional>

namespace Xna {
	//Defines a point in 2D space.
	struct Point final {
		//Specifies the x-coordinate of the point.
		int32_t X{ 0 };
		//Specifies the y - coordinate of the point.
		int32_t Y{ 0 };
		
		//Initializes a new instance of Point.
		constexpr Point() = default;
		//Initializes a new instance of Point.
		constexpr Point(const int32_t& X, const int32_t& Y)
			: X(X), Y(Y) {
		}
		
		//Returns the point (0,0).
		static constexpr Point Zero() { return {}; }

		constexpr bool operator==(const Point& other) const {
			return X == other.X && Y == other.Y;
		}

		constexpr operator std::optional<Point>() const {
			return std::make_optional<Point>(X, Y);
		}
	};
}

#endif