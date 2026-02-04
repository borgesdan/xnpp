#ifndef XNA_FRAMEWORK_INPUT_GAMEPADBUTTONS_HPP
#define XNA_FRAMEWORK_INPUT_GAMEPADBUTTONS_HPP

#include "Shared.hpp"

namespace Xna {
	//Enumerates input device buttons. 
	enum class Buttons {
		A = 4096, // 0x00001000
		B = 8192, // 0x00002000
		X = 16384, // 0x00004000
		Y = 32768, // 0x00008000
		Back = 32, // 0x00000020
		Start = 16, // 0x00000010
		DPadUp = 1,
		DPadDown = 2,
		DPadLeft = 4,
		DPadRight = 8,
		LeftShoulder = 256, // 0x00000100
		RightShoulder = 512, // 0x00000200
		LeftStick = 64, // 0x00000040
		RightStick = 128, // 0x00000080
		BigButton = 2048, // 0x00000800
		LeftThumbstickLeft = 2097152, // 0x00200000
		LeftThumbstickRight = 1073741824, // 0x40000000
		LeftThumbstickDown = 536870912, // 0x20000000
		LeftThumbstickUp = 268435456, // 0x10000000
		RightThumbstickLeft = 134217728, // 0x08000000
		RightThumbstickRight = 67108864, // 0x04000000
		RightThumbstickDown = 33554432, // 0x02000000
		RightThumbstickUp = 16777216, // 0x01000000
		LeftTrigger = 8388608, // 0x00800000
		RightTrigger = 4194304, // 0x00400000
	};

	//Identifies whether buttons on an Xbox Controller are pressed or released. 
	struct GamePadButtons final {
		constexpr GamePadButtons() = default;

		constexpr GamePadButtons(
			ButtonState a,
			ButtonState b,
			ButtonState x,
			ButtonState y,
			ButtonState leftStick,
			ButtonState rightStick,
			ButtonState leftShoulder,
			ButtonState rightShoulder,
			ButtonState back,
			ButtonState start,
			ButtonState bigButton) {
			this->a = a;
			this->b = b;
			this->x = x;
			this->y = y;
			this->leftStick = leftStick;
			this->rightStick = rightStick;
			this->leftShoulder = leftShoulder;
			this->rightShoulder = rightShoulder;
			this->back = back;
			this->start = start;
			this->bigButton = bigButton;
		}

		constexpr GamePadButtons(Buttons buttons) {
			a = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::A))) == Buttons::A ? ButtonState::Pressed : ButtonState::Released;
			b = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::B))) == Buttons::B ? ButtonState::Pressed : ButtonState::Released;
			x = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::X))) == Buttons::X ? ButtonState::Pressed : ButtonState::Released;
			y = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::Y))) == Buttons::Y ? ButtonState::Pressed : ButtonState::Released;
			start = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::Start))) == Buttons::Start ? ButtonState::Pressed : ButtonState::Released;
			back = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::Back))) == Buttons::Back ? ButtonState::Pressed : ButtonState::Released;
			leftStick = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::LeftStick))) == Buttons::LeftStick ? ButtonState::Pressed : ButtonState::Released;
			rightStick = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::RightStick))) == Buttons::RightStick ? ButtonState::Pressed : ButtonState::Released;
			leftShoulder = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::LeftShoulder))) == Buttons::LeftShoulder ? ButtonState::Pressed : ButtonState::Released;
			rightShoulder = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::RightShoulder))) == Buttons::RightShoulder ? ButtonState::Pressed : ButtonState::Released;
			bigButton = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::BigButton))) == Buttons::BigButton ? ButtonState::Pressed : ButtonState::Released;
		}

		//Identifies if the A button on the Xbox Controller is pressed. 
		constexpr ButtonState A() const { return a; }
		//Identifies if the B button on the Xbox Controller is pressed.
		constexpr ButtonState B() const { return b; }
		//Identifies if the X button on the Xbox Controller is pressed. 
		constexpr ButtonState X() const { return x; }
		//Identifies if the Y button on the Xbox Controller is pressed. 
		constexpr ButtonState Y() const { return y; }
		//Identifies if the left stick button on the Xbox Controller is pressed (the stick is "clicked" in
		constexpr ButtonState LeftStick() const { return leftStick; }
		//Identifies if the right stick button on the Xbox Controller is pressed (the stick is "clicked" in).
		constexpr ButtonState RightStick() const { return rightStick; }
		//Identifies if the left shoulder (bumper) button on the Xbox Controller is pressed.
		constexpr ButtonState LeftShoulder() const { return leftShoulder; }
		//Identifies if the right shoulder (bumper) button on the Xbox Controller is pressed.
		constexpr ButtonState RightShoulder() const { return rightShoulder; }
		//Identifies if the BACK button on the Xbox Controller is pressed. 
		constexpr ButtonState Back() const { return back; }
		//Identifies if the START button on the Xbox Controller is pressed. 
		constexpr ButtonState Start() const { return start; }
		//Identifies if the BigButton button is pressed. 
		constexpr ButtonState BigButton() const { return bigButton; }

	private:
		ButtonState a{};
		ButtonState b{};
		ButtonState x{};
		ButtonState y{};
		ButtonState leftStick{};
		ButtonState rightStick{};
		ButtonState leftShoulder{};
		ButtonState rightShoulder{};
		ButtonState back{};
		ButtonState start{};
		ButtonState bigButton{};
	};

}

#endif