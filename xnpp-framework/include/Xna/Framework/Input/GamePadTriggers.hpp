#ifndef XNA_FRAMEWORK_INPUT_GAMEPADTRIGGERS_HPP
#define XNA_FRAMEWORK_INPUT_GAMEPADTRIGGERS_HPP

namespace Xna {
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
}

#endif