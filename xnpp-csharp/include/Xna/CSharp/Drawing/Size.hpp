#ifndef CSHARP_DRAWING_SIZE_HPP
#define CSHARP_DRAWING_SIZE_HPP

#include <cstdint>

namespace Xna::CSharp {
	struct Size {
		constexpr Size() = default;

		constexpr Size(int32_t width, int32_t height)
			: Width(width), Height(height) {
		}

		constexpr bool IsZero() const {
			return Width == 0 && Height == 0;
		}

		int32_t Width{ 0 };
		int32_t Height{ 0 };

		bool operator==(const Size& other) const = default;
	};
}

#endif