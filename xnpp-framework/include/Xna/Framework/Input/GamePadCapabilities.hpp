#ifndef XNA_FRAMEWORK_INPUT_GAMEPADCAPABILITIES_HPP
#define XNA_FRAMEWORK_INPUT_GAMEPADCAPABILITIES_HPP

namespace Xna {
    //Describes the type of a specified Xbox Controller. 
    enum class GamePadCapabilitiesType
    {
        Unknown = 0,
        Gamepad,
        Wheel,
        ArdaceStick,
        FlightStick,
        DancePAd,
        Guitar,
        GuitarAlternate,
        DrumKit,
        GuitarBass = 11,
        ArcadePad = 19,
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
		GamePadCapabilitiesType type{};
		bool isConnected{ false };
	};
}

#endif