#pragma once

#define XNPP_DECLARE_NULL_IMPL_WRAPPER(ClassName, Impl) \
    inline ClassName(std::nullptr_t) { Impl = nullptr; } \
    inline bool operator==(ClassName const& other) const noexcept { return Impl == other.Impl; } \
    inline bool operator==(std::nullptr_t) const noexcept { return Impl == nullptr; } \
    inline explicit operator bool() const noexcept { return Impl != nullptr; }

#define XNPP_DECLARE_NULL_IMPL_BASE_WRAPPER(ClassName, BaseClassName, Impl) \
    inline ClassName(std::nullptr_t) : BaseClassName(nullptr) { Impl = nullptr; } \
    inline bool operator==(ClassName const& other) const noexcept { return Impl == other.Impl; } \
    inline bool operator==(std::nullptr_t) const noexcept { return Impl == nullptr; } \
    inline explicit operator bool() const noexcept { return Impl != nullptr; }