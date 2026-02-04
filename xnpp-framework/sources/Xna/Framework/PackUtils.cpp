#include "Xna/Framework/PackUtils.hpp"

#include <cmath>

namespace Xna {
	uint32_t PackUtils::PackUnsigned(float bitmask, float value) {
		return static_cast<uint32_t>(ClampAndRound(value, 0.0f, bitmask));
	}

	uint32_t PackUtils::PackSigned(uint32_t bitmask, float value) {
		const auto max = static_cast<float>(bitmask >> 1);
		const auto min = -max - 1.0F;

		return static_cast<uint32_t>(ClampAndRound(value, min, max)) & bitmask;
	}

	uint32_t PackUtils::PackUNorm(float bitmask, float value) {
		value *= bitmask;
		return static_cast<uint32_t>(ClampAndRound(value, 0.0f, bitmask));
	}

	uint32_t PackUtils::PackSNorm(uint32_t bitmask, float value) {
		const auto max = static_cast<float>(bitmask >> 1);
		value *= max;
		return static_cast<uint32_t>(ClampAndRound(value, -max, max)) & bitmask;
	}

	double PackUtils::ClampAndRound(float value, float min, float max) {
		if (isnan(value))
			return 0.0;

		if (isinf(value))
			return value < 0 ? static_cast<double>(min) : static_cast<double>(max);

		if (value < min)
			return static_cast<double>(min);

		return value > max ? static_cast<double>(max) : std::round(static_cast<double>(value));
	}
}