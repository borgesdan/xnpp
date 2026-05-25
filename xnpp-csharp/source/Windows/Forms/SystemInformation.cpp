#include "Xna/CSharp/Windows/Forms/SystemInformation.hpp"
#include "Xna/CSharp/Platform.hpp"

namespace Xna::CSharp {
	bool SystemInformation::MultiMonitorSupport() {
		if (!s_checkMultiMonitorSupport) {
			s_multiMonitorSupport = Platform::System::MultiMonitorSupport();
			s_checkMultiMonitorSupport = true;
		}

		return s_multiMonitorSupport;
	}

	Rectangle SystemInformation::VirtualScreen() {
		if (MultiMonitorSupport()) 
			return Platform::System::VirtualScreen();

		const auto size = PrimaryMonitorSize();
		return Rectangle(0, 0, size.Width, size.Height);
	}

	Size SystemInformation::PrimaryMonitorSize() {
		auto size = Platform::System::PrimaryMonitorSize();
		return Size(size.Width, size.Height);
	}	

	Rectangle SystemInformation::WorkingArea() {
		return Platform::System::WorkingArea();
	}
}