#include "Xna/Platform/_Platform.hpp"
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/math.h>
#include <bx/debug.h>
#include <stdexcept>

#include "Xna/Framework/Graphics/SpriteBatch.hpp"

namespace Xna {
	struct SpriteVertex {
		float x, y, z;
		uint32_t color;
		float u, v;

		static void init() {
			ms_layout
				.begin()
				.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
				.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
				.end();
		}

		static bgfx::VertexLayout ms_layout;
	};

	bgfx::VertexLayout SpriteVertex::ms_layout;

	struct Sprite {
		float x, y, width, height;
		float u1, v1, u2, v2;
		uint32_t color;
		bgfx::TextureHandle texture;
	};

	// Estrutura para representar um retângulo (já que bgfx não tem Rect)
	struct Rect {
		int32_t x, y;
		int32_t width, height;

		Rect() : x(0), y(0), width(0), height(0) {}
		Rect(int32_t _x, int32_t _y, int32_t _w, int32_t _h)
			: x(_x), y(_y), width(_w), height(_h) {
		}
	};

	// Estrutura para armazenar informações da textura
	struct TextureInfo {
		uint16_t width;
		uint16_t height;
		bool hasAlpha;
	};

	struct BgfxSpriteBatch final : public PlatformNS::ISpriteBatch {
		BgfxSpriteBatch() : m_beginCalled(false), m_currentSpriteCount(0) {
			SpriteVertex::init();
			m_layout = SpriteVertex::ms_layout;

			// Criar buffers com tamanho máximo
			m_vb = bgfx::createDynamicVertexBuffer(
				bgfx::makeRef(nullptr, 0),
				m_layout,
				BGFX_BUFFER_NONE
			);

			// Índices estáticos (triângulos para quads)
			uint16_t* indices = new uint16_t[kMaxIndices];
			for (uint16_t i = 0; i < kMaxSprites; ++i) {
				uint16_t base = i * 4;
				uint16_t idx = i * 6;
				indices[idx + 0] = base;
				indices[idx + 1] = base + 1;
				indices[idx + 2] = base + 2;
				indices[idx + 3] = base + 2;
				indices[idx + 4] = base + 3;
				indices[idx + 5] = base;
			}

			m_ib = bgfx::createIndexBuffer(
				bgfx::makeRef(indices, kMaxIndices * sizeof(uint16_t))
			);

			delete[] indices;

			// Criar uniform para textura
			m_textureUniform = bgfx::createUniform("s_texture", bgfx::UniformType::Sampler);

			// Criar shaders (você precisará compilar seus próprios shaders)
			m_program = loadShaderProgram("vs_sprite.bin", "fs_sprite.bin");
		}

		void begin() {
			m_beginCalled = true;
			m_currentSpriteCount = 0;
			m_sprites.clear();
			m_currentTexture.idx = bgfx::kInvalidHandle;
		}

		// Overload sem source rect
		void draw(bgfx::TextureHandle texture, float x, float y,
			uint32_t color = 0xffffffff) {
			draw(texture, x, y, 1.0f, 1.0f, nullptr, color);
		}

		// Overload com scale
		void draw(bgfx::TextureHandle texture, float x, float y,
			float scaleX, float scaleY,
			uint32_t color = 0xffffffff) {
			draw(texture, x, y, scaleX, scaleY, nullptr, color);
		}

		// Overload com source rect (usando nossa estrutura Rect)
		void draw(bgfx::TextureHandle texture, float x, float y,
			const Rect* sourceRect,
			uint32_t color = 0xffffffff) {
			draw(texture, x, y, 1.0f, 1.0f, sourceRect, color);
		}

		// Método principal que todos os outros chamam
		void draw(bgfx::TextureHandle texture, float x, float y,
			float scaleX, float scaleY,
			const Rect* sourceRect,
			uint32_t color = 0xffffffff) {
			if (!m_beginCalled) return;

			// Você precisa obter as dimensões reais da textura
			// Por enquanto, vamos usar valores padrão
			TextureInfo texInfo = getTextureInfo(texture);

			float width, height;
			float u1 = 0.0f, v1 = 0.0f, u2 = 1.0f, v2 = 1.0f;

			if (sourceRect) {
				width = static_cast<float>(sourceRect->width);
				height = static_cast<float>(sourceRect->height);

				// Calcular UVs baseado nas dimensões da textura
				if (texInfo.width > 0 && texInfo.height > 0) {
					u1 = sourceRect->x / static_cast<float>(texInfo.width);
					v1 = sourceRect->y / static_cast<float>(texInfo.height);
					u2 = (sourceRect->x + sourceRect->width) / static_cast<float>(texInfo.width);
					v2 = (sourceRect->y + sourceRect->height) / static_cast<float>(texInfo.height);
				}
			}
			else {
				width = static_cast<float>(texInfo.width);
				height = static_cast<float>(texInfo.height);
			}

			Sprite sprite;
			sprite.x = x;
			sprite.y = y;
			sprite.width = width * scaleX;
			sprite.height = height * scaleY;
			sprite.u1 = u1;
			sprite.v1 = v1;
			sprite.u2 = u2;
			sprite.v2 = v2;
			sprite.color = color;
			sprite.texture = texture;

			m_sprites.push_back(sprite);
		}

