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
#include "Macros.hpp"

namespace Xna::CSharp {
    template <typename TENUM>
    static constexpr void SetFlag(int& current, TENUM flag) {
        current |= static_cast<int>(flag);
    }

    template <typename TENUM>
    static constexpr void SetFlag(int& current, TENUM flag1, TENUM flag2) {
        current |= static_cast<int>(flag1) | static_cast<int>(flag2);
    }

    template <typename TENUM>
    static constexpr bool HasFlag(int const& current, TENUM flag) {
        return (current & static_cast<int>(flag)) == static_cast<int>(flag);
    }

    //
    // Smart Pointer Comparator
    //

    template<typename T> struct is_shared_ptr : std::false_type {};
    template<typename T> struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};
    template<typename T> struct is_weak_ptr : std::false_type {};
    template<typename T> struct is_weak_ptr<std::weak_ptr<T>> : std::true_type {};
    template<typename T> struct is_unique_ptr : std::false_type {};
    template<typename T> struct is_unique_ptr<std::unique_ptr<T>> : std::true_type {};

    //Returns true if the type is a smart pointer
    template <typename T>
    static constexpr bool IsSmartPointer() {
        return is_shared_ptr<T>::value || is_unique_ptr<T>::value || is_weak_ptr<T>::value;
    }

    enum class TypeFlags {
        None = 1 << 0,   
        Array = 1 << 1,//2
        Pointer = 1 << 2, //4
        Class = 1 << 3, //8
        Enum = 1 << 4, //16
        Primitive = 1 << 5, //32
        ValueType = 1 << 6, //64
        SmartPointer = 1 << 7, //128
        ReferenceType = 1 << 8, //256
        Interface = 1 << 9, //512
    };

    class Type {
    public:
        inline bool operator==(const Type& other) const {
            return *info_ == *other.info_;
        }        
        
        XNPP_API static Type* GetType(std::string const& typeName);

        inline bool IsValueType() const { return HasFlag(flags, TypeFlags::ValueType); }

        inline const std::type_info& info() const { return *info_; }
        inline std::string Name() const { return info_->name(); }
        inline bool operator<(const Type& other) const { return info_->name() < other.info().name(); }       
        
        template <class T> friend Type typeof();
        template <class T> friend Type typeof(T const& _);

        static Type Empty() { return Type(); }
        
        Type() : info_(&typeid(void)) {}
        Type(const std::type_info& info) : info_(&info) {}
    
    private:        
        template <class T> static constexpr Type FromTemplate();

    private:        
        const std::type_info* info_;
        int flags{ 0 };

    public:     
        static XNPP_API std::map<std::string, Type>& NamedTypes();
    };

    template <class T> Type typeof() { return Type::FromTemplate<T>(); }
    template <class T> Type typeof(T const& _) { return Type::FromTemplate<T>(); }

    template <class T> static constexpr Type Type::FromTemplate() {
        int flags{ 0 };
        
        if constexpr (std::is_arithmetic<T>::value) {
            SetFlag(flags, TypeFlags::Primitive, TypeFlags::ValueType);
        }
        else if constexpr (std::is_class<T>::value) {
            SetFlag(flags, TypeFlags::Class);

            if constexpr (std::is_constructible_v<T, std::nullptr_t>) {
                SetFlag(flags, TypeFlags::ReferenceType);
            }
            else {
                SetFlag(flags, TypeFlags::ValueType);
            }

            if constexpr (std::is_abstract<T>::value) {
                SetFlag(flags, TypeFlags::Interface);
            }
        }
        else if constexpr (std::is_enum<T>::value) {
            SetFlag(flags, TypeFlags::Enum, TypeFlags::ValueType);
        }       
        else if constexpr (std::is_pointer<T>::value || IsSmartPointer<T>()) {
            SetFlag(flags, TypeFlags::Pointer);

            if constexpr (IsSmartPointer<T>())
                SetFlag(flags, TypeFlags::SmartPointer);
        }                
        else if constexpr (std::is_array<T>::value) {
            SetFlag(flags, TypeFlags::Array);
        }

        CSharp::Type type = typeid(T);
        type.flags = flags;
        return type;
    }
}

namespace std {
    template <> struct hash<Xna::CSharp::Type> {
        size_t operator()(const Xna::CSharp::Type& t) const noexcept {            
            return std::hash<std::string>{}(std::string(t.info().name()));            
        }
    };
}


#endif