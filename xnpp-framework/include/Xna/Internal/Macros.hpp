#ifndef XNA_MACROS_HPP
#define XNA_MACROS_HPP

#define XNPP_DECLARE_IMPL_WRAPPER(ClassName, Impl) \
    inline ClassName(std::nullptr_t) { Impl = nullptr; } \
    inline bool operator==(ClassName const& other) const noexcept { return Impl == other.Impl; } \
    inline bool operator==(std::nullptr_t) const noexcept { return Impl == nullptr; } \
    inline explicit operator bool() const noexcept { return Impl != nullptr; }

#ifdef _WIN32
#ifdef XNPP_EXPORT_DLL
#define XNPP_API __declspec(dllexport)
#else
#define XNPP_API __declspec(dllimport)
#endif
#else
#define XNPP_API
#endif


#endif