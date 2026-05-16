#ifndef XNA_MACROS_HPP
#define XNA_MACROS_HPP

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

//Export
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

//OS
#if defined(_WIN32)
#define PLATFORM_WINDOWS 1
#elif defined(__APPLE__) && defined(__MACH__)
#define PLATFORM_MACOS 1
#elif defined(__ANDROID__)
#define PLATFORM_ANDROID 1
#elif defined(__linux__)
#define PLATFORM_LINUX 1
#elif defined(__unix__)
#define PLATFORM_UNIX 1
#else
#error "Sistema operacional não suportado"
#endif
#ifndef PLATFORM_WINDOWS
#define PLATFORM_WINDOWS 0
#endif
#ifndef PLATFORM_MACOS
#define PLATFORM_MACOS 0
#endif
#ifndef PLATFORM_LINUX
#define PLATFORM_LINUX 0
#endif
#ifndef PLATFORM_ANDROID
#define PLATFORM_ANDROID 0
#endif
#ifndef PLATFORM_UNIX
#define PLATFORM_UNIX 0
#endif

//Architecture
#if defined(_M_X64) || defined(__x86_64__)
#define ARCH_X64 1
#elif defined(_M_IX86) || defined(__i386__)
#define ARCH_X86 1
#elif defined(_M_ARM64) || defined(__aarch64__)
#define ARCH_ARM64 1
#elif defined(_M_ARM) || defined(__arm__)
#define ARCH_ARM 1
#else
#error "Arquitetura não suportada"
#endif

#ifndef ARCH_X64
#define ARCH_X64 0
#endif
#ifndef ARCH_X86
#define ARCH_X86 0
#endif
#ifndef ARCH_ARM64
#define ARCH_ARM64 0
#endif
#ifndef ARCH_ARM
#define ARCH_ARM 0
#endif

//Compiler

#if defined(_MSC_VER)
#define COMPILER_MSVC 1
#elif defined(__clang__)
#define COMPILER_CLANG 1
#elif defined(__GNUC__)
#define COMPILER_GCC 1
#else
#error "Compilador não suportado"
#endif

#ifndef COMPILER_MSVC
#define COMPILER_MSVC 0
#endif
#ifndef COMPILER_CLANG
#define COMPILER_CLANG 0
#endif
#ifndef COMPILER_GCC
#define COMPILER_GCC 0
#endif

//Endianess
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define PLATFORM_BIG_ENDIAN 1
#define PLATFORM_LITTLE_ENDIAN 0
#else
#define PLATFORM_BIG_ENDIAN 0
#define PLATFORM_LITTLE_ENDIAN 1
#endif

//Util
#if PLATFORM_WINDOWS
#define FORCE_INLINE __forceinline
#elif COMPILER_GCC || COMPILER_CLANG
#define FORCE_INLINE inline __attribute__((always_inline))
#else
#define FORCE_INLINE inline
#endif

#endif