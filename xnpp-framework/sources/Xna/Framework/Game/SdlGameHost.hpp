#pragma once
#include "SdlGameWindow.hpp"
#include <memory>
#include <SDL3/SDL.h>
#include <Xna/Framework/Game/GameHost.hpp>
#include <Xna/Framework/Game/GameWindow.hpp>
#include <Xna/Macros/CoreAssert.hpp>

namespace Xna {
	struct GamepadPlayer {
		SDL_Gamepad* gamepad = nullptr;
		int index = -1;

		Uint16 low_frequency_rumble{ 0 };
		Uint16 high_frequency_rumble{ 0 };
		Uint16 left_rumble{ 0 };
		Uint16 right_rumble{ 0 };
	};

	struct SdlGameHost final : public GameHost {
		SdlGameHost() {
			gameWindow = std::make_unique<SdlGameWindow>();
		}

		~SdlGameHost() override = default;

		void Tick() override;
		GameWindow* Window() const noexcept override { return gameWindow.get(); }

		std::unique_ptr<GameWindow> gameWindow{nullptr};
	};
}