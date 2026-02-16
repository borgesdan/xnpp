#ifndef XNA_SDL_INPUT_HPP
#define XNA_SDL_INPUT_HPP

#include <cstdint>
#include <SDL3/SDL.h>

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
}

#endif