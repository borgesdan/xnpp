#ifndef XNA_FRAMEWORK_INPUT_GAMEPAD_HPP
#define XNA_FRAMEWORK_INPUT_GAMEPAD_HPP

#include "GamePadState.hpp"
#include "GamePadCapabilities.hpp"
#include "Shared.hpp"
#include <memory>
#include "Xna/Platform/Platform.hpp"

namespace Xna {
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
		inline static GamePadState GetState(PlayerIndex index, GamePadDeadZone deadZone = GamePadDeadZone::IndependentAxes) {
			Platform::GamePad_GetState(index, deadZone);
		}

		//Retrieves the capabilities of an Xbox Controller.
		static inline GamePadCapabilities GetCapabilities(PlayerIndex index) { Platform::GamePad_GetCapabilities(index); }
		//Sets the vibration motor speeds on an Xbox Controller.
		static inline bool SetVibration(PlayerIndex index, float leftMotor, float rightMotor, float leftTrigger = 0, float rightTrigger = 0) {
			Platform::GamePad_SetVibration(index, leftMotor, rightMotor, leftTrigger, rightTrigger);
		}
	
		GamePad() = delete;
		GamePad(GamePad&) = delete;
		GamePad(GamePad&&) = delete;
	};
}

#endif