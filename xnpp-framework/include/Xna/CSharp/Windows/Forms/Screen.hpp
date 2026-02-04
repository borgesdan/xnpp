#ifndef CSHARP_WINDOWS_FORMS_SCREEN_HPP
#define CSHARP_WINDOWS_FORMS_SCREEN_HPP

#include <string>
#include <cstdint>
#include <vector>
#include <memory>
#include <optional>
#include "../../Drawing/Rectangle.hpp"

namespace Xna::CSharp {
	class Screen {
	public:		
		Screen() = default;
		Screen(intptr_t monitor) : Screen(monitor, 0) {}
		Screen(intptr_t monitor, intptr_t hdc);

		static std::vector<Screen> AllScreens();
		static Screen PrimaryScreen();
		static int32_t DesktopChangedCount();

		constexpr int32_t BitsPerPixel() const {
			return _bitDepth;
		}

		constexpr Rectangle Bounds() const {
			return _bounds;
		}

		constexpr std::string DeviceName() const {
			return _deviceName;
		}

		constexpr bool Primary() const {
			return _primary;
		}

		Rectangle WorkingArea();

		constexpr bool operator==(Screen const& other) const {
			return _hmonitor == other._hmonitor;
		}

		static Screen FromPoint(Point const& point);
		static Screen FromRectangle(Rectangle const& rectangle);
		static Screen FromHandle(intptr_t hwnd);

		static Rectangle GetWorkingArea(Point const pt) {
			return FromPoint(pt).WorkingArea();
		}

		static Rectangle GetWorkingArea(Rectangle const& rectangle) {
			return FromRectangle(rectangle).WorkingArea();
		}

		static Rectangle GetBounds(Point const pt) {
			return FromPoint(pt).Bounds();
		}

		static Rectangle GetBounds(Rectangle const& rectangle) {
			return FromRectangle(rectangle).Bounds();
		}

	private:
		static int s_primaryMonitor;

	private:
		intptr_t _hmonitor{ 0 };
		Rectangle _bounds{};
		Rectangle _workingArea{};
		bool _primary{ false };
		std::string _deviceName;
		int32_t _bitDepth{ 0 };
		int32_t _currentDesktopChangedCount{ -1 };

		inline static int32_t s_desktopChangedCount = -1;

	};
}

#endif