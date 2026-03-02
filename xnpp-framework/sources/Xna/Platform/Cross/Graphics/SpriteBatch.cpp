#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/math.h>
#include <bx/debug.h>
#include <stdexcept>

#include "Xna/Framework/Graphics/SpriteBatch.hpp"
#include "Xna/Framework/Rectangle.hpp"
#include "Xna/Framework/Vector2.hpp"
#include "BgfxGraphics.hpp"

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

	struct BgfxSpriteBatch final : public PlatformNS::ISpriteBatch {
		BgfxSpriteBatch() : m_beginCalled(false), m_currentSpriteCount(0) {
			SpriteVertex::init();
			m_layout = SpriteVertex::ms_layout;

			// Criar buffers com tamanho máximo
			m_vb = bgfx::createDynamicVertexBuffer(
				kMaxVertices,    // Capacidade total pré-alocada
				m_layout,        // Layout dos vértices
				BGFX_BUFFER_NONE // Ou BGFX_BUFFER_ALLOW_RESIZE se quiser que ele cresça dinamicamente
			);

			if (!bgfx::isValid(m_vb)) {
				throw std::runtime_error("");
			}

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

			m_ib = bgfx::createDynamicIndexBuffer(
				bgfx::copy(indices, kMaxIndices * sizeof(uint16_t)),
				BGFX_BUFFER_NONE
			);

			delete[] indices;

			// Criar uniform para textura
			m_textureUniform = bgfx::createUniform("s_texture", bgfx::UniformType::Sampler);

			// Criar shaders (você precisará compilar seus próprios shaders)
			m_program = loadShaderProgram("C:/Users/Borges/source/repos/xnpp/xnpp-framework/shaders/sprite.vs.bin", "C:/Users/Borges/source/repos/xnpp/xnpp-framework/shaders/sprite.fs.bin");
		}

		void Begin() override {
			m_beginCalled = true;
			m_currentSpriteCount = 0;
			m_sprites.clear();
			m_currentTexture.idx = bgfx::kInvalidHandle;
		}		

		// Método principal que todos os outros chamam
		void Draw(PlatformNS::ITexture2D const& texture, const Rectangle* sourceRect, Vector2 const& pos, Vector2 const& scale, Color const& color) override {
			if (!m_beginCalled) return;			

			float width, height;
			float u1 = 0.0f, v1 = 0.0f, u2 = 1.0f, v2 = 1.0f;

			const auto bgfxTex = reinterpret_cast<const BgfxTexture2D*>(&texture);

			if (sourceRect) {
				width = static_cast<float>(sourceRect->Width);
				height = static_cast<float>(sourceRect->Height);

				// Calcular UVs baseado nas dimensões da textura
				if (bgfxTex->width > 0 && bgfxTex->height > 0) {
					u1 = sourceRect->X / static_cast<float>(bgfxTex->width);
					v1 = sourceRect->Y / static_cast<float>(bgfxTex->height);
					u2 = (sourceRect->X + sourceRect->Width) / static_cast<float>(bgfxTex->width);
					v2 = (sourceRect->Y + sourceRect->Height) / static_cast<float>(bgfxTex->height);
				}
			}
			else {
				width = static_cast<float>(bgfxTex->width);
				height = static_cast<float>(bgfxTex->height);
			}

			Sprite sprite;
			sprite.x = pos.X;
			sprite.y = pos.Y;
			sprite.width = width * scale.X;
			sprite.height = height * scale.Y;
			sprite.u1 = u1;
			sprite.v1 = v1;
			sprite.u2 = u2;
			sprite.v2 = v2;
			sprite.color = SwapXnaColor(color);
			sprite.texture = bgfxTex->textureHandle;

			m_sprites.push_back(sprite);
		}

		void End() override  {
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
			bgfx::update(m_vb, 0, bgfx::copy(vertices.data(), vertices.size() * sizeof(SpriteVertex)));

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
				
				const bgfx::Memory* mem = bgfx::copy(data, size);
				delete[] data;
				
				bgfx::ShaderHandle handle = bgfx::createShader(mem);

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
		bgfx::DynamicIndexBufferHandle m_ib;
		bgfx::DynamicVertexBufferHandle m_vb;
		bgfx::VertexLayout m_layout;

		bool m_beginCalled;
		uint32_t m_currentSpriteCount;

		static constexpr uint16_t kMaxSprites = 2048;
		static constexpr uint16_t kMaxVertices = kMaxSprites * 4;
		static constexpr uint16_t kMaxIndices = kMaxSprites * 6;
	};

	std::unique_ptr<PlatformNS::ISpriteBatch> PlatformNS::ISpriteBatch::Create() {
		return std::make_unique<BgfxSpriteBatch>();
	}
}