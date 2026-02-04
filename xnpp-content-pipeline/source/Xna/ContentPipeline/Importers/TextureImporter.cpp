#include "Xna/ContentPipeline/Importers/TextureImporter.hpp"
#include "Xna/ContentPipeline/Graphics/Texture2DContent.hpp"
#include "Xna/ContentPipeline/Graphics/PixelBitmapContent.hpp"
#include <stdexcept>
#include <Xna/Framework/Color.hpp>
#include <cstdint>
#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace fs = std::filesystem;

namespace Xna {
    Texture2DContent Texture2DImporter::ImportT(std::filesystem::path const& file, ContentImporterContext& context) {
        int width;
        int height;
        int channels;

        stbi_uc* pixels = stbi_load(
            file.string().c_str(),
            &width, &height,
            &channels,
            STBI_rgb_alpha
        );

        if (!pixels)
            throw CSharp::InvalidOperationException("stbi_load failed.");

        auto bitmapContent = std::make_unique<PixelBitmapContent<Color>>(width, height);
        const auto pixelCount = static_cast<size_t>(width) * height;
        auto& source = bitmapContent->Source();        

        for (size_t i = 0; i < pixelCount; ++i) {
            const uint8_t r = pixels[i * 4 + 0];
            const uint8_t g = pixels[i * 4 + 1];
            const uint8_t b = pixels[i * 4 + 2];
            const uint8_t a = pixels[i * 4 + 3];

            source[i] = Color(r, g, b, a);
        }      

        stbi_image_free(pixels);

        auto texture2Dcontent = Texture2DContent();
        texture2Dcontent.Mipmaps().Add(std::move(bitmapContent));        

        return texture2Dcontent;
    }
}