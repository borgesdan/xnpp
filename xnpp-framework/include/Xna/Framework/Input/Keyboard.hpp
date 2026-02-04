#ifndef XNA_INPUT_KEYBOARD_HPP
#define XNA_INPUT_KEYBOARD_HPP

#include <memory>
#include "KeyboardState.hpp"
#include "Xna/Platform/Platform.hpp"

namespace Xna {
    //Allows retrieval of keystrokes from a keyboard input device. 
    struct Keyboard final {
    public:
        //Returns the current keyboard. 
        static inline KeyboardState GetState() { return Platform::Keyboard_GetState(); }

        Keyboard() = delete;
        Keyboard(Keyboard&) = delete;
        Keyboard(Keyboard&&) = delete;
    };
}

#endif