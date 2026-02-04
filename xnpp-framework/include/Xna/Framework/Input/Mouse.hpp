#ifndef XNA_INPUT_MOUSE_HPP
#define XNA_INPUT_MOUSE_HPP

#include "MouseState.hpp"
#include <cstdint>
#include <memory>
#include "Xna/Platform/Platform.hpp"

namespace Xna {
	//Allows retrieval of position and button clicks from a mouse input device. 
	class Mouse final {
	public:
		//Gets the current state of the mouse, including mouse position and buttons pressed. 
		static MouseState GetState();

		//Gets or sets the window used for mouse processing. 
		//Mouse coordinates returned by GetState are relative to the upper-left corner of this window. 
		static intptr_t WindowHandle() { Platform::Mouse_GetWindowHandle(); }

		//Gets or sets the window used for mouse processing. 
		//Mouse coordinates returned by GetState are relative to the upper-left corner of this window. 
		static void WindowHandle(intptr_t value) { Platform::Mouse_SetWindowHandle(value); }

		//Sets the position of the mouse cursor relative to the upper-left corner of the window.
		static void SetPosition(int32_t x, int32_t y) { Platform::Mouse_SetPosition(x, y); }

		Mouse() = delete;
		Mouse(Mouse&) = delete;
		Mouse(Mouse&&) = delete;
	};
}

#endif