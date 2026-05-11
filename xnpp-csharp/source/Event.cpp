#include "Xna/CSharp/Event.hpp"

namespace Xna::CSharp {
    const EventArgs& EventArgs::Empty()
    {
        static EventArgs instance;
        return instance;
    }
}