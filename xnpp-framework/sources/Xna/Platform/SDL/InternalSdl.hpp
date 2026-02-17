#ifndef XNA_SDL_INPUT_HPP
#define XNA_SDL_INPUT_HPP

#include <cstdint>
#include <SDL3/SDL.h>
#include <vector>

namespace Xna {
	struct InternalSdl {
		static constexpr size_t MAX_GAMEPADS = 4;

		//Global mouse wheel
		static inline int g_MouseWheel = 0;
		//Global window handle
		static inline intptr_t g_CurrentWindowHandle = 0;
		static inline SDL_Gamepad* g_Gamepads[InternalSdl::MAX_GAMEPADS] = {};
		static inline SDL_JoystickID g_InstanceIDs[InternalSdl::MAX_GAMEPADS] = {};
	};

	struct SdlGamePadPlayer {
		SDL_Gamepad* gamepad = nullptr;
		// Player Index
		int id = -1;

		Uint16 low_frequency_rumble{ 0 };
		Uint16 high_frequency_rumble{ 0 };
		Uint16 left_rumble{ 0 };
		Uint16 right_rumble{ 0 };
		
		static constexpr Uint32 FRAME_RUMBLE_MS = 16;		
	};

	struct Global {
		static inline bool GamepadsEnabled = false;
		static inline std::vector<SdlGamePadPlayer> Gamepads = std::vector<SdlGamePadPlayer>(4);
	};
}

#endif