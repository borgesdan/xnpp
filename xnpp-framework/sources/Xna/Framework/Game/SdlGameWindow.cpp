#include "SdlGameWindow.hpp"
#include <Xna/Macros/CoreAssert.hpp>

namespace Xna {
	using base = IGameWindow;

	static SDL_WindowFlags ConvertStyleToSdlWindowFlags(GameWindowStyle style) {
		switch (style)
		{
		case GameWindowStyle::Fullscreen:
			return static_cast<SDL_WindowFlags>(SDL_WINDOW_FULLSCREEN);
		case GameWindowStyle::Windowed:
			return static_cast<SDL_WindowFlags>(SDL_WINDOW_RESIZABLE);
		case GameWindowStyle::Borderless:
			return static_cast<SDL_WindowFlags>(SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN);
		default:
			return static_cast<SDL_WindowFlags>(SDL_WINDOW_RESIZABLE);
		}
	}

	void SdlGameWindow::Title(std::string_view value) {
		auto result = SDL_SetWindowTitle(window, value.data());

		XNPP_CORE_ASSERT(result, SDL_GetError());

		base::Title(value);
	}

	Rectangle SdlGameWindow::ClientBounds() const {
		int x, y, w, h;
		bool result;

		result = SDL_GetWindowPosition(window, &x, &y);
		XNPP_CORE_ASSERT(result, SDL_GetError());

		result = SDL_GetWindowSize(window, &w, &h);
		XNPP_CORE_ASSERT(result, SDL_GetError());

		return { x, y, w, h, };
	}

	void SdlGameWindow::AllowUserResizing(bool value) {
		auto result = SDL_SetWindowResizable(window, value);
		XNPP_CORE_ASSERT(result, SDL_GetError());

		base::AllowUserResizing(value);
	}

	void SdlGameWindow::Close() {
		SDL_DestroyWindow(window);

		base::Close();
	}

	void SdlGameWindow::Create() {
		bool result;

		result = SDL_Init(SDL_INIT_VIDEO);
		XNPP_CORE_ASSERT(result, SDL_GetError());

		auto handle = SDL_CreateWindow(
			"",
			Width(),
			Height(),
			ConvertStyleToSdlWindowFlags(GetStyle())
		);

		XNPP_CORE_ASSERT(handle, SDL_GetError());

		result = SDL_SetWindowPosition(handle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		XNPP_CORE_ASSERT(result, SDL_GetError());

		window = handle;
		base::Create();
	}

	void SdlGameWindow::IsMouseVisible(bool value) {
		bool result;
		if (value)
			result = SDL_ShowCursor();
		else
			result = SDL_HideCursor();

		XNPP_CORE_ASSERT(result, SDL_GetError());
		base::IsMouseVisible(value);
	}

	void SdlGameWindow::IsMinimized(bool value) {
		bool result;
		if (value) {
			result = SDL_MinimizeWindow(window);
			XNPP_CORE_ASSERT(result, SDL_GetError());
		}
		else {
			result = SDL_RestoreWindow(window);
			XNPP_CORE_ASSERT(result, SDL_GetError());
			result = SDL_RaiseWindow(window);
			XNPP_CORE_ASSERT(result, SDL_GetError());
		}

		base::IsMinimized(value);
	}

	void SdlGameWindow::Update() {
		int w, h;
		auto result = SDL_GetWindowSize(window, &w, &h);
		XNPP_CORE_ASSERT(result, SDL_GetError());

		if (Width() != w || Height() != h)
			result = SDL_SetWindowSize(window, Width(), Height());

		XNPP_CORE_ASSERT(result, SDL_GetError());

		const auto flags = SDL_GetWindowFlags(window);

		if ((flags & SDL_WINDOW_FULLSCREEN) != 0)
			result = SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

		XNPP_CORE_ASSERT(result, SDL_GetError());
	}
}