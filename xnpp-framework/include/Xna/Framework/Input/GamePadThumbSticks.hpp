#ifndef XNA_FRAMEWORK_GAMEPADTHUMBSTICKS_HPP
#define XNA_FRAMEWORK_GAMEPADTHUMBSTICKS_HPP

#include "../Vector2.hpp"

namespace Xna {
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
}

#endif