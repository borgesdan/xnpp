#include "Xna/CSharp/BitConverter.hpp"
#include "Xna/CSharp/Exception.hpp"
#include <bit>

namespace Xna::CSharp {
    int32_t BitConverter::ToInt32(const uint8_t *value, size_t valueSize, size_t startIndex) {
        if (!value)
            throw CSharp::ArgumentNullException("value");

        if (startIndex >= valueSize)
            throw CSharp::ArgumentOutOfRangeException("startIndex >= valueSize");

        if (startIndex > valueSize - 4)
            throw CSharp::ArgumentException("startIndex > valueSize - 4");

        auto ptr = value + startIndex;

        //if (startIndex % 4 == 0)
        if ((startIndex & 0x3) == 0)
        {
            int32_t result;
            std::memcpy(&result, ptr, sizeof(int32_t));
            return result;
        }

        if constexpr (std::endian::native == std::endian::little)
        {
            return  static_cast<int32_t>( ptr[0]
                  | (ptr[1] << 8)
                  | (ptr[2] << 16)
                  | (ptr[3] << 24));
        }

        return  static_cast<int32_t>((ptr[0] << 24)
              | (ptr[1] << 16)
              | (ptr[2] << 8)
              |  ptr[3]);
    }
}
