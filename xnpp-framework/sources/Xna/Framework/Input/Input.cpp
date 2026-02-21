#include "Xna/Framework/Input/Keyboard.hpp"
#include "Xna/Framework/Input/Mouse.hpp"
#include "Xna/Framework/Input/GamePad.hpp"
#include "Xna/Platform/Platform.hpp"

namespace Xna {	
	KeyboardState Keyboard::GetState() { 
		return Platform::Keyboard_GetState(); 
	}
		
	MouseState Mouse::GetState() { 
		return Platform::Mouse_GetState(); 
	}
		
	intptr_t Mouse::WindowHandle() { 
		return Platform::Mouse_GetWindowHandle(); 
	}
	
	void Mouse::WindowHandle(intptr_t value) {
		Platform::Mouse_SetWindowHandle(value); 
	}
	
	void Mouse::SetPosition(int32_t x, int32_t y) {
		Platform::Mouse_SetPosition(x, y); 
	}

	GamePadState GamePad::GetState(PlayerIndex index, GamePadDeadZone deadZone) {
		return Platform::GamePad_GetState(index, deadZone);
	}

	GamePadCapabilities GamePad::GetCapabilities(PlayerIndex index) { 
		return Platform::GamePad_GetCapabilities(index); 
	}

	bool GamePad::SetVibration(PlayerIndex index, float leftMotor, float rightMotor, float leftTrigger, float rightTrigger) {
		return Platform::GamePad_SetVibration(index, leftMotor, rightMotor, leftTrigger, rightTrigger);
	}
}