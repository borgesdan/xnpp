#ifndef XNA_CONTENT_READERS_TEXTURE2DREADER_HPP
#define XNA_CONTENT_READERS_TEXTURE2DREADER_HPP

#include "../ContentTypeReader.hpp"
#include "../ContentReader.hpp"
#include "Xna/Framework/Graphics/Texture2D.hpp"
#include "Xna/Framework/Graphics/GraphicsContentHelper.hpp"

namespace Xna {
	struct Texture2DReader : public ContentTypeReaderT<Texture2D> {
	protected:
		inline Texture2D Read(ContentReader& input, Texture2D& existingInstance) override {
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
	};
}

#endif