#ifndef XNA_FRAMEWORK_INPUT_GAMEPAD_HPP
#define XNA_FRAMEWORK_INPUT_GAMEPAD_HPP

#include "Shared.hpp"
#include "Xna/Framework/Vector2.hpp"

namespace Xna {
	//Specifies the game controller associated with a player.
	enum class PlayerIndex {
		//The first controller.
		One,
		//The second controller.
		Two,
		//The third controller.
		Three,
		//The fourth controller.
		Four
	};

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

	//Describes the type of a specified Xbox Controller. 
	enum class GamePadCapabilitiesType
	{
		//Controller is an unknown type
		Unknown = 0,
		//Controller is the Standard, Xbox 360, Xbox One, PS3, PS4, PS5 or Nintendo Switch Pro Controller
		GamePad,
		//Controller is an alternate guitar
		AlternateGuitar,
		//Controller is an Arcade stick
		ArcadeStick,
		//Controller is a big button pad
		BigButtonPad,
		//Controller is a dance pad
		DancePad,
		//Controller is a drum kit
		DrumKit,
		//Controller is a flight stick
		FlightStick,
		//Controller is a guitar
		Guitar,
		//Controller is a wheel
		Wheel,
	};

	//Describes the capabilities of an Xbox Controller, including controller type, and identifies if the controller supports voice. 
	struct GamePadCapabilities final {
		constexpr GamePadCapabilities() = default;

		constexpr GamePadCapabilities(bool isConnected, GamePadCapabilitiesType type) : isConnected(isConnected), type(type) {}

		//Indicates whether the Xbox Controller is connected.
		constexpr bool IsConnected() const { return isConnected; }
		//Gets the type of controller.
		constexpr GamePadCapabilitiesType GamePadType() const { return type; }

	private:
		GamePadCapabilitiesType type{ GamePadCapabilitiesType::Unknown };
		bool isConnected{ false };
	};

	//Identifies which directions on the directional pad of an Xbox Controller are being pressed.
	struct GamePadDPad final {
		constexpr GamePadDPad() = default;

		constexpr GamePadDPad(const ButtonState& up, const ButtonState& down,
			const ButtonState& left, const ButtonState& right) : up(up), down(down), left(left), right(right) {
		}

		//Identifies whether the Up direction on the Xbox Controller directional pad is pressed.
		constexpr ButtonState Up() const { return up; }
		//Identifies whether the Right direction on the Xbox Controller directional pad is pressed.
		constexpr ButtonState Right() const { return right; }
		//Identifies whether the Down direction on the Xbox Controller directional pad is pressed.
		constexpr ButtonState Down() const { return down; }
		//Identifies whether the Left direction on the Xbox Controller directional pad is pressed.
		constexpr ButtonState Left() const { return left; }

	private:
		ButtonState up{};
		ButtonState right{};
		ButtonState down{};
		ButtonState left{};
	};

	//Structure that represents the position of left and right sticks (thumbsticks) on an Xbox Controller.
	struct GamePadThumbSticks final {
		constexpr GamePadThumbSticks() = default;

		constexpr GamePadThumbSticks(Vector2 left, Vector2 right) : _left(left), _right(right) {
			clamp();
		}

		//Returns the position of the left Xbox Controller stick (thumbstick) as a 2D vector.
		constexpr Vector2 Left() const {
			return _left;
		}

		//Returns the position of the right Xbox Controller stick (thumbstick) as a 2D vector.
		constexpr Vector2 Right() const {
			return _right;
		}

		constexpr bool operator==(const GamePadThumbSticks& other) const {
			return _left == other._left && _right == other._right;
		}

	private:
		Vector2 _left{};
		Vector2 _right{};

	private:
		constexpr void clamp() {
			_left = Vector2::Min(_left, Vector2::One());
			_left = Vector2::Max(_left, -Vector2::One());
			_right = Vector2::Min(_right, Vector2::One());
			_right = Vector2::Max(_right, -Vector2::One());
		}
	};

	//Structure that defines the position of the left and right triggers on an Xbox Controller.
	struct GamePadTriggers final {
		constexpr GamePadTriggers() = default;

		constexpr GamePadTriggers(float left, float right)
			: _left(left), _right(right) {
			clamp();
		}

		//Identifies the position of the left trigger on the Xbox Controller.
		constexpr float Left() const {
			return _left;
		}

		//Identifies the position of the right trigger on the Xbox Controller.
		constexpr float Right() const {
			return _right;
		}

		constexpr bool operator==(const GamePadTriggers& other) const {
			return _left == other._left && _right == other._right;
		}

	private:
		float _left{ 0 };
		float _right{ 0 };

	private:
		void constexpr clamp() {
			_left = _left < 1.0F ? _left : 1.0F;
			_left = _left > 0.0F ? _left : 0.0F;
			_right = _right < 1.0F ? _right : 1.0F;
			_right = _right > 0.0F ? _right : 0.0F;
		}
	};

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

	//Specifies a type of dead zone processing to apply to Xbox Controller analog sticks when calling GetState. 
	enum class GamePadDeadZone {
		//The X and Y positions of each stick are compared against the dead zone independently. This setting is the default when calling GetState.
		IndependentAxes,
		//The combined X and Y position of each stick is compared to the dead zone. 
		//This provides better control than IndependentAxes when the stick is used as a two-dimensional control surface, such as when controlling a character's view in a first-person game.
		Circular,
		//The values of each stick are not processed and are returned by GetState as "raw" values. This is best if you intend to implement your own dead zone processing.
		None,
	};

	//Allows retrieval of user interaction with an Xbox Controller and setting of controller vibration motors.
	struct GamePad final {
	public:		
		//Gets the current state of a game pad controller. As an option, it specifies a dead zone processing method for the analog sticks.
		static GamePadState GetState(PlayerIndex index, GamePadDeadZone deadZone = GamePadDeadZone::IndependentAxes);

		//Retrieves the capabilities of an Xbox Controller.
		static GamePadCapabilities GetCapabilities(PlayerIndex index);
		//Sets the vibration motor speeds on an Xbox Controller.
		//leftMotor: The speed of the left motor, between 0.0 and 1.0.This motor is a low - frequency motor.
		//rightMotor: The speed of the right motor, between 0.0 and 1.0. This motor is a high-frequency motor.
		static bool SetVibration(PlayerIndex index, float leftMotor, float rightMotor, float leftTrigger = 0, float rightTrigger = 0);
	
		GamePad() = delete;
		GamePad(GamePad&) = delete;
		GamePad(GamePad&&) = delete;
	};
}

#endif