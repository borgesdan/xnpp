#ifndef XNA_FRAMEWORK_INPUT_GAMEPADSTATE_HPP
#define XNA_FRAMEWORK_INPUT_GAMEPADSTATE_HPP

#include "GamePadThumbSticks.hpp"
#include "GamePadTriggers.hpp"
#include "GamePadButtons.hpp"
#include "GamePadDPad.hpp"

namespace Xna {
	//Represents specific information about the state of an Xbox Controller, including the current state of buttons and sticks.
	struct GamePadState final {
		constexpr GamePadState() = default;

		constexpr GamePadState(
			GamePadThumbSticks sticks,
			GamePadTriggers triggers,
			GamePadButtons buttons,
			GamePadDPad dpad, bool isConnected
		) : buttons(buttons), dpad(dpad), triggers(triggers), thumbSticks(sticks), isConnected(isConnected) {
		}

		//Determines whether specified input device buttons are pressed in this GamePadState.
		constexpr bool IsButtonDown(Buttons button) const {
			switch (button)
			{
			case Buttons::A:
				return this->Buttons().A() == ButtonState::Pressed;
			case Buttons::B:
				return this->Buttons().B() == ButtonState::Pressed;
			case Buttons::X:
				return this->Buttons().X() == ButtonState::Pressed;
			case Buttons::Y:
				return this->Buttons().Y() == ButtonState::Pressed;
			case Buttons::Back:
				return this->Buttons().Back() == ButtonState::Pressed;
			case Buttons::Start:
				return this->Buttons().Start() == ButtonState::Pressed;
			case Buttons::DPadUp:
				return this->Dpad().Up() == ButtonState::Pressed;
			case Buttons::DPadDown:
				return this->Dpad().Down() == ButtonState::Pressed;
			case Buttons::DPadLeft:
				return this->Dpad().Left() == ButtonState::Pressed;
			case Buttons::DPadRight:
				return this->Dpad().Right() == ButtonState::Pressed;
			case Buttons::LeftShoulder:
				return this->Buttons().LeftShoulder() == ButtonState::Pressed;
			case Buttons::RightShoulder:
				return this->Buttons().RightShoulder() == ButtonState::Pressed;
			case Buttons::LeftStick:
				return this->Buttons().LeftStick() == ButtonState::Pressed;
			case Buttons::RightStick:
				return this->Buttons().RightStick() == ButtonState::Pressed;
			case Buttons::BigButton:
				return this->Buttons().BigButton() == ButtonState::Pressed;
			case Buttons::LeftThumbstickLeft:
				return this->ThumbSticks().Left().X < 0.5F;
			case Buttons::LeftThumbstickRight:
				return this->ThumbSticks().Left().X > 0.5F;
			case Buttons::LeftThumbstickDown:
				return this->ThumbSticks().Left().Y > 0.5F;
			case Buttons::LeftThumbstickUp:
				return this->ThumbSticks().Left().Y < 0.5F;
			case Buttons::RightThumbstickLeft:
				return this->ThumbSticks().Right().X < 0.5F;
			case Buttons::RightThumbstickRight:
				return this->ThumbSticks().Right().X > 0.5F;
			case Buttons::RightThumbstickDown:
				return this->ThumbSticks().Right().Y > 0.5F;
			case Buttons::RightThumbstickUp:
				return this->ThumbSticks().Right().Y < 0.5F;
			case Buttons::LeftTrigger:
				return this->Triggers().Left() > 0.5F;
			case Buttons::RightTrigger:
				return this->Triggers().Right() > 0.5F;
			default:
				return false;
			}
		}

		//Determines whether specified input device buttons are up (not pressed) in this GamePadState.
		constexpr bool IsButtonUp(Buttons button) const {
			return !IsButtonDown(button);
		}

		//Returns a structure that identifies what buttons on the Xbox controller are pressed.
		constexpr GamePadButtons Buttons() const { return buttons; }
		//Returns a structure that identifies what directions of the directional pad on the Xbox Controller are pressed.
		constexpr GamePadDPad Dpad() const { return dpad; }
		//Indicates whether the Xbox Controller is connected.
		constexpr bool IsConnected() const { return isConnected; }
		//Returns a structure that indicates the position of the Xbox Controller sticks (thumbsticks).
		constexpr GamePadThumbSticks ThumbSticks() const { return thumbSticks; }
		//Returns a structure that identifies the position of triggers on the Xbox controller.
		constexpr GamePadTriggers Triggers() const { return triggers; }

	private:
		GamePadButtons buttons{};
		GamePadDPad dpad{};
		bool isConnected{ false };
		GamePadThumbSticks thumbSticks{};
		GamePadTriggers triggers{};
	};

}

#endif