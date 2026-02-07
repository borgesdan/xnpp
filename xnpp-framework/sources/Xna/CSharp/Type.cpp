#include "Xna/CSharp/Type.hpp"

namespace Xna::CSharp {
    std::map<std::string, Type>& Type::NamedTypes() {
        static std::map<std::string, Type> namedTypes;
        return namedTypes;
    }

    Type* Type::GetType(std::string const& typeName) {
        auto find = NamedTypes().find(typeName);

        if (find != NamedTypes().end())
            return &find->second;

        return nullptr;
    }    
}