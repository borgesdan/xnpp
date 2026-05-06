#include "Xna/ContentPipeline/Processors/FontDescriptionProcessor.hpp"
#include "Xna/ContentPipeline/Graphics/PixelBitmapContent.hpp"
#include <Xna/Framework/Color.hpp>
#include "Xna/ContentPipeline/Graphics/DxtBitmapContent.hpp"
#include <algorithm>
#include <limits>
#include <cstdlib> // Para std::getenv
#include <ft2build.h>
#include FT_FREETYPE_H

namespace Xna {
    static PixelBitmapContent<Color> RasterizeFreeType(FT_Face face, uint32_t ch, int32_t& characterY, int32_t& characterHeight);
    static void CropGlyphs(const std::vector<PixelBitmapContent<Color>>& bitmaps, std::vector<Rectangle>& cropping, const std::vector<int>& yValues, const std::vector<int>& heights, int lineSpacing);
    static PixelBitmapContent<Color> ArrangeGlyphs(const std::vector<PixelBitmapContent<Color>>& bitmaps, std::vector<Rectangle>& glyphRects);
    static std::unique_ptr<Dxt3BitmapContent> CompressFontTexture(PixelBitmapContent<Color> const& source);
    static void CompressFontTextureBlock(PixelBitmapContent<Color> const& source, int32_t blockX, int32_t blockY, std::vector<uint8_t>& outputData, int32_t outputPosition);   


    static std::filesystem::path CheckFontPath(FontDescription const& input) {
        // 1. Prepara o nome do arquivo da fonte com a extensão correta
        std::filesystem::path requestedFont = input.FontName;        

        // 2. Se for um caminho absoluto, apenas verifica se existe e retorna
        if (requestedFont.is_absolute()) {
            return requestedFont.lexically_normal();
        }

        // 3. Define a lista de diretórios de busca baseados no Sistema Operacional
        std::vector<std::filesystem::path> searchPaths;

#if defined(_WIN32) || defined(_WIN64)
        // Windows: Tenta pegar a pasta do sistema da variável de ambiente, com fallback seguro
        if (const char* windir = std::getenv("windir")) {
            searchPaths.push_back(std::filesystem::path(windir) / "Fonts");
        }
        else {
            searchPaths.push_back("C:/Windows/Fonts");
        }
        // Windows: Fontes instaladas apenas para o usuário local (Windows 10+)
        if (const char* localappdata = std::getenv("LOCALAPPDATA")) {
            searchPaths.push_back(std::filesystem::path(localappdata) / "Microsoft" / "Windows" / "Fonts");
        }

#elif defined(__APPLE__) && defined(__MACH__)
        // macOS: Fontes do sistema e fontes globais
        searchPaths.push_back("/System/Library/Fonts");
        searchPaths.push_back("/Library/Fonts");
        // macOS: Fontes do usuário local
        if (const char* home = std::getenv("HOME")) {
            searchPaths.push_back(std::filesystem::path(home) / "Library" / "Fonts");
        }

#elif defined(__linux__)
        // Linux: Fontes do sistema
        searchPaths.push_back("/usr/share/fonts");
        searchPaths.push_back("/usr/local/share/fonts");
        // Linux: Fontes do usuário local
        if (const char* home = std::getenv("HOME")) {
            searchPaths.push_back(std::filesystem::path(home) / ".local" / "share" / "fonts");
            searchPaths.push_back(std::filesystem::path(home) / ".fonts"); // Diretório antigo, mas ainda comum
        }
#else
        // Fallback para sistemas desconhecidos
        searchPaths.push_back("./fonts");
#endif

        // 4. Itera sobre os diretórios para encontrar a fonte
        for (const auto& basePath : searchPaths) {
            std::filesystem::path fullPath1 = basePath / requestedFont;
            std::filesystem::path fullPath2 = basePath / requestedFont;

            if (!fullPath1.has_extension()) {
                fullPath1 = fullPath1.replace_extension(".ttf");
                fullPath2 = fullPath2.replace_extension(".otf");
            }

            // Verifica se o arquivo realmente existe neste diretório
            if (std::filesystem::exists(fullPath1)) {
                return fullPath1.lexically_normal();
            }else if (std::filesystem::exists(fullPath2))
                return fullPath2.lexically_normal();
        }

        // 5. Fallback: Se a fonte não for encontrada em lugar nenhum,
        // retorna o caminho combinando o primeiro diretório da lista.
        // O sistema que chamar essa função terá que lidar com a ausência do arquivo.
        std::filesystem::path fallbackPath = searchPaths.empty() ? std::filesystem::path(".") : searchPaths.front();
        fallbackPath /= requestedFont;

        return fallbackPath.lexically_normal();
    }

