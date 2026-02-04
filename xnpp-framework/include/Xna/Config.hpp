#ifndef MYLIB_CONFIG_H
#define MYLIB_CONFIG_H

// --- Identificação do Sistema Operacional ---
#if defined(_WIN32) || defined(__CYGWIN__)
    // No Windows, usamos declspec
#ifdef MYLIB_EXPORT_DLL
#define MYLIB_API __declspec(dllexport)
#else
#define MYLIB_API __declspec(dllimport)
#endif
#define MYLIB_LOCAL 
#else
    // No Linux/Unix/macOS, usamos visibilidade do GCC/Clang
#if __GNUC__ >= 4
#define MYLIB_API __attribute__ ((visibility ("default")))
#define MYLIB_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define MYLIB_API
#define MYLIB_LOCAL
#endif
#endif

#endif