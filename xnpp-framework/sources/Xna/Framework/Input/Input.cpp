#include "Xna/Framework/Input/Keyboard.hpp"
#include "Xna/Framework/Input/Mouse.hpp"
#include "Xna/Platform/Platform.hpp"

namespace Xna {
	//Returns the current keyboard. 
	KeyboardState Keyboard::GetState() { return Platform::Keyboard_GetState(); }

	//Gets the current state of the mouse, including mouse position and buttons pressed. 
	MouseState Mouse::GetState() { return Platform::Mouse_GetState(); }

	//Gets or sets the window used for mouse processing. 
	//Mouse coordinates returned by GetState are relative to the upper-left corner of this window. 
	intptr_t Mouse::WindowHandle() { return Platform::Mouse_GetWindowHandle(); }

	//Gets or sets the window used for mouse processing. 
	//Mouse coordinates returned by GetState are relative to the upper-left corner of this window. 
	void Mouse::WindowHandle(intptr_t value) {	Platform::Mouse_SetWindowHandle(value); }

	//Sets the position of the mouse cursor relative to the upper-left corner of the window.
	void Mouse::SetPosition(int32_t x, int32_t y) { Platform::Mouse_SetPosition(x, y); }
}