    static void ReleaseFontLibrary(FT_Library ft, FT_Face face) {
        if(face)
            FT_Done_Face(face);

        if(ft)
            FT_Done_FreeType(ft);
    }    

	SpriteFontContent FontDescriptionProcessor::ProcessT(FontDescription& input, ContentProcessorContext& context) {
        auto charList = input.Characters.CharList();
        
        if (input.DefaultCharacter && !input.Characters.Contains(*input.DefaultCharacter))
            charList.push_back(*input.DefaultCharacter);

        std::sort(charList.begin(), charList.end());
        SpriteFontContent spriteFontContent{};

        FT_Library ft;
        FT_Face face;        
        
        if (FT_Init_FreeType(&ft))
            throw std::runtime_error("FT_Init_FreeType failed");        
        
        std::filesystem::path fontPath = CheckFontPath(input);

        if (FT_New_Face(ft, fontPath.string().c_str(), 0, &face)) {
            ReleaseFontLibrary(ft, nullptr);
            throw std::runtime_error("FT_New_Face failed");
        }

        if (FT_Set_Pixel_Sizes(face, 0, static_cast<FT_UInt>(input.Size))) {
            ReleaseFontLibrary(ft,face);
            throw std::runtime_error("FT_Set_Pixel_Sizes failed");
        }

        std::vector<int32_t> characterYValues;
        std::vector<int32_t> characterHeights;
        std::vector<PixelBitmapContent<Color>> pixelBitmapContentList;

        // FreeType usa unidades 26.6 (fixed point), então dividimos por 64 (>> 6)
        // O Math.Ceiling do C# é replicado garantindo que arredondamos para cima.
        const auto lineSpacing = static_cast<int>(std::ceil(face->size->metrics.height / 64.0f));
        
        for (const auto& ch : charList) {
            // 1. Carregar o Glifo (Equivalente ao fontRasterizer.Rasterize)
            // FT_LOAD_RENDER já transforma o glifo em um bitmap de tons de cinza
            if (FT_Load_Char(face, ch, FT_LOAD_RENDER)) {
                continue; //TODO: [!] continue ou exception?
            }

            FT_GlyphSlot slot = face->glyph;

            // 2. Mapeamento de Caractere
            spriteFontContent.CharacterMap.push_back(ch);

            // 3. Extração das Métricas ABC (Equivalente ao kerningHelper.GetCharacterSpacing)
            // FreeType usa unidades de 1/64 de pixel, por isso o shift >> 6 (ou x/64)
            const auto a = static_cast<float>(slot->metrics.horiBearingX >> 6);
            const auto b = static_cast<float>(slot->metrics.width >> 6);
            const auto advance = static_cast<float>(slot->metrics.horiAdvance >> 6);
            const auto c = advance - (a + b);

            spriteFontContent.Kerning.push_back(Vector3(a, b, c));

            // 4. Rasterização e Bitmaps (Preparação para o GlyphPacker)
            // Armazenamos o bitmap temporariamente para o Packer organizar depois
            // slot->bitmap.buffer contém os pixels (8-bit greyscale)

            // Simulação do Rasterize(ch, out y, out height):
            int32_t characterY = slot->bitmap_top; // Onde o glifo começa em relação à linha de base
            int32_t characterHeight = slot->bitmap.rows;

            // Gera o bitmap já "recortado" e as métricas
            auto glyphBitmap = RasterizeFreeType(face, ch, characterY, characterHeight);
            pixelBitmapContentList.push_back(glyphBitmap);

            characterYValues.push_back(characterY);
            characterHeights.push_back(characterHeight);            
        }        
        
        CropGlyphs(pixelBitmapContentList, spriteFontContent.Cropping, characterYValues, characterHeights, lineSpacing);

        for (size_t i = 0; i < charList.size(); ++i) {
            Vector3& kerning = spriteFontContent.Kerning[i];

            // Se não usar Kerning, o valor 'B' (Y no Vector3) vira a largura total do recorte
            if (!input.UseKerning) {
                kerning.Y = static_cast<float>(spriteFontContent.Cropping[i].Width);
                kerning.X = 0.0f; // Sem margem esquerda
                kerning.Z = 0.0f; // Sem margem direita
            }
            // [!]: O FreeType sempre contém o caractere se o LoadChar funcionou, 
            // então a verificação FontContainsCharacter é implícita.
        }

        auto source = ArrangeGlyphs(pixelBitmapContentList, spriteFontContent.Glyphs);
        auto bitmap = CompressFontTexture(source);
        auto mipmap = MipmapChain(std::move(bitmap));
        spriteFontContent.Texture.Mipmaps(mipmap);
        spriteFontContent.LineSpacing = lineSpacing;

        ReleaseFontLibrary(ft, face);

        spriteFontContent.Spacing = input.Spacing;
        spriteFontContent.DefaultCharacter = input.DefaultCharacter;
        return spriteFontContent;
	}    

