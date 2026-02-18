#ifndef XNA_SDL_INPUT_HPP
#define XNA_SDL_INPUT_HPP

#include <cstdint>
#include <SDL3/SDL.h>
#include <vector>

namespace Xna::Sdl {
	struct System {
		static void Initialize();
	};

	struct GamepadPlayer {
		SDL_Gamepad* gamepad = nullptr;
		int index = -1;

		Uint16 low_frequency_rumble{ 0 };
		Uint16 high_frequency_rumble{ 0 };
		Uint16 left_rumble{ 0 };
		Uint16 right_rumble{ 0 };
		
		static constexpr Uint32 FRAME_RUMBLE_MS = 16;		
	};

	struct Global {
		static constexpr size_t MaxGamepads = 4;
		static inline bool SuspendGamepads = false;
		static inline std::vector<GamepadPlayer> Gamepads = std::vector<GamepadPlayer>(MaxGamepads);
		static inline int MouseWheel = 0;		
		static inline intptr_t CurrentWindowHandle = 0;
	};
}

#endif