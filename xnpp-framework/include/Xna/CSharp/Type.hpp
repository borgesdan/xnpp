#ifndef CSHARP_TYPE_HPP
#define CSHARP_TYPE_HPP

#include <iostream>
#include <typeinfo>
#include <vector>
#include <memory>
#include <map>
#include <string>
#include <optional>
#include <functional>

namespace Xna::CSharp {
    class Type {
    public:
        Type() : info_(&typeid(void))  {}
        Type(const std::type_info& info) : info_(&info) {}

        inline bool operator==(const Type& other) const {
            return *info_ == *other.info_;
        }        
        
        inline static Type* GetType(std::string const& typeName) {
            auto find = NamedTypes().find(typeName);

            if (find != NamedTypes().end())
                return &find->second;

            return nullptr;
        }

        inline bool IsValueType() const {
            return false;
        }        

        inline const std::type_info& info() const { return *info_; }

        inline std::string Name() const {
            return info_->name();
        }

        inline bool operator<(const Type& other) const {
            return info_->name() < other.info().name();
        }      
        

    private:        
        const std::type_info* info_;        

    public:     
        static std::map<std::string, Type>& NamedTypes();
    };
}

namespace std {
    template <> struct hash<Xna::CSharp::Type> {
        size_t operator()(const Xna::CSharp::Type& t) const noexcept {            
            return std::hash<std::string>{}(std::string(t.info().name()));            
        }
    };
}

#endif