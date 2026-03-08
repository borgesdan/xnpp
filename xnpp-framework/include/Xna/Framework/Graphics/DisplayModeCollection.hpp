#ifndef XNA_GRAPHICS_DISPLAYMODECOLLECTION_HPP
#define XNA_GRAPHICS_DISPLAYMODECOLLECTION_HPP

#include "DisplayMode.hpp"
#include <memory>
#include <vector>
#include <optional>
#include "Xna/Internal/Export.hpp"

namespace Xna {
	//Manipulates a collection of DisplayMode structures. 
	struct DisplayModeCollection {	
		XNPP_API DisplayMode& operator[](size_t index);
		XNPP_API std::vector<DisplayMode> operator[](SurfaceFormat surfaceFormat);

		constexpr auto begin() { return displayModes.begin(); }
		constexpr auto end() { return displayModes.end(); }
		constexpr auto begin() const { return displayModes.begin(); }
		constexpr auto end() const { return displayModes.end(); }

		constexpr DisplayModeCollection() = default;		
		inline DisplayModeCollection(std::vector<DisplayMode> const& displayModes) : displayModes(displayModes) {}
		inline DisplayModeCollection(std::vector<DisplayMode>& displayModes) : displayModes(std::move(displayModes)) {}

	private:
		std::vector<DisplayMode> displayModes;

		constexpr size_t Count() const noexcept { return displayModes.size(); }

		friend struct Platform;
		friend class GraphicsAdapter;
	};
}

#endif