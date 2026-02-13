#include "Xna/Platform/Platform.hpp"
#include <algorithm>
#include <SDL3/SDL.h>

namespace Xna {
	PlatformRectangle Platform::System_ClientRect(intptr_t hwnd) {
		auto window = reinterpret_cast<SDL_Window*>(hwnd);
		if (window) {
			int width, height;
			SDL_GetWindowSizeInPixels(window, &width, &height);
			return { 0,0, width, height };
		}

		return {};
	}

	PlatformRectangle Platform::System_WindowRect(intptr_t hwnd) {
		auto window = reinterpret_cast<SDL_Window*>(hwnd);		

		if (window) {
			int x, y, w, h;
			SDL_GetWindowPosition(window, &x, &y);			
			SDL_GetWindowSize(window, &w, &h);

			return PlatformRectangle(x, y, x + w, y + h);
		}

		return {};
	}

	size_t Platform::System_GetClockCounter() {
		return static_cast<size_t>(SDL_GetPerformanceCounter());
	}

	size_t Platform::System_GetClockFrequency() {
		return static_cast<size_t>(SDL_GetPerformanceFrequency());
	}

	bool Platform::System_MultiMonitorSupport() {
		int count = 0;		
		auto displays = SDL_GetDisplays(&count);
		
		if (displays) 
			SDL_free(displays);

		return count > 1;
	}

	PlatformRectangle Platform::System_VirtualScreen() {
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
					maxX = rect.x + rect.w;
					maxY = rect.y + rect.h;
				}
				else {
					minX = std::min(minX, rect.x);
					minY = std::min(minY, rect.y);
					maxX = std::max(maxX, rect.x + rect.w);
					maxY = std::max(maxY, rect.y + rect.h);
				}
			}
		}

		SDL_free(displays);

		return PlatformRectangle(minX, minY, maxX, maxY);
	}
}