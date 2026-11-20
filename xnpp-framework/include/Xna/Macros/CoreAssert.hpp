#pragma once

#include "Xna/Log.hpp"

#ifdef _DEBUG
#if defined(_MSC_VER)
#define XNPP_CORE_DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
#define XNPP_CORE_DEBUG_BREAK() __builtin_trap()
#else
#include <csignal>
#define XNPP_CORE_DEBUG_BREAK() raise(SIGTRAP)
#endif

// Asserção interna da Engine (Core)
#define XNPP_CORE_ASSERT(condition, ...) \
        do { \
            if (!(condition)) { \
                XNPP_ENGINE_CRITICAL("Falha de Asserção: {0}", __VA_ARGS__); \
                XNPP_CORE_DEBUG_BREAK(); \
            } \
        } while(false)

    // Asserção para lógica de Jogo (Client)
#define XNPP_CLIENT_ASSERT(condition, ...) \
        do { \
            if (!(condition)) { \
                XNPP_CLIENT_CRITICAL("Falha de Asserção no Jogo: {0}", __VA_ARGS__); \
                XNPP_CORE_DEBUG_BREAK(); \
            } \
        } while(false)
#else
#define XNPP_CORE_DEBUG_BREAK()
#define XNPP_CORE_ASSERT(condition, ...) do { } while(false)
#define XNPP_CLIENT_ASSERT(condition, ...) do { } while(false)
#endif

