#pragma once

#define XNPP_NULLABLE_CLASS(ClassName, Impl) \
    ClassName(std::nullptr_t) { Impl = nullptr; } \
    bool operator==(ClassName const& other) const noexcept { return Impl == other.Impl; } \
    bool operator==(std::nullptr_t) const noexcept { return Impl == nullptr; } \
    explicit operator bool() const noexcept { return Impl != nullptr; }

#define XNPP_NULLABLE_CLASS_WITH_BASE(ClassName, BaseClassName, Impl) \
    ClassName(std::nullptr_t) : BaseClassName(nullptr) { Impl = nullptr; } \
    bool operator==(ClassName const& other) const noexcept { return Impl == other.Impl; } \
    bool operator==(std::nullptr_t) const noexcept { return Impl == nullptr; } \
    explicit operator bool() const noexcept { return Impl != nullptr; }

#define XNPP_STATIC_CLASS(ClassName) \
public: \
    ClassName() = delete; \
    ClassName(ClassName const&) = delete; \
    ClassName(ClassName&&) = delete; \
    ClassName& operator=(ClassName const&) = delete; \
    ClassName& operator=(ClassName&&) = delete;