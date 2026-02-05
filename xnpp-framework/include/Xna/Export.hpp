#ifndef XNA_EXPORT_HPP
#define XNA_EXPORT_HPP

#ifdef _WIN32
#ifdef XNPP_EXPORT_DLL
// Quando compilando a biblioteca, exportamos
#define XNPP_API __declspec(dllexport)
#else
// Quando o usuário da lib inclui o header, ele importa
#define XNPP_API __declspec(dllimport)
#endif
#else
// Para outros sistemas (Linux/macOS)
#define XNPP_API
#endif

#endif