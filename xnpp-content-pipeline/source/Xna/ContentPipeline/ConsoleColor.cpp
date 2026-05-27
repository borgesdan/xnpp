#include "Xna/ContentPipeline/ConsoleColor.hpp"

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

namespace Xna {
    void ConsoleColor::EnableVirtualTerminal() {
#ifdef _WIN32
        // Habilita as cores no std::cout
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut != INVALID_HANDLE_VALUE) {
            DWORD dwMode = 0;
            if (GetConsoleMode(hOut, &dwMode)) {
                SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
            }
        }

        // Habilita as cores no std::cerr
        HANDLE hErr = GetStdHandle(STD_ERROR_HANDLE);
        if (hErr != INVALID_HANDLE_VALUE) {
            DWORD dwMode = 0;
            if (GetConsoleMode(hErr, &dwMode)) {
                SetConsoleMode(hErr, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
            }
        }
#endif
    }
}