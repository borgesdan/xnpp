#ifndef XNA_CSHARP_BITCONVERTER_HPP
#define XNA_BITCONVERTER_HPP

#include <cstdint>

namespace Xna::CSharp {
    struct BitConverter final {
    public:
        static int32_t ToInt32(const uint8_t* value, size_t valueSize, size_t startIndex);

    public:
        BitConverter() = delete;
        ~BitConverter() = delete;
        BitConverter(const BitConverter&) = delete;
        BitConverter& operator=(const BitConverter&) = delete;
        BitConverter(BitConverter&&) = delete;
        BitConverter& operator=(BitConverter&&) = delete;
    };
}


#endif