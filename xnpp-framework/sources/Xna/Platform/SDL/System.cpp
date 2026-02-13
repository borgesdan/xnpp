#include "Xna/Platform/Platform.hpp"
#include <SDL3/SDL.h>

namespace Xna {
	PlatformRectangle Platform::System_ClientRect(intptr_t hwnd) {
		auto window = reinterpret_cast<SDL_Window*>(hwnd);
		int width, height;

		if (window) {
			SDL_GetWindowSizeInPixels(window, &width, &height);
			return { 0,0, width, height };
		}

		return {};
	}
}