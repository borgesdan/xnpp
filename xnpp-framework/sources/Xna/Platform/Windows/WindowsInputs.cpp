#include "Xna/Platform/Platform.hpp"
#ifdef PLATFORM_WINDOWS

#include "Xna/Framework/Input/Keyboard.hpp"
#include "Xna/Framework/Input/Mouse.hpp"
#include "Xna/Framework/Input/GamePad.hpp"

namespace Xna {
	KeyboardState Platform::Keyboard_GetState() {
		const auto implState = WindowsPlatform::GetKeyboard().GetState();
		auto ptr = reinterpret_cast<const uint32_t*>(&implState);
		const auto state = reinterpret_cast<const KeyboardState*>(ptr);

		return *state;
	}

	void Platform::Keyboard_ProcessMessage(InputProcessMessage const& msg) {
		WindowsPlatform::GetKeyboard().ProcessMessage(msg.msg, msg.wParam, msg.lParam);
	}

	MouseState Platform::Mouse_GetState() {
		const auto state = WindowsPlatform::GetMouse().GetState();
		MouseState mstate;
		mstate.LeftButton = static_cast<ButtonState>(state.leftButton);
		mstate.RightButton = static_cast<ButtonState>(state.rightButton);
		mstate.MiddleButton = static_cast<ButtonState>(state.middleButton);
		mstate.XButton1 = static_cast<ButtonState>(state.xButton1);
		mstate.XButton2 = static_cast<ButtonState>(state.xButton2);
		mstate.X = state.x;
		mstate.Y = state.y;
		mstate.ScroolWheelValue = state.scrollWheelValue;

		return mstate;
	}

	void Platform::Mouse_ProcessMessage(InputProcessMessage const& msg) {
		WindowsPlatform::GetMouse().ProcessMessage(msg.msg, msg.wParam, msg.lParam);
	}

	void Platform::Mouse_SetWindowHandle(intptr_t value) {
		auto hwnd = reinterpret_cast<HWND>(value);

		if (!hwnd) return;

		WindowsPlatform::GetMouse().SetWindow(hwnd);
	}

	intptr_t Platform::Mouse_GetWindowHandle() {
		const auto state = WindowsPlatform::GetMouse().GetState();
		POINT p{};
		p.x = state.x;
		p.y = state.y;

		const auto hwnd = WindowFromPoint(p);
		return reinterpret_cast<intptr_t>(hwnd);
	}

	void Platform::Mouse_SetPosition(int32_t x, int32_t y) {
		SetCursorPos(static_cast<int>(x), static_cast<int>(y));
	}

	GamePadState Platform::GamePad_GetState(PlayerIndex playerIndex, GamePadDeadZone deadZone) {
		const auto stateDeadZone = static_cast<DirectX::GamePad::DeadZone>(deadZone);

		const auto state = WindowsPlatform::GetGamePad().GetState(static_cast<int>(playerIndex), stateDeadZone);

		const auto buttons = GamePadButtons(
			static_cast<ButtonState>(state.buttons.a),
			static_cast<ButtonState>(state.buttons.b),
			static_cast<ButtonState>(state.buttons.x),
			static_cast<ButtonState>(state.buttons.y),
			static_cast<ButtonState>(state.buttons.leftStick),
			static_cast<ButtonState>(state.buttons.rightStick),
			static_cast<ButtonState>(state.buttons.leftShoulder),
			static_cast<ButtonState>(state.buttons.rightShoulder),
			static_cast<ButtonState>(state.buttons.back),
			static_cast<ButtonState>(state.buttons.start),
			static_cast<ButtonState>(state.buttons.view));

		const auto dpad = GamePadDPad(
			static_cast<ButtonState>(state.dpad.up),
			static_cast<ButtonState>(state.dpad.right),
			static_cast<ButtonState>(state.dpad.down),
			static_cast<ButtonState>(state.dpad.left));

		const auto isConnected = state.connected;

		const auto thumbSticks = GamePadThumbSticks(
			Vector2(state.thumbSticks.leftX, state.thumbSticks.leftY),
			Vector2(state.thumbSticks.rightX, state.thumbSticks.rightY));
		const auto triggers = GamePadTriggers(state.triggers.left, state.triggers.right);

		const auto pad = GamePadState(thumbSticks, triggers, buttons, dpad, isConnected);
		return pad;
	}

	GamePadCapabilities Platform::GamePad_GetCapabilities(PlayerIndex playerIndex) {
		const auto index = static_cast<int>(playerIndex);
		auto capabilities = WindowsPlatform::GetGamePad().GetCapabilities(index);

		const auto gamePadType = static_cast<GamePadCapabilitiesType>(capabilities.gamepadType);
		const auto cap = GamePadCapabilities(capabilities.connected, gamePadType);

		return cap;
	}

	bool Platform::GamePad_SetVibration(PlayerIndex index, float leftMotor, float rightMotor, float leftTrigger, float rightTrigger) {
		return WindowsPlatform::GetGamePad().SetVibration(static_cast<int>(index), leftMotor, rightMotor, leftTrigger, rightTrigger);
	}

	void Platform::GamePad_Suspend() {
		WindowsPlatform::GetGamePad().Suspend();
	}

	void Platform::GamePad_Resume() {
		WindowsPlatform::GetGamePad().Resume();
	}
}

#endif