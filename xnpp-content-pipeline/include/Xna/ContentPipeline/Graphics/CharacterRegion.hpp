#ifndef XNA_CONTENTPIPELINE_GRAPHICS_CHARACTERREGION_HPP
#define XNA_CONTENTPIPELINE_GRAPHICS_CHARACTERREGION_HPP

#include <cstdint>

namespace Xna {
	struct CharacterRegion {
		uint32_t Start{0};
		uint32_t End{0};

		constexpr CharacterRegion() = default;
		constexpr CharacterRegion(uint32_t start, uint32_t end) : Start(start), End(end){}
	};
}

#endif