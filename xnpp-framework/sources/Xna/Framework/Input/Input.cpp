#include "Xna/Framework/Input/Keyboard.hpp"
#include "Xna/Framework/Input/Mouse.hpp"
#include "Xna/Framework/Input/GamePad.hpp"
#include "Xna/Platform/Platform.hpp"

namespace Xna {	
	KeyboardState Keyboard::GetState() { 
		return Platform::Input::KeyboardGetState(); 
	}
		
	MouseState Mouse::GetState() { 
		return Platform::Input::MouseGetState(); 
	}
		
	intptr_t Mouse::WindowHandle() { 
		return Platform::Input::MouseGetWindowHandle(); 
	}
	
	void Mouse::WindowHandle(intptr_t value) {
		Platform::Input::MouseSetWindowHandle(value); 
	}
	
	void Mouse::SetPosition(int32_t x, int32_t y) {
		Platform::Input::MouseSetPosition(x, y); 
	}

	GamePadState GamePad::GetState(PlayerIndex index, GamePadDeadZone deadZone) {
		return Platform::Input::GamePadGetState(index, deadZone);
	}

	GamePadCapabilities GamePad::GetCapabilities(PlayerIndex index) { 
		return Platform::Input::GamePadGetCapabilities(index); 
	}

	bool GamePad::SetVibration(PlayerIndex index, float leftMotor, float rightMotor, float leftTrigger, float rightTrigger) {
		return Platform::Input::GamePadSetVibration(index, leftMotor, rightMotor, leftTrigger, rightTrigger);
	}
}