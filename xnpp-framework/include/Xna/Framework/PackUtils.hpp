#ifndef XNA_FRAMEWORK_PACKUTILS_HPP
#define XNA_FRAMEWORK_PACKUTILS_HPP

#include <cstdint>
#include "Xna/Internal/Export.hpp"

namespace Xna {
	//Internal class
	struct PackUtils final {

		static constexpr float UnpackUNorm(uint32_t bitmask, uint32_t value) {
			value &= bitmask;
			return static_cast<float>(value) / static_cast<float>(bitmask);
		}

		static constexpr float UnpackSNorm(uint32_t bitmask, uint32_t value) {
			const auto num1 = (bitmask + 1U) >> 1;

			const auto ivalue = static_cast<int32_t>(value);
			const auto inum1 = static_cast<int32_t>(num1);
			const auto ibitmask = static_cast<int32_t>(bitmask);

			if ((ivalue & inum1) != 0) {
				if ((ivalue & ibitmask) == inum1)
					return -1.0f;

				value |= ~bitmask;
			}
			else
				value &= bitmask;

			const auto num2 = static_cast<float>(bitmask >> 1);

			return static_cast<int32_t>(value) / num2;
		}

		XNPP_API static uint32_t PackUnsigned(float bitmask, float value);
		XNPP_API static uint32_t PackSigned(uint32_t bitmask, float value);
		XNPP_API static uint32_t PackUNorm(float bitmask, float value);
		XNPP_API static uint32_t PackSNorm(uint32_t bitmask, float value);
		XNPP_API static double ClampAndRound(float value, float min, float max);
	};
}

#endif