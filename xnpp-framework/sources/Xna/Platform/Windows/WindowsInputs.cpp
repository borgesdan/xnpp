#include "Xna/Platform/Platform.hpp"
#ifdef PLATFORM_WINDOWS

#include "Xna/Framework/Input/Keyboard.hpp"
#include "Xna/Framework/Input/Mouse.hpp"
#include "Xna/Framework/Input/GamePad.hpp"

namespace Xna {	

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