#ifndef XNA_FRAMEWORK_INPUT_GAMEPADCAPABILITIES_HPP
#define XNA_FRAMEWORK_INPUT_GAMEPADCAPABILITIES_HPP

namespace Xna {
    //Describes the type of a specified Xbox Controller. 
    enum class GamePadCapabilitiesType
    {
		//Controller is an unknown type
        Unknown =0,
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
}

#endif