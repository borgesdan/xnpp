#ifndef XNA_CSHARP_PLATFORM_HPP
#define XNA_CSHARP_PLATFORM_HPP

#include "Macros.hpp"
#include <cstdint>
#include "Drawing/Point.hpp"
#include "Drawing/Rectangle.hpp"
#include "Drawing/Size.hpp"
#include <string>
#include <vector>
#include <filesystem>

//OS
#if defined(_WIN32)
#define PLATFORM_WINDOWS 1
#elif defined(__APPLE__) && defined(__MACH__)
#define PLATFORM_MACOS 1
#elif defined(__ANDROID__)
#define PLATFORM_ANDROID 1
#elif defined(__linux__)
#define PLATFORM_LINUX 1
#elif defined(__unix__)
#define PLATFORM_UNIX 1
#else
#error "Sistema operacional não suportado"
#endif

namespace Xna::CSharp::Platform {

	struct Value2 {
		intptr_t One{ 0 };
		intptr_t Two{ 0 };

		constexpr Value2() = default;
		constexpr Value2(intptr_t one, intptr_t two)
			: One(one), Two(two) {
		}
	};

	XNPP_API Rectangle System_ClientRect(intptr_t hwnd);
	XNPP_API Rectangle System_WindowRect(intptr_t hwnd);
	XNPP_API size_t System_GetClockCounter();
	XNPP_API size_t System_GetClockFrequency();
	XNPP_API bool System_MultiMonitorSupport();
	XNPP_API Rectangle System_VirtualScreen();
	XNPP_API Size System_PrimaryMonitorSize();
	XNPP_API Rectangle System_WorkingArea();
	XNPP_API Rectangle System_MonitorWorkingArea(intptr_t hMonitor);
	XNPP_API intptr_t System_MonitorFromHandle(intptr_t hwnd);
	XNPP_API intptr_t System_MonitorFromRect(int32_t left, int32_t top, int32_t right, int32_t bottom);
	XNPP_API intptr_t System_MonitorFromPoint(int32_t x, int32_t y);
	XNPP_API std::string System_MonitorDeviceName(intptr_t monitor);
	XNPP_API Rectangle System_MonitorArea(intptr_t monitor);
	XNPP_API bool System_MonitorIsPrimary(intptr_t monitor);
	XNPP_API int32_t System_MonitorBitDepth(intptr_t monitor, intptr_t hdc);
	XNPP_API std::vector<Value2> System_GetAllMonitorHandlers();
	XNPP_API void System_GetExecutablePath(std::filesystem::path& path);
}

#endif