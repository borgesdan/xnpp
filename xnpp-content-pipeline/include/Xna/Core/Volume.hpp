#ifndef XNA_CORE_HPP_VOLUME
#define XNA_CORE_HPP_VOLUME

#include "Numeric.hpp"

namespace Xna::Core {	
	struct BoundingBox {
		Vector3 Min{};
		Vector3 Max{};
	};
}

#endif