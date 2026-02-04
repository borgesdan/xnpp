#ifndef XNA_GRAPHICS_DISPLAYMODE_HPP
#define XNA_GRAPHICS_DISPLAYMODE_HPP

#include <memory>
#include <cstdint>
#include <vector>
#include <optional>
#include "Shared.hpp"
#include "../Rectangle.hpp"

namespace Xna {
	//Describes the display mode. 
	struct DisplayMode final {
	public:
		//Public Properties
		
		//Gets the aspect ratio used by the graphics device.
		constexpr float AspectRatio() const {
			if (height == 0 || width == 0)
				return 0;

			return static_cast<float>(width) / static_cast<float>(height);
		}
		//Gets a value indicating the surface format of the display mode.
		constexpr SurfaceFormat Format() const { return format; }
		//Gets a value indicating the screen height, in pixels. 
		constexpr int32_t Height() const { return height; }
		//Gets a value indicating the screen width, in pixels. 
		constexpr int32_t Width() const { return width; }
		//Returns the title safe area of the display.
		constexpr Rectangle TitleSafeArea() const { return { 0,0,width, height }; }

		constexpr bool operator==(const DisplayMode& other) const {
			return width == other.width
				&& height == other.height
				&& format == other.format;
		}

		constexpr operator std::optional<DisplayMode>() const {
			return std::make_optional<DisplayMode>(*this);
		}

	private:
		constexpr DisplayMode() = default;
		constexpr DisplayMode(int32_t width, int32_t height, SurfaceFormat format) : width(width), height(height), format(format) {}

		int32_t width{ 0 };
		int32_t height{ 0 };
		SurfaceFormat format{ SurfaceFormat::Unknown };	

		friend class DisplayModeCollection;
		friend class GraphicsAdapter;
		friend struct Platform;
	};

	
}

#endif