    static PixelBitmapContent<Color> RasterizeFreeType(FT_Face face, uint32_t ch, int32_t& characterY, int32_t& characterHeight) {
        // 1. Renderiza o glifo
        FT_Load_Char(face, ch, FT_LOAD_RENDER);
        FT_GlyphSlot slot = face->glyph;

        // 2. Métrica de Altura e Y (Equivalente ao que o XNA extraía no Rasterize)
        // No FreeType, rows é a altura real do desenho
        characterHeight = slot->bitmap.rows;
        // bitmap_top é a distância do topo do glifo até a linha de base
        characterY = slot->bitmap_top;

        int width = slot->bitmap.width;
        int height = slot->bitmap.rows;

        PixelBitmapContent<Color> xna(width, height);

        // 3. Converter o bitmap de 8-bits do FreeType para o Color (RGBA) do XNA
        // O FreeType gera um buffer de tons de cinza (0-255)
        for (size_t y = 0; y < height; ++y) {
            for (size_t x = 0; x < width; ++x) {
                // No FreeType, o buffer é um array linear
                uint8_t alpha = slot->bitmap.buffer[y * slot->bitmap.pitch + x];

                // O XNA SpriteFont por padrão usa cores brancas com o valor do alpha
                // vindo da rasterização (ou R=G=B=A como no seu código C# original)
                Color color;
                color.R(255);
                color.G(255);
                color.B(255);
                color.A(alpha);                

                xna.SetPixel(x, y, color);
            }
        }        

        return xna;
    }

    static void CropGlyphs(const std::vector<PixelBitmapContent<Color>>& bitmaps, std::vector<Rectangle>& cropping,
        const std::vector<int>& yValues, const std::vector<int>& heights, int lineSpacing) {
        for (size_t i = 0; i < bitmaps.size(); ++i) {
            Rectangle rect;
            rect.X = 0;
            rect.Width = bitmaps[i].Width;

            // No XNA, o Y do Cropping é a distância do topo da linha até o topo do glifo
            // O FreeType nos dá o yValue (bitmap_top), que é a distância da baseline para cima.
            // Portanto: Y = (Distância da Baseline ao topo) - bitmap_top
            // Usaremos uma aproximação baseada no LineSpacing para centralizar.
            int ascent = lineSpacing; // Simplificação: use o valor de 'ascent' da fonte se disponível
            rect.Y = ascent - yValues[i];

            rect.Height = heights[i];

            cropping.push_back(rect);
        }
    }

