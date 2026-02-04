#ifndef XNA_INPUT_MOUSESTATE_HPP
#define XNA_INPUT_MOUSESTATE_HPP

#include "Shared.hpp"

namespace Xna {
	//Represents the state of a mouse input device, including mouse cursor position and buttons pressed.
	struct MouseState final {
		//Returns the state of the left mouse button.
		ButtonState LeftButton{ ButtonState::Released };
		//Returns the state of the middle mouse button.
		ButtonState MiddleButton{ ButtonState::Released };
		//Returns the state of the right mouse button.
		ButtonState RightButton{ ButtonState::Released };
		//Gets the cumulative mouse scroll wheel value since the game was started.
		int ScroolWheelValue{ 0 };
		//Specifies the horizontal position of the mouse cursor.
		int X{ 0 };
		//Returns the state of XBUTTON1.
		ButtonState XButton1{ ButtonState::Released };
		//Returns the state of XBUTTON2.
		ButtonState XButton2{ ButtonState::Released };
		//Specifies the vertical position of the mouse cursor.
		int Y{ 0 };
	};
}

#endif