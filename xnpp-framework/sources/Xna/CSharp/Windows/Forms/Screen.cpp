#include "Xna/CSharp/Windows/Forms/Screen.hpp"
#include "Xna/CSharp/Windows/Forms/SystemInformation.hpp"
#include "Xna/Platform/Platform.hpp"

namespace Xna::CSharp {
	Screen::Screen(intptr_t monitor, intptr_t hdc) {
		if (!SystemInformation::MultiMonitorSupport() || monitor == s_primaryMonitor) {
			_bounds = SystemInformation::VirtualScreen();
			_primary = true;
			_deviceName = Platform::System_MonitorDeviceName(monitor);
		}
		else {
			auto info = Platform::System_MonitorArea(monitor);
			_bounds = Rectangle::FromLTRB(info.Left, info.Top, info.Right, info.Bottom);
			_primary = Platform::System_MonitorIsPrimary(monitor);			
		}

		_hmonitor = monitor;
		_bitDepth = Platform::System_MonitorBitDepth(monitor, hdc);		
	}	

	std::vector<Screen> Screen::AllScreens() {
		std::vector<Screen> s_screens;
		
		if (SystemInformation::MultiMonitorSupport()) {			
			auto handlers = Platform::System_GetAllMonitorHandlers();
			
			if (!handlers.empty()) {
				s_screens.reserve(handlers.size());

				for (auto& hnd : handlers) {
					auto screen = Screen(
						get<0>(hnd),
						get<1>(hnd));

					s_screens.push_back(screen);
				}

				return s_screens;
			}
		}
		
		auto primary = Screen(s_primaryMonitor);
		s_screens.push_back(primary);

		return s_screens;
	}

	Screen Screen::PrimaryScreen() {
		auto screen = Screen(s_primaryMonitor, 0);

		if (SystemInformation::MultiMonitorSupport())
		{
			auto screens = AllScreens();
			for (size_t i = 0; i < screens.size(); ++i)
			{
				if (screens[i]._primary)
					screen = screens[i];
			}			
		}
		
		return screen;
	}

	Rectangle Screen::WorkingArea() {
		if (_currentDesktopChangedCount != DesktopChangedCount()) {
			if (!SystemInformation::MultiMonitorSupport() || _hmonitor == s_primaryMonitor) {
				_workingArea = SystemInformation::WorkingArea();
			}
			else {
				auto monitor = Platform::System_MonitorWorkingArea(_hmonitor);
				_workingArea = Rectangle::FromLTRB(monitor.Left, monitor.Top, monitor.Right, monitor.Bottom);
			}
		}

		return _workingArea;
	}

	int32_t Screen::DesktopChangedCount() {
		if (s_desktopChangedCount == -1) {
			s_desktopChangedCount = 0;
		}

		return s_desktopChangedCount;
	}

	Screen Screen::FromPoint(Point const& point) {
		if (SystemInformation::MultiMonitorSupport())
		{			
			auto i_monitor = Platform::System_MonitorFromPoint(point.X, point.Y);
			return Screen(i_monitor);
		}

		return Screen(s_primaryMonitor);
	}

	Screen Screen::FromRectangle(Rectangle const& rectangle) {
		if (SystemInformation::MultiMonitorSupport())
		{			
			const auto i_monitor = Platform::System_MonitorFromRect(
				rectangle.Left(),
				rectangle.Top(),
				rectangle.Right(),
				rectangle.Bottom()
			);
			return Screen(i_monitor);
		}

		return Screen(s_primaryMonitor);
	}

	Screen Screen::FromHandle(intptr_t hwnd) {
		if (SystemInformation::MultiMonitorSupport())
		{			
			auto i_monitor = Platform::System_MonitorFromHandle(hwnd);
			return Screen(i_monitor);
		}

		return Screen(s_primaryMonitor);
	}
}