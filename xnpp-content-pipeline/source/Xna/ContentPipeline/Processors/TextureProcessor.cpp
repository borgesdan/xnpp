#include "Xna/ContentPipeline/Processors/TextureProcessor.hpp"

namespace Xna {
    TextureContent TextureProcessor::ProcessT(TextureContent& input, ContentProcessorContext& context) {
        ProcessContent(input, context);
        return input;
    }

    Texture2DContent Texture2DProcessor::ProcessT(Texture2DContent& input, ContentProcessorContext& context) {
        ProcessContent(input, context);
        return input;
    }

    void TextureProcessorUtils::ColorKey(TextureContent& texture, Color const& colorKey)
    {
        for(auto& face : texture.Faces())
        {
            for(auto& bitmapContent : face)
            {
                auto pixelBitmapContent1 = reinterpret_pointer_cast<PixelBitmapContent<Color>>(bitmapContent);

                if (pixelBitmapContent1) {
                    pixelBitmapContent1->ReplaceColor(colorKey, Color::Transparent());
                    continue;
                }
                
                auto pixelBitmapContent2 = reinterpret_pointer_cast<PixelBitmapContent<Vector4>>(bitmapContent);

                if (pixelBitmapContent2) {
                    pixelBitmapContent2->ReplaceColor(colorKey.ToVector4(), Vector4::Zero());
                    continue;
                }

                throw std::runtime_error("BitmapContent type not suported.");
            }
        }
    }

    void TextureProcessorUtils::PremultiplyAlpha(TextureContent& texture) {
        for(auto& face : texture.Faces()) {
            for(auto& bitmapContent : face) {
                auto pixelBitmapContent1 = reinterpret_pointer_cast<PixelBitmapContent<Color>>(bitmapContent);

                if (pixelBitmapContent1) {                   
                    auto& source = pixelBitmapContent1->Source();
                    for (auto& color : source) {
                        color = Color::FromNonPremultiplied(
                            static_cast<int32_t>(color.R()),
                            static_cast<int32_t>(color.G()),
                            static_cast<int32_t>(color.B()),
                            static_cast<int32_t>(color.A()));
                    }
                    continue;
                }

                auto pixelBitmapContent2 = reinterpret_pointer_cast<PixelBitmapContent<Vector4>>(bitmapContent);

                if (pixelBitmapContent2) {
                    auto& source = pixelBitmapContent2->Source();
                    for (auto& vector4 : source) {
                        if (vector4.W < 1.0F) {
                            vector4.X *= vector4.W;
                            vector4.Y *= vector4.W;
                            vector4.Z *= vector4.W;
                        }
                    }

                    continue;
                }

                throw std::runtime_error("BitmapContent type not suported.");
            }
        }
    }
}