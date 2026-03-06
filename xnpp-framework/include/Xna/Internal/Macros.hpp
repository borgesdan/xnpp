#ifndef XNA_MACROS_HPP
#define XNA_MACROS_HPP

#define XNPP_DECLARE_IMPL_WRAPPER(ClassName, Impl) \
    inline ClassName(std::nullptr_t) { Impl = nullptr; } \
    inline bool operator==(ClassName const& other) const noexcept { return Impl == other.Impl; } \
    inline bool operator==(std::nullptr_t) const noexcept { return Impl == nullptr; } \
    inline explicit operator bool() const noexcept { return Impl != nullptr; }


#endif