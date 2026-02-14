#include "Xna/Platform/Platform.hpp"
#include "Xna/Framework/Game/GameWindow.hpp"
#include "Xna/CSharp/Exception.hpp"
#include <SDL3/SDL.h>

namespace Xna {
	enum class SdlGameWindowMode : SDL_WindowFlags {
		Fullscreen = SDL_WINDOW_FULLSCREEN,
		Windowed = SDL_WINDOW_RESIZABLE,
		Borderless = SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN,
	};

	static SDL_WindowFlags ConvertStyleToSdlWindowFlags(GameWindow::Style style) {
		switch (style)
		{
		case GameWindow::Style::Fullscreen:
			return static_cast<SDL_WindowFlags>(SdlGameWindowMode::Fullscreen);
		case GameWindow::Style::Windowed:
			return static_cast<SDL_WindowFlags>(SdlGameWindowMode::Windowed);
		case GameWindow::Style::Borderless:
			return static_cast<SDL_WindowFlags>(SdlGameWindowMode::Borderless);
		default:
			return static_cast<SDL_WindowFlags>(SdlGameWindowMode::Windowed);
		}
	}

	void Platform::GameWindow_Create(GameWindow const& gw) {
		auto& impl = gw.impl;

		if (!SDL_WasInit(SDL_INIT_VIDEO)) {
			if (!SDL_Init(SDL_INIT_VIDEO)) {
				throw CSharp::InvalidOperationException("SDL_Init(VIDEO) failed.");
			}
		}

		auto handle = SDL_CreateWindow(
			impl->title.c_str(),
			impl->width,
			impl->height,
			ConvertStyleToSdlWindowFlags(impl->style)
		);

		if (!handle)
			throw CSharp::InvalidOperationException(SDL_GetError());

		SDL_SetWindowPosition(handle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

		impl->handle = reinterpret_cast<intptr_t>(handle);
	}

	void Platform::GameWindow_Update(GameWindow const& gw) {
		auto handle = reinterpret_cast<SDL_Window*>(gw.impl->handle);

		int w, h;
		SDL_GetWindowSize(handle, &w, &h);		

		if (gw.impl->width != w || gw.impl->height != h) 
			SDL_SetWindowSize(handle, gw.impl->width, gw.impl->height);

		const auto flags = SDL_GetWindowFlags(handle);

		if ((flags & SDL_WINDOW_FULLSCREEN) != 0)
			SDL_SetWindowPosition(handle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}

	void Platform::GameWindow_Close(GameWindow const& gw) {
		SDL_Window* window = reinterpret_cast<SDL_Window*>(gw.impl->handle);

		if (window) {
			SDL_DestroyWindow(window);
			gw.impl->handle = 0;
		}
	}

	bool Platform::GameWindow_WindowIsMinimized(GameWindow const& gw) {
		auto handle = reinterpret_cast<SDL_Window*>(gw.impl->handle);
		const auto flags = SDL_GetWindowFlags(handle);

		return (flags & SDL_WINDOW_MINIMIZED) != 0;
	}

	void Platform::GameWindow_MinimizeWindow(GameWindow const& gw, bool value) {
		auto window = reinterpret_cast<SDL_Window*>(gw.impl->handle);

		if (!window)
			return;

		if (value) {
			SDL_MinimizeWindow(window);
		}
		else {
			SDL_RestoreWindow(window);
			SDL_RaiseWindow(window);
		}
	}

	void Platform::GameWindow_SetMouseVisible(GameWindow const& gw, bool value) {
		if (value)
			SDL_ShowCursor();
		else
			SDL_HideCursor();
	}

	void Platform::GameWindow_AllowUserResizing(GameWindow const& gw, bool value) {
		auto window = reinterpret_cast<SDL_Window*>(gw.impl->handle);

		if (window)
			SDL_SetWindowResizable(window, value);
	}

	Rectangle Platform::GameWindow_ClientBounds(GameWindow const& gw) {
		auto window = reinterpret_cast<SDL_Window*>(gw.impl->handle);

		int x, y, w, h;
		SDL_GetWindowPosition(window, &x, &y);
		SDL_GetWindowSize(window, &w, &h);

		return { x, y, w, h, };
	}

	void Platform::GameWindow_SetTitle(GameWindow const& gw, std::string const& title) {
		auto window = reinterpret_cast<SDL_Window*>(gw.impl->handle);
		SDL_SetWindowTitle(window, title.c_str());
	}	
}