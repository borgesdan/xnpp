#include "Xna/CSharp/Windows/Forms/SystemInformation.hpp"
#include "Xna/Platform/Platform.hpp"

namespace Xna::CSharp {
	bool SystemInformation::MultiMonitorSupport() {
		if (!s_checkMultiMonitorSupport) {
			s_multiMonitorSupport = Platform::System_MultiMonitorSupport();
			s_checkMultiMonitorSupport = true;
		}

		return s_multiMonitorSupport;
	}

	Rectangle SystemInformation::VirtualScreen() {
		if (MultiMonitorSupport()) {
			auto rect = Platform::System_VirtualScreen();
			return { rect.X(), rect.Y(), rect.W(), rect.H() };
		}

		const auto size = PrimaryMonitorSize();
		return Rectangle(0, 0, size.Width, size.Height);
	}

	Size SystemInformation::PrimaryMonitorSize() {
		auto size = Platform::System_PrimaryMonitorSize();
		return Size(size.Width, size.Height);
	}	

	Rectangle SystemInformation::WorkingArea() {
		const auto area = Platform::System_WorkingArea();
		const auto rect = Rectangle::FromLTRB(area.Left, area.Top, area.Right, area.Bottom);

		return rect;
	}
}