    static PixelBitmapContent<Color> ArrangeGlyphs(const std::vector<PixelBitmapContent<Color>>& bitmaps, std::vector<Rectangle>& glyphRects) {
        // 1. Decidir o tamanho da textura (ex: 512x512 ou 1024x1024)
        int atlasWidth = 512;
        int atlasHeight = 512;
        PixelBitmapContent<Color> atlas(atlasWidth, atlasHeight);

        int32_t currentX = 0;
        int32_t currentY = 0;
        int32_t maxHeightInRow = 0;

        for (const auto& bitmap : bitmaps) {
            // Se estourar a largura, pula para a próxima "prateleira" (shelf)
            if (currentX + bitmap.Width > atlasWidth) {
                currentX = 0;
                currentY += maxHeightInRow + 1;
                maxHeightInRow = 0;
            }

            // Armazena a posição onde o glifo foi colocado
            Rectangle r = { currentX, currentY, static_cast<int32_t>(bitmap.Width), static_cast<int32_t>(bitmap.Height) };
            glyphRects.push_back(r);

            // Copia os pixels do bitmap individual para o atlas
            //atlas.Blit(bitmap, currentX, currentY);
            atlas.BlitFast(bitmap, currentX, currentY);

            currentX += bitmap.Width + 1; // +1 para evitar sangramento de pixels (bleeding)
            maxHeightInRow = std::max(static_cast<size_t>(maxHeightInRow), bitmap.Height);
        }

        return atlas;
    }

    static std::unique_ptr<Dxt3BitmapContent> CompressFontTexture(PixelBitmapContent<Color> const& source) {
        std::vector<uint8_t> numArray;
        numArray.resize(source.Width * source.Height);
        int32_t outputPosition = 0;
        for (int32_t blockY = 0; blockY < source.Height; blockY += 4) {
            for (int32_t blockX = 0; blockX < source.Width; blockX += 4) {
                CompressFontTextureBlock(source, blockX, blockY, numArray, outputPosition);
                outputPosition += 16 /*0x10*/;
            }
        }
        
        auto dxt3BitmapContent = std::make_unique<Dxt3BitmapContent>(source.Width, source.Height);
        dxt3BitmapContent->SetPixelData(numArray.data(), numArray.size());
        return dxt3BitmapContent;
    }

    static void CompressFontTextureBlock(PixelBitmapContent<Color> const& source, int32_t blockX, int32_t blockY, std::vector<uint8_t>& outputData, int32_t outputPosition) {
        int64_t num1 = 0;
        int32_t num2 = 0;
        int32_t num3 = 0;
        for (size_t index1 = 0; index1 < 4; ++index1)
        {
            for (size_t index2 = 0; index2 < 4; ++index2)
            {
                auto a = static_cast<int32_t>(source.GetPixel(blockX + index2, blockY + index1).A());
                int64_t num4;
                int32_t num5;
                if (a < 42) {
                    num4 = 0L;
                    num5 = 1;
                }
                else if (a < 128 /*0x80*/) {
                    num4 = 5L;
                    num5 = 3;
                }
                else if (a < 213) {
                    num4 = 10L;
                    num5 = 2;
                }
                else {
                    num4 = 15L;
                    num5 = 0;
                }
                num1 |= num4 << num3 * 4;
                num2 |= num5 << num3 * 2;
                ++num3;
            }
        }
        for (int32_t index = 0; index < 8; ++index)
            outputData[outputPosition + index] = static_cast<uint8_t>(num1 >> index * 8);

        outputData[outputPosition + 8] = 255; //byte.MaxValue
        outputData[outputPosition + 9] = 255; //byte.MaxValue
        outputData[outputPosition + 10] = 0;
        outputData[outputPosition + 11] = 0;
        
        for (int32_t index = 0; index < 4; ++index)
            outputData[outputPosition + 12 + index] = static_cast<uint8_t>(num2 >> index * 8);
    }
}