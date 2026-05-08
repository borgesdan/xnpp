#ifndef XNA_CORE_GRAPHICS_HPP
#define XNA_CORE_GRAPHICS_HPP

namespace Xna::Core {
	struct Color {
		constexpr uint32_t PackedValue() const {
			return _packedValue;
		}

		uint32_t _packedValue{ 0 };
	};
}

#endif