		void end() {
			if (!m_beginCalled) return;

			if (!m_sprites.empty()) {
				flush();
			}

			m_beginCalled = false;
		}

		~BgfxSpriteBatch() override {
			if (bgfx::isValid(m_vb)) bgfx::destroy(m_vb);
			if (bgfx::isValid(m_ib)) bgfx::destroy(m_ib);
			if (bgfx::isValid(m_textureUniform)) bgfx::destroy(m_textureUniform);
			if (bgfx::isValid(m_program)) bgfx::destroy(m_program);
		}

	private:
		void flush() {
			if (m_sprites.empty()) return;

			std::vector<SpriteVertex> vertices;
			vertices.reserve(m_sprites.size() * 4);

			for (const auto& sprite : m_sprites) {
				addSpriteVertices(vertices, sprite);
			}

			// Atualizar vertex buffer
			bgfx::update(m_vb, 0,
				bgfx::makeRef(vertices.data(),
					vertices.size() * sizeof(SpriteVertex)));

			// Setar estado de renderização
			uint64_t state = BGFX_STATE_WRITE_RGB
				| BGFX_STATE_WRITE_A
				| BGFX_STATE_BLEND_FUNC(
					BGFX_STATE_BLEND_SRC_ALPHA,
					BGFX_STATE_BLEND_INV_SRC_ALPHA);

			// Setar vertex e index buffer
			bgfx::setVertexBuffer(0, m_vb, 0, vertices.size());
			bgfx::setIndexBuffer(m_ib, 0, m_sprites.size() * 6);

			// Setar textura
			bgfx::setTexture(0, m_textureUniform, m_sprites[0].texture);

			// Setar estado de renderização
			bgfx::setState(state);

			// Submeter
			bgfx::submit(0, m_program);

			m_sprites.clear();
		}

		void addSpriteVertices(std::vector<SpriteVertex>& vertices, const Sprite& sprite) {
			float left = sprite.x;
			float right = sprite.x + sprite.width;
			float top = sprite.y;
			float bottom = sprite.y + sprite.height;

			// Vertex order: top-left, top-right, bottom-right, bottom-left
			vertices.push_back({ left, top, 0.0f, sprite.color, sprite.u1, sprite.v1 });
			vertices.push_back({ right, top, 0.0f, sprite.color, sprite.u2, sprite.v1 });
			vertices.push_back({ right, bottom, 0.0f, sprite.color, sprite.u2, sprite.v2 });
			vertices.push_back({ left, bottom, 0.0f, sprite.color, sprite.u1, sprite.v2 });
		}

		// Função para obter informações da textura
		// Você precisa implementar isso baseado em como você carrega texturas
		TextureInfo getTextureInfo(bgfx::TextureHandle texture) {
			TextureInfo info = { 256, 256, true }; // Valores padrão

			// TODO: Implementar recuperação real das dimensões da textura
			// bgfx não tem uma função direta para obter info de texture handle
			// Você precisa manter um mapa de textura -> info quando carrega			

			return info;
		}

		bgfx::ProgramHandle loadShaderProgram(const char* vsPath, const char* fsPath) {
			// Função auxiliar para carregar shader
			auto loadShader = [](const char* filePath) -> bgfx::ShaderHandle {
				FILE* file = fopen(filePath, "rb");
				if (!file) {
					return BGFX_INVALID_HANDLE;
				}

				fseek(file, 0, SEEK_END);
				long size = ftell(file);
				fseek(file, 0, SEEK_SET);

				char* data = new char[size];
				fread(data, 1, size, file);
				fclose(file);

				const bgfx::Memory* mem = bgfx::makeRef(data, size);
				bgfx::ShaderHandle handle = bgfx::createShader(mem);
				delete[] data;

				return handle;
				};

			bgfx::ShaderHandle vsh = loadShader(vsPath);
			bgfx::ShaderHandle fsh = loadShader(fsPath);

			if (!bgfx::isValid(vsh) || !bgfx::isValid(fsh)) {
				return BGFX_INVALID_HANDLE;
			}

			return bgfx::createProgram(vsh, fsh, true); // true = destroy shaders when program is destro
		}		

	private:
		std::vector<Sprite> m_sprites;
		bgfx::ProgramHandle m_program;
		bgfx::UniformHandle m_textureUniform;
		bgfx::TextureHandle m_currentTexture;
		bgfx::IndexBufferHandle m_ib;
		bgfx::DynamicVertexBufferHandle m_vb;
		bgfx::VertexLayout m_layout;

		bool m_beginCalled;
		uint32_t m_currentSpriteCount;

		static constexpr uint16_t kMaxSprites = 2048;
		static constexpr uint16_t kMaxVertices = kMaxSprites * 4;
		static constexpr uint16_t kMaxIndices = kMaxSprites * 6;
	};
}