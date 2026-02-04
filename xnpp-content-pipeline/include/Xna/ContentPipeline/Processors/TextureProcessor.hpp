#ifndef XNA_CONTENTPIPELINE_PROCESSORS_TEXTUREPROCESSOR_HPP
#define XNA_CONTENTPIPELINE_PROCESSORS_TEXTUREPROCESSOR_HPP

#include "../ContentProcessor.hpp"
#include "../Graphics/Texture2DContent.hpp"
#include "../Graphics/TextureContent.hpp"
#include "Shared.hpp"
#include "Xna/ContentPipeline/Graphics/PixelBitmapContent.hpp"
#include <limits>
#include <Xna/CSharp/Exception.hpp>
#include <Xna/Framework/Color.hpp>

namespace Xna {
    struct TextureProcessorUtils final {
        static void ColorKey(TextureContent& texture, Color const& colorKey);
        static void PremultiplyAlpha(TextureContent& texture);
    };
}

namespace Xna {
    template <typename T1, typename T2>
    struct TextureProcessorBase : public ContentProcessor<T1, T2> {
        virtual ~TextureProcessorBase() = default;

        virtual bool GenerateMipMaps() const noexcept { return generateMipmaps; }
        virtual TextureProcessorOutputFormat TextureFormat() const noexcept { return textureFormat; }
        virtual bool ColorKeyEnabled() const noexcept { return colorKeyEnabled; }
        virtual Color ColorKeyColor() const noexcept { return colorKeyColor; }
        virtual bool ResizeToPowerOfTwo() const noexcept { return resizeToPowerOfTwo; }
        virtual bool PremultiplyAlpha() const noexcept { return premultiplyAlpha; }

        void GenerateMipMaps(bool value) { generateMipmaps = value; }
        void ColorKeyEnabled(bool value) { colorKeyEnabled = value; }
        void PremultiplyAlpha(bool value) { premultiplyAlpha = value; }
        void ResizeToPowerOfTwo(bool value) { resizeToPowerOfTwo = value; }
        void TextureFormat(TextureProcessorOutputFormat value) { textureFormat = value; }
        void ColorKeyColor(Color const& value) { colorKeyColor = value; }

        template <typename TTextureContent>
        void ProcessContent(TTextureContent& input, ContentProcessorContext& context) {
            if (input == nullptr)
                throw CSharp::ArgumentNullException("input");

            const auto colorKeyEnabled = ColorKeyEnabled();
            const auto premultiplyAlpha = PremultiplyAlpha();

            if (colorKeyEnabled)
                TextureProcessorUtils::ColorKey(input, colorKeyColor);

            if (premultiplyAlpha)
                TextureProcessorUtils::PremultiplyAlpha(input);
        }

    protected:
        bool generateMipmaps{ false };
        TextureProcessorOutputFormat textureFormat{ TextureProcessorOutputFormat::Color };
        bool colorKeyEnabled{ true };
        Color colorKeyColor{ Color::Magenta() };
        bool resizeToPowerOfTwo{ false };
        bool premultiplyAlpha{ true };
    };

	struct TextureProcessor : public TextureProcessorBase<TextureContent, TextureContent> {
        ~TextureProcessor() override = default;       

        TextureContent ProcessT(TextureContent& input, ContentProcessorContext& context) override;
	};

    struct Texture2DProcessor : public TextureProcessorBase<Texture2DContent, Texture2DContent> {
        using Base = TextureProcessorBase<Texture2DContent, Texture2DContent>;
        
        ~Texture2DProcessor() override = default;        

        Texture2DContent ProcessT(Texture2DContent& input, ContentProcessorContext& context) override;
    };
}


#endif