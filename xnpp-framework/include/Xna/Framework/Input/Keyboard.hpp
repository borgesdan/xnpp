#ifndef XNA_INPUT_KEYBOARD_HPP
#define XNA_INPUT_KEYBOARD_HPP

#include <memory>
#include "KeyboardState.hpp"

namespace Xna {
    //Allows retrieval of keystrokes from a keyboard input device. 
    struct Keyboard final {
    public:
        //Returns the current keyboard. 
        static KeyboardState GetState();

        Keyboard() = delete;
        Keyboard(Keyboard&) = delete;
        Keyboard(Keyboard&&) = delete;
    };
}

#endif