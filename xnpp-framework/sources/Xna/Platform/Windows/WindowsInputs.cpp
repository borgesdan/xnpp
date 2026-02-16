#include "Xna/Platform/Platform.hpp"
#ifdef PLATFORM_WINDOWS

#include "Xna/Framework/Input/Keyboard.hpp"
#include "Xna/Framework/Input/Mouse.hpp"
#include "Xna/Framework/Input/GamePad.hpp"

namespace Xna {		

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