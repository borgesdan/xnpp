#include "Xna/Framework/Input/Keyboard.hpp"
#include "Xna/Platform/Platform.hpp"

namespace Xna {
	//Returns the current keyboard. 
	KeyboardState Keyboard::GetState() { return Platform::Keyboard_GetState(); }
}