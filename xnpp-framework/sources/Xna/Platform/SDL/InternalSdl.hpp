#ifndef XNA_SDL_INPUT_HPP
#define XNA_SDL_INPUT_HPP

#include <cstdint>

namespace Xna {
	struct InternalSdl {
		//Global mouse wheel
		static inline int g_MouseWheel = 0;
		//Global window handle
		static inline intptr_t g_CurrentWindowHandle = 0;
	};
}

#endif