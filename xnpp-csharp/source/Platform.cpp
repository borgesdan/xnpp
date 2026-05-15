#include "Xna/CSharp/Platform.hpp"
#include <SDL3/SDL.h>

#ifdef PLATFORM_WINDOWS
#define NOMINMAX
#include <windows.h>
#elif PLATFORM_LINUX
#include <unistd.h>
#include <limits.h>
#elif PLATFORM_MACOS
#include <mach-o/dyld.h>
#endif

namespace Xna::CSharp::Platform {
	Rectangle System_ClientRect(intptr_t hwnd) {
		auto window = reinterpret_cast<SDL_Window*>(hwnd);
		if (window) {
			int width, height;
			SDL_GetWindowSizeInPixels(window, &width, &height);
			return { 0,0, width, height };
		}

		return {};
	}

	Rectangle System_WindowRect(intptr_t hwnd) {
		auto window = reinterpret_cast<SDL_Window*>(hwnd);

		if (window) {
			int x, y, w, h;
			SDL_GetWindowPosition(window, &x, &y);
			SDL_GetWindowSize(window, &w, &h);

			return Rectangle(x, y, w, h);
		}

		return {};
	}

	size_t System_GetClockCounter() {
		return static_cast<size_t>(SDL_GetPerformanceCounter());
	}

	size_t System_GetClockFrequency() {
		return static_cast<size_t>(SDL_GetPerformanceFrequency());
	}

	bool System_MultiMonitorSupport() {
		int count = 0;
		auto displays = SDL_GetDisplays(&count);

		if (displays)
			SDL_free(displays);

		return count > 1;
	}

	Rectangle System_VirtualScreen() {
		int count = 0;
		auto displays = SDL_GetDisplays(&count);

		if (!displays || count == 0) {
			return {};
		}

		int minX = 0;
		int minY = 0;
		int maxX = 0;
		int maxY = 0;

		for (size_t i = 0; i < count; ++i) {
			SDL_Rect rect;

			if (SDL_GetDisplayBounds(displays[i], &rect)) {
				if (i == 0) {
					minX = rect.x;
					minY = rect.y;
					maxX = rect.w;
					maxY = rect.h;
				}
				else {
					minX = std::min(minX, rect.x);
					minY = std::min(minY, rect.y);
					maxX = std::max(maxX, rect.w);
					maxY = std::max(maxY, rect.h);
				}
			}
		}

		SDL_free(displays);

		return Rectangle(minX, minY, maxX, maxY);
	}

	Size System_PrimaryMonitorSize() {
		SDL_Rect rect;
		if (SDL_GetDisplayBounds(SDL_GetPrimaryDisplay(), &rect)) {
			return { rect.w, rect.h };
		}
		return {};
	}

	Rectangle System_WorkingArea() {
		SDL_Rect rect;

		if (SDL_GetDisplayUsableBounds(SDL_GetPrimaryDisplay(), &rect)) {
			return { rect.x, rect.y,  rect.w, rect.h };
		}

		return {};
	}

	Rectangle System_MonitorWorkingArea(intptr_t displayID) {
		SDL_Rect rect;
		const auto id = static_cast<SDL_DisplayID>(displayID);

		if (SDL_GetDisplayUsableBounds(id, &rect)) {
			return { rect.x, rect.y, rect.w, rect.h };
		}

		return {};
	}

	intptr_t System_MonitorFromHandle(intptr_t windowPtr) {
		auto window = reinterpret_cast<SDL_Window*>(windowPtr);
		SDL_DisplayID displayID = SDL_GetDisplayForWindow(window);

		return static_cast<intptr_t>(displayID);
	}

	intptr_t System_MonitorFromRect(int32_t left, int32_t top, int32_t right, int32_t bottom) {
		SDL_Rect targetRect = { left, top, right - left, bottom - top };

		int count = 0;
		SDL_DisplayID* displays = SDL_GetDisplays(&count);

		if (!displays || count == 0) return 0;

		SDL_DisplayID bestMonitor = displays[0];
		int maxOverlapArea = -1;

		for (int i = 0; i < count; ++i) {
			SDL_Rect displayBounds;
			if (SDL_GetDisplayBounds(displays[i], &displayBounds)) {
				SDL_Rect intersection;

				if (SDL_GetRectIntersection(&targetRect, &displayBounds, &intersection)) {
					int area = intersection.w * intersection.h;
					if (area > maxOverlapArea) {
						maxOverlapArea = area;
						bestMonitor = displays[i];
					}
				}
			}
		}

		SDL_free(displays);
		return static_cast<intptr_t>(bestMonitor);
	}

	intptr_t System_MonitorFromPoint(int32_t x, int32_t y) {
		SDL_Point point = { x, y };
		SDL_DisplayID displayID = SDL_GetDisplayForPoint(&point);

		if (displayID == 0) {
			displayID = SDL_GetPrimaryDisplay();
		}

		return static_cast<intptr_t>(displayID);
	}

	std::string System_MonitorDeviceName(intptr_t monitorID) {
		SDL_DisplayID id = static_cast<SDL_DisplayID>(monitorID);

		if (id == 0)
			id = SDL_GetPrimaryDisplay();

		const auto name = SDL_GetDisplayName(id);

		if (name) {
			return std::string(name);
		}

		return {};
	}

	Rectangle System_MonitorArea(intptr_t monitor) {
		SDL_DisplayID id = static_cast<SDL_DisplayID>(monitor);

		if (id == 0)
			id = SDL_GetPrimaryDisplay();

		SDL_Rect rect;

		if (!SDL_GetDisplayBounds(id, &rect))
			return {};

		return Rectangle(rect.x, rect.y, rect.w, rect.h);
	}

	bool System_MonitorIsPrimary(intptr_t monitor) {
		SDL_DisplayID id = static_cast<SDL_DisplayID>(monitor);
		const auto primary = SDL_GetPrimaryDisplay();

		return id == 0 || id == primary;
	}

	int32_t System_MonitorBitDepth(intptr_t monitorID, intptr_t _) {
		SDL_DisplayID id = static_cast<SDL_DisplayID>(monitorID);
		if (id == 0) {
			id = SDL_GetPrimaryDisplay();
		}

		const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(id);

		if (mode)
			return static_cast<int32_t>(SDL_BITSPERPIXEL(mode->format));

		return 32;
	}

	std::vector<Value2> System_GetAllMonitorHandlers() {
		std::vector<Value2> list;
		int count = 0;

		SDL_DisplayID* displays = SDL_GetDisplays(&count);

		if (displays) {
			for (size_t i = 0; i < count; ++i) {
				list.push_back(Value2(static_cast<intptr_t>(displays[i]), 0));
			}

			SDL_free(displays);
		}

		return list;
	}

	void System_GetExecutablePath(std::filesystem::path& path) {
#ifdef PLATFORM_WINDOWS
		wchar_t buffer[MAX_PATH]{};
		const auto length = GetModuleFileNameW(nullptr, buffer, MAX_PATH);

		if (length > 0)
			path = std::filesystem::path(buffer, buffer + length);
#elif PLATFORM_MACOS
		char buffer[PATH_MAX];
		uint32_t size = sizeof(buffer);
		if (_NSGetExecutablePath(buffer, &size) == 0) {
			path = std::filesystem::path(buffer);
		}
#elif PLATFORM_LINUX
		char buffer[PATH_MAX];
		ssize_t count = readlink("/proc/self/exe", buffer, PATH_MAX);
		if (count != -1) {
			path = std::filesystem::path(buffer, buffer + count);
		}
#endif
	}
}