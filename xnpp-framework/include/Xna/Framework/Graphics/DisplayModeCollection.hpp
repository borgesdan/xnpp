#ifndef XNA_GRAPHICS_DISPLAYMODECOLLECTION_HPP
#define XNA_GRAPHICS_DISPLAYMODECOLLECTION_HPP

#include "DisplayMode.hpp"
#include <memory>
#include <vector>
#include <optional>


namespace Xna {
	//Manipulates a collection of DisplayMode structures. 
	struct DisplayModeCollection {	
		DisplayMode& operator[](size_t index);
		std::vector<std::optional<DisplayMode>> operator[](SurfaceFormat surfaceFormat);

		constexpr auto begin() { return displayModes.begin(); }
		constexpr auto end() { return displayModes.end(); }
		constexpr auto begin() const { return displayModes.begin(); }
		constexpr auto end() const { return displayModes.end(); }

	private:
		constexpr DisplayModeCollection() = default;
		inline DisplayModeCollection(size_t count) : displayModes(count) {}
		inline DisplayModeCollection(std::vector<std::optional<DisplayMode>> const& displayModes) : displayModes(displayModes) {}

		std::vector<std::optional<DisplayMode>> displayModes;

		constexpr size_t Count() const noexcept { return displayModes.size(); }

		friend struct Platform;
		friend class GraphicsAdapter;
	};
}

#endif