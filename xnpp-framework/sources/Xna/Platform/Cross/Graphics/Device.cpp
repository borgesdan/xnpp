#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include "SDL3/SDL.h"
#include "Xna/Platform/_Platform.hpp"

namespace Xna {
	//Com DirectX11 inicializamos o Swapchain após a criação da janela no GameHost.
	//O dispositivo gráfico é criado antes, após o GameHost criar a janela e depois o Swapchain é criado.
	//Com o bgfx precisamos da informação da janela para inicializá-lo.
	void Platform::GraphicsDevice_LazyInitialization1(intptr_t windowHandle) {
		auto window = reinterpret_cast<SDL_Window*>(windowHandle);

		bgfx::PlatformData pd{};

#ifdef PLATFORM_WINDOWS
		pd.nwh = SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
#elif PLATFORM_LINUX
		const auto props = SDL_GetWindowProperties(window);
		const auto driver = SDL_GetCurrentVideoDriver();
		std::string driver_name = driver;

		if (driver_name == "wayland") {
			pd.ndt = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, NULL);
			pd.nwh = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, NULL);
		}
		else {
			pd.nwh = SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_WINDOW_NUMBER, NULL);
			pd.ndt = SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_DISPLAY_POINTER, NULL);
		}
#elif PLATFORM_MACOS
		pd.nwh = (void*)SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, NULL);
#endif	

		bgfx::Init init{};
		init.platformData = pd;
		init.type = bgfx::RendererType::Count;

		if (!bgfx::init(init))
			throw std::runtime_error("bgfx init failed");
	}
}