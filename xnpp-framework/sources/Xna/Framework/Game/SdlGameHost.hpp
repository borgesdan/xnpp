#pragma once
#include <SDL3/SDL.h>
#include <Xna/Framework/Game/GameHost.hpp>
#include <Xna/Macros/CoreAssert.hpp>
#include <Xna/Framework/Game/GameWindow.hpp>
#include <memory>
#include "SdlGameWindow.hpp"

namespace Xna {
	struct GamepadPlayer {
		SDL_Gamepad* gamepad = nullptr;
		int index = -1;

		Uint16 low_frequency_rumble{ 0 };
		Uint16 high_frequency_rumble{ 0 };
		Uint16 left_rumble{ 0 };
		Uint16 right_rumble{ 0 };
	};

	struct SdlGameHost final : public IGameHost {
		SdlGameHost() {
			gameWindow = std::make_unique<SdlGameWindow>();
		}

		~SdlGameHost() override = default;

		void Tick() override;
		IGameWindow* Window() const noexcept override { return gameWindow.get(); }

		std::unique_ptr<IGameWindow> gameWindow{nullptr};
	};
}