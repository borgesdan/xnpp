#pragma once

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