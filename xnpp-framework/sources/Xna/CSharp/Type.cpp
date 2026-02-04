#include "Xna/CSharp/Type.hpp"

namespace Xna::CSharp {
    std::map<std::string, Type>& Type::NamedTypes() {
        static std::map<std::string, Type> namedTypes;
        return namedTypes;
    }
}