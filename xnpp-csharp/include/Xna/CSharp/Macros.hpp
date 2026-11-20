#ifndef XNA_MACROS_HPP
#define XNA_MACROS_HPP

#define XNPP_NULLABLE_CLASS(ClassName, Impl) \
public: \
    inline ClassName(std::nullptr_t) { Impl = nullptr; } \
    inline bool operator==(ClassName const& other) const noexcept { return Impl == other.Impl; } \
    inline bool operator==(std::nullptr_t) const noexcept { return Impl == nullptr; } \
    inline explicit operator bool() const noexcept { return Impl != nullptr; }

#define XNPP_NULLABLE_CLASS_WITH_BASE(ClassName, BaseClassName, Impl) \
public: \
    inline ClassName(std::nullptr_t) : BaseClassName(nullptr) { Impl = nullptr; } \
    inline bool operator==(ClassName const& other) const noexcept { return Impl == other.Impl; } \
    inline bool operator==(std::nullptr_t) const noexcept { return Impl == nullptr; } \
    inline explicit operator bool() const noexcept { return Impl != nullptr; }

#ifdef _WIN32
#ifdef XNPP_STATIC
// Se for estático, não usamos import/export
#define XNPP_API 
#else
// Se for DLL, verificamos se estamos construindo ou consumindo a DLL
#ifdef XNPP_EXPORT_DLL
#define XNPP_API __declspec(dllexport)
#else
#define XNPP_API __declspec(dllimport)
#endif
#endif
#else
// Para Linux/Mac, deixamos vazio ou usamos __attribute__((visibility("default"))))
#define XNPP_API
#endif


#endif