#ifndef XNA_FRAMEWORK_INPUT_GAMEPADDPAD_HPP
#define XNA_FRAMEWORK_INPUT_GAMEPADDPAD_HPP

namespace Xna {
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
}

#endif