#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>
#include <bx/math.h>
#include <bx/debug.h>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <cmath>

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
		float originX, originY;
		float scaleX, scaleY;
		float layerDepth;
		float m00, m01;
		float m10, m11;
		float m02, m12;
		uint32_t color;
		uint8_t effects;
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

			// Criar shaders
			std::filesystem::path currentPath;
			Platform::System_GetExecutablePath(currentPath);
			currentPath.remove_filename();
			currentPath /= "shaders/bgfx/";
			currentPath = currentPath.lexically_normal();

			m_program = loadShaderProgram(currentPath / "sprite.vs.bin", currentPath / "sprite.fs.bin");
		}

		void Begin(SpriteSortMode sortMode, const BlendState* blendState, const SamplerState* samplerState, 
			const DepthStencilState* depthStencilState, const RasterizerState* rasterizerState, const Matrix* matrix) override {
			m_beginCalled = true;
			m_currentSpriteCount = 0;
			m_sprites.clear();
			m_currentTexture.idx = bgfx::kInvalidHandle;
			m_blendState = blendState ? BgfxBlendState(*blendState) : BgfxBlendState::NonPremultiplied();
			m_samplerState = samplerState ? BgfxSamplerState(*samplerState) : BgfxSamplerState();
			m_depthStencilState = depthStencilState ? BgfxDepthStencilState(*depthStencilState) : BgfxDepthStencilState();
			m_rasterizerState = rasterizerState ? BgfxRasterizerState(*rasterizerState) : BgfxRasterizerState();			

			m_transformMatrix = matrix ? *matrix : Matrix::Identity();
		}

		void Draw(PlatformNS::ITexture2D const& texture, Vector2 const& pos, const Rectangle* sourceRect, Vector2 const& origin, Vector2 const& scale, float rotation, Color const& color, SpriteEffects effects, float layerDepth) override {
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

			Sprite sprite{};
			sprite.x = pos.X;
			sprite.y = pos.Y;
			sprite.width = width;
			sprite.height = height;
			sprite.originX = origin.X;
			sprite.originY = origin.Y;
			sprite.scaleY = scale.Y;
			sprite.scaleX = scale.X;
			sprite.u1 = u1;
			sprite.v1 = v1;
			sprite.u2 = u2;
			sprite.v2 = v2;
			sprite.color = color;
			sprite.layerDepth = layerDepth;

			const auto cosR = std::cos(rotation);
			const auto sinR = std::sin(rotation);
			const auto m00 = cosR * scale.X;
			const auto m01 = -sinR * scale.Y;
			const auto m10 = sinR * scale.X;
			const auto m11 = cosR * scale.Y;
			const auto m02 = pos.X - origin.X * m00 - origin.Y * m01;
			const auto m12 = pos.Y - origin.X * m10 - origin.Y * m11;

			sprite.m00 = m00;
			sprite.m01 = m01;
			sprite.m10 = m10;
			sprite.m11 = m11;
			sprite.m02 = m02;
			sprite.m12 = m12;

			sprite.texture = bgfxTex->textureHandle;
			sprite.effects = static_cast<uint8_t>(effects);

			if (m_sortMode != SpriteSortMode::Immediate) {

				if (m_sprites.size() >= kMaxSprites) {
					flush(); // Desenha o que tem até agora e limpa a lista
				}

				m_sprites.push_back(sprite);
			}
			else
				immediateFlush(sprite);
		}

		void End() override {
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
			if (m_sprites.size() == 0) return;

			// 1. Ordenação (se necessário)
			sortSprites();

			// 2. Preparar buffer de vértices
			// Usamos alloc para que o bgfx gerencie a memória de transição de forma thread-safe
			const bgfx::Memory* mem = bgfx::alloc(static_cast<uint32_t>(m_sprites.size() * 4 * sizeof(SpriteVertex)));
			SpriteVertex* verts = (SpriteVertex*)mem->data;

			for (size_t i = 0; i < m_sprites.size(); ++i) {
				// Se usou índices na ordenação, acessamos via m_spriteIndices
				uint32_t spriteIdx = (m_sortMode == SpriteSortMode::Deferred) ? (uint32_t)i : m_spriteIndices[i];

				// Pequeno ajuste: passamos o ponteiro destino para evitar cópias extras
				updateSpriteVerticesDirect(verts + (i * 4), m_sprites[spriteIdx]);
			}

			bgfx::update(m_vb, 0, mem);

			//Aplicar transformação
			Matrix transposed = Matrix::Transpose(m_transformMatrix);
			bgfx::setTransform(&transposed.M11);

			// 3. Renderização por Batches (O "Pulo do Gato")
			uint32_t batchStart = 0;
			bgfx::TextureHandle currentBatchTexture = (m_sortMode == SpriteSortMode::Deferred)
				? m_sprites[0].texture
				: m_sprites[m_spriteIndices[0]].texture;

			for (uint32_t i = 0; i <= m_sprites.size(); ++i) {
				bool isLast = (i == m_sprites.size());
				bgfx::TextureHandle tex = BGFX_INVALID_HANDLE;

				if (!isLast) {
					uint32_t idx = (m_sortMode == SpriteSortMode::Deferred) ? i : m_spriteIndices[i];
					tex = m_sprites[idx].texture;
				}

				// Se a textura mudou ou é o fim da lista, desenhamos o batch anterior
				if (isLast || tex.idx != currentBatchTexture.idx) {
					uint32_t spriteCount = i - batchStart;

					bgfx::setVertexBuffer(0, m_vb, 0, m_sprites.size() * 4); // VB inteiro
					// Setamos o range correto do Index Buffer
					bgfx::setIndexBuffer(m_ib, batchStart * 6, spriteCount * 6);					

					//Aplicamos textura e estados					
					bgfx::setStencil(m_depthStencilState.frontStencil, m_depthStencilState.frontStencil);
					uint64_t state = m_blendState.state | m_depthStencilState.depthBuffer | m_rasterizerState.state;
					bgfx::setState(state, m_blendState.blendFactor);						

					const auto samplerStateFlags = m_samplerState > 0 ? m_samplerState : UINT32_MAX;
					bgfx::setTexture(0, m_textureUniform, currentBatchTexture, samplerStateFlags);

					bgfx::submit(0, m_program);

					// Inicia novo batch
					if (!isLast) {
						batchStart = i;
						currentBatchTexture = tex;
					}
				}
			}

			m_sprites.clear();
			m_spriteIndices.clear();
		}

		void immediateFlush(Sprite const& sprite) {
			// 1. Alocamos memória transiente apenas para 4 vértices (1 quad)
			const bgfx::Memory* mem = bgfx::alloc(4 * sizeof(SpriteVertex));
			SpriteVertex* verts = reinterpret_cast<SpriteVertex*>(mem->data);

			// 2. Preenchemos os vértices diretamente na memória alocada
			updateSpriteVerticesDirect(verts, sprite);

			// 3. Atualizamos o Dynamic Vertex Buffer
			// Importante: usamos o offset 0 porque estamos submetendo IMEDIATAMENTE
			bgfx::update(m_vb, 0, mem);

			//Aplicar transformação
			Matrix transposed = Matrix::Transpose(m_transformMatrix);
			bgfx::setTransform(&transposed.M11);

			// 4. Configuração de estado
			uint64_t state = m_blendState.state | m_depthStencilState.depthBuffer | m_rasterizerState.state;

			// 5. Submissão
			// Usamos apenas os primeiros 6 índices do IB (referentes a 1 quad)
			bgfx::setVertexBuffer(0, m_vb, 0, 4);
			bgfx::setIndexBuffer(m_ib, 0, 6);

			//Aplicar transformação
			bgfx::setTransform(&m_transformMatrix);

			bgfx::setStencil(m_depthStencilState.frontStencil, m_depthStencilState.frontStencil);
			
			bgfx::setState(state, m_blendState.blendFactor);

			const auto samplerStateFlags = m_samplerState > 0 ? m_samplerState : UINT32_MAX;
			bgfx::setTexture(0, m_textureUniform, sprite.texture, samplerStateFlags);
			
			bgfx::submit(0, m_program);
		}

		void sortSprites() {
			if (m_spriteIndices.size() != m_sprites.size())
				m_spriteIndices.resize(m_sprites.size());

			switch (m_sortMode)
			{
			case Xna::SpriteSortMode::Texture:
				std::sort(m_spriteIndices.begin(), m_spriteIndices.end(), [&](uint32_t a, uint32_t b) {
					return m_sprites[a].texture.idx < m_sprites[b].texture.idx;
					});
				break;
			case Xna::SpriteSortMode::BackToFront:
				std::stable_sort(m_spriteIndices.begin(), m_spriteIndices.end(), [&](uint32_t a, uint32_t b) {
					return m_sprites[a].layerDepth < m_sprites[b].layerDepth;
					});
				break;
			case Xna::SpriteSortMode::FrontToBack:
				std::stable_sort(m_spriteIndices.begin(), m_spriteIndices.end(), [&](uint32_t a, uint32_t b) {
					return m_sprites[a].layerDepth > m_sprites[b].layerDepth;
					});
				break;
			default:
				return;
			}
		}

		void updateSpriteVerticesDirect(SpriteVertex* target, Sprite const& s) {
			// Definimos os cantos locais (0,0 até width, height)
			float w = s.width;
			float h = s.height;

			// Coordenadas locais dos 4 vértices
			float px[4] = { 0.0f, w,    w,    0.0f };
			float py[4] = { 0.0f, 0.0f, h,    h };

			// Aplicamos a matriz de transformação pré-calculada no Draw()
			for (int i = 0; i < 4; ++i) {
				target[i].x = px[i] * s.m00 + py[i] * s.m01 + s.m02;
				target[i].y = px[i] * s.m10 + py[i] * s.m11 + s.m12;
				target[i].z = s.layerDepth; // Usamos o depth para o Z-buffer se necessário
				target[i].color = s.color;
			}

			// Cálculo de UVs com suporte a Flip
			float u1 = s.u1, v1 = s.v1, u2 = s.u2, v2 = s.v2;
			if (s.effects & (uint8_t)SpriteEffects::FlipHorizontally) std::swap(u1, u2);
			if (s.effects & (uint8_t)SpriteEffects::FlipVertically)   std::swap(v1, v2);

			target[0].u = u1; target[0].v = v1;
			target[1].u = u2; target[1].v = v1;
			target[2].u = u2; target[2].v = v2;
			target[3].u = u1; target[3].v = v2;
		}

		bgfx::ProgramHandle loadShaderProgram(std::filesystem::path const& vsPath, std::filesystem::path const& fsPath) {
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

			bgfx::ShaderHandle vsh = loadShader(vsPath.string().c_str());
			bgfx::ShaderHandle fsh = loadShader(fsPath.string().c_str());

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

		std::vector<uint32_t> m_spriteIndices;
		SpriteSortMode m_sortMode{ SpriteSortMode::Deferred };

		bool m_beginCalled;
		uint32_t m_currentSpriteCount;

		BgfxBlendState m_blendState{};
		BgfxSamplerState m_samplerState{};
		BgfxDepthStencilState m_depthStencilState{};
		BgfxRasterizerState m_rasterizerState{};
		Matrix m_transformMatrix{};

		static constexpr uint16_t kMaxSprites = 2048;
		static constexpr uint16_t kMaxVertices = kMaxSprites * 4;
		static constexpr uint16_t kMaxIndices = kMaxSprites * 6;
	};

	std::unique_ptr<PlatformNS::ISpriteBatch> PlatformNS::ISpriteBatch::Create(GraphicsDevice const& device) {
		//O backend padrão com bgfx não necessita do GraphicsDevice
		return std::make_unique<BgfxSpriteBatch>();
	}
}