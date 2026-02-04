#include "Xna/CSharp/Windows/Forms/SystemInformation.hpp"
#include "Xna/Platform/Platform.hpp"
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
			return { get<0>(rect), get<1>(rect) ,get<2>(rect) ,get<3>(rect) };
		}

		const auto size = PrimaryMonitorSize();
		return Rectangle(0, 0, size.Width, size.Height);
	}

	Size SystemInformation::PrimaryMonitorSize() {
		auto size = Platform::System_PrimaryMonitorSize();
		return Size(get<0>(size), get<1>(size));
	}	

	Rectangle SystemInformation::WorkingArea() {
		const auto area = Platform::System_WorkingArea();
		const auto rect = Rectangle::FromLTRB(
			get<0>(area),
			get<1>(area),
			get<2>(area),
			get<3>(area));

		return rect;
	}
}