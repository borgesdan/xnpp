#include "Xna/Framework/Content/Readers/Texture2DReader.hpp"
#include "../../Graphics/GraphicsContentHelper.hpp"

namespace Xna {
    Texture2D Texture2DReader::Read(ContentReader& input, Texture2D& existingInstance) {
        SurfaceFormat format = static_cast<SurfaceFormat>(input.ReadInt32());
        const auto width = input.ReadInt32();
        const auto height = input.ReadInt32();
        const auto num1 = input.ReadInt32();
        auto texture2D = Texture2D(GraphicsContentHelper::GraphicsDeviceFromContentReader(input), width, height, num1 > 1, format);

        for (size_t level = 0; level < num1; ++level)
        {
            const auto num2 = input.ReadInt32();
            auto data = input.ReadByteBuffer(num2);
            texture2D.SetData(level, std::nullopt, *data, 0, num2);
        }

        return texture2D;
    }
}