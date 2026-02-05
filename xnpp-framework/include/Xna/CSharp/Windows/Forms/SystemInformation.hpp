#ifndef CSHARP_WINDOWS_FORMS_SYSTEMINFORMATION_HPP
#define CSHARP_WINDOWS_FORMS_SYSTEMINFORMATION_HPP

#include "../../Drawing/Rectangle.hpp"
#include <cstdint>
#include "Xna/Internal/Export.hpp"

namespace Xna::CSharp {
	struct XNPP_API SystemInformation {
		static bool MultiMonitorSupport();
		static Rectangle VirtualScreen();
		static Size PrimaryMonitorSize();
		static Rectangle WorkingArea();	

	private:
		inline static bool s_checkMultiMonitorSupport{ false };
		inline static bool s_multiMonitorSupport{ false };
	};
}

#endif