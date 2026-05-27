#ifndef XNA_CONTENTPIPELINE_CONSOLECOLOR_HPP
#define XNA_CONTENTPIPELINE_CONSOLECOLOR_HPP

namespace Xna {
    struct ConsoleColor {
        // Códigos ANSI padrão
        static constexpr const char* Reset = "\033[0m";
        static constexpr const char* Red = "\033[31m";
        static constexpr const char* Green = "\033[32m";
        static constexpr const char* Yellow = "\033[33m";
        static constexpr const char* Blue = "\033[34m";
        static constexpr const char* Magenta = "\033[35m";
        static constexpr const char* Cyan = "\033[36m";
        static constexpr const char* White = "\033[37m";

        // Versões em negrito/brilhantes
        static constexpr const char* BoldRed = "\033[1;31m";
        static constexpr const char* BoldYellow = "\033[1;33m";
        static constexpr const char* BoldGreen = "\033[1;32m";

        // Função para habilitar as cores no Windows (no Linux/Mac ela fica vazia e é ignorada)
        static void EnableVirtualTerminal();
    };    
}

#endif