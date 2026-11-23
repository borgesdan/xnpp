#pragma once

#include <Xna/Framework/Game/GameWindow.hpp>
#include <SDL3/SDL.h>

namespace Xna {


	struct SdlGameWindow final : public IGameWindow {
		SDL_Window* window;

		intptr_t Handle() const noexcept override { return reinterpret_cast<intptr_t>(window); }

		void Title(std::string_view value) override;
		void AllowUserResizing(bool value) override;
		void Close() override;
		void Create() override;
		void IsMouseVisible(bool value) override;
		void IsMinimized(bool value) override;
		void Update() override;
		Rectangle ClientBounds() const override;		
	};
}