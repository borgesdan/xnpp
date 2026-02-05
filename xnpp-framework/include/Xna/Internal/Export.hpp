#ifndef XNA_EXPORT_HPP
#define XNA_EXPORT_HPP

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