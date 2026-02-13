#include "Xna/Platform/Platform.hpp"
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
}