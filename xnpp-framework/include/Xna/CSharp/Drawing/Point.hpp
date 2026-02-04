#ifndef CSHARP_DRAWING_POINT_HPP
#define CSHARP_DRAWING_POINT_HPP

#include <cstdint>

namespace Xna::CSharp {
	struct Point {
		constexpr Point() = default;

		constexpr Point(int32_t x, int32_t y)
			: X(x), Y(y) {
		}

		constexpr bool IsEmpty() const {
			return X == 0 && Y == 0;
		}

		int32_t X{ 0 };
		int32_t Y{ 0 };

		bool operator==(const Point& other) const = default;
	};
}

#endif