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

	struct System {
		XNPP_API static Rectangle ClientRect(intptr_t hwnd);
		XNPP_API static Rectangle WindowRect(intptr_t hwnd);
		XNPP_API static size_t GetClockCounter();
		XNPP_API static size_t GetClockFrequency();
		XNPP_API static bool MultiMonitorSupport();
		XNPP_API static Rectangle VirtualScreen();
		XNPP_API static Size PrimaryMonitorSize();
		XNPP_API static Rectangle WorkingArea();
		XNPP_API static Rectangle MonitorWorkingArea(intptr_t hMonitor);
		XNPP_API static intptr_t MonitorFromHandle(intptr_t hwnd);
		XNPP_API static intptr_t MonitorFromRect(int32_t left, int32_t top, int32_t right, int32_t bottom);
		XNPP_API static intptr_t MonitorFromPoint(int32_t x, int32_t y);
		XNPP_API static std::string MonitorDeviceName(intptr_t monitor);
		XNPP_API static Rectangle MonitorArea(intptr_t monitor);
		XNPP_API static bool MonitorIsPrimary(intptr_t monitor);
		XNPP_API static int32_t MonitorBitDepth(intptr_t monitor, intptr_t hdc);
		XNPP_API static std::vector<Value2> GetAllMonitorHandlers();
		XNPP_API static void GetExecutablePath(std::filesystem::path& path);
	};	
}

#endif