#include "Xna/Framework/Graphics/SpriteBatch.hpp"
#include "Xna/CSharp/Exception.hpp"

namespace Xna {
	SpriteBatch::SpriteBatch(GraphicsDevice const& device) {
		//[TODO]: GraphicsDevice é recebido como argumento mas o backend padrão com bgfx não precisa dele.
		//Talvez para outro backend gráfico seja necessário.
		backend = PlatformNS::ISpriteBatch::Create();
	}

	void SpriteBatch::Begin(
		SpriteSortMode sortMode,
		const BlendState* blendState,
		const SamplerState* samplerState,
		const DepthStencilState* depthStencilState,
		const RasterizerState* rasterizerState,
		const Effect* effect,
		Matrix const& transformMatrix) {

		//Aqui teria uma exceção ("Invalid attempt to call the Begin method twice.") caso o  campo 'beginCalled' fosse true.
		//A validação ficou a cargo do backend, mas um assert ou um 'return' caso true é mais do que necessário.
		backend->Begin(
			sortMode,
			blendState,
			samplerState,
			depthStencilState,
			rasterizerState,
			&transformMatrix);
	}

	void SpriteBatch::Begin(
		SpriteSortMode sortMode,
		std::optional<std::reference_wrapper<const BlendState>> const& blendState,
		std::optional<std::reference_wrapper<const SamplerState>> const& samplerState,
		std::optional<std::reference_wrapper<const DepthStencilState>> const& depthStencilState,
		std::optional<std::reference_wrapper<const RasterizerState>> const& rasterizerState,
		std::optional<std::reference_wrapper<const Effect>> const& effect,
		std::optional<std::reference_wrapper<const Matrix>> transformMatrix) {
		
		//Aqui teria uma exceção ("Invalid attempt to call the Begin method twice.") caso o  campo 'beginCalled' fosse true.
		//A validação ficou a cargo do backend, mas um assert ou um 'return' caso true é mais do que necessário.
		backend->Begin(
			sortMode, 
			blendState ? &blendState.value().get() : nullptr,
			samplerState ? &samplerState.value().get() : nullptr,
			depthStencilState ? &depthStencilState.value().get() : nullptr,
			rasterizerState ? &rasterizerState.value().get() : nullptr,
			transformMatrix ? &transformMatrix.value().get() : nullptr);
	}

	void SpriteBatch::End() { 
		//Aqui teria uma exceção ("Invalid attempt to call the End method before Begin.") caso o  campo 'beginCalled' fosse false.		
		//A validação ficou a cargo do backend, mas um assert ou um 'return' caso false é mais do que necessário.
		backend->End();
	}

	void SpriteBatch::Draw(Texture2D const& texture, Rectangle const& destRect, std::optional<Rectangle> const& sourceRectangle, Color const& color,
		float rotation, Vector2 const& origin, SpriteEffects effects, float layerDepth) {

		assert(texture != nullptr && "texture is null.");
		assert((layerDepth >= 0.0f && layerDepth <= 1.0f) && "layerDepth out of bounds.");
		assert((destRect.Width > 0 && destRect.Height > 0) && "destRect is empty.");
		assert(((!sourceRectangle.has_value() || (sourceRectangle->Width > 0 && sourceRectangle->Height > 0))) && "sourceRectangle is empty.");

		//Aqui teria uma exceção ("The Begin method was not called before the Draw method.") caso o  campo 'beginCalled' fosse false.
		//A validação ficou a cargo do backend, mas um assert ou um 'return' caso false é mais do que necessário.
		const auto& texBackend = texture.GetBackend();

		backend->Draw(
			texBackend,
			destRect.Location(),
			sourceRectangle ? &sourceRectangle.value() : nullptr,
			origin,
			Vector2(
				static_cast<float>(texture.Width()) / static_cast<float>(destRect.Width),
				static_cast<float>(texture.Height()) / static_cast<float>(destRect.Height)),
			rotation,
			color,
			effects,
			layerDepth);
	}

	void SpriteBatch::Draw(Texture2D const& texture, Vector2 const& position, std::optional<Rectangle> const& sourceRectangle, Color const& color,
		float rotation, Vector2 const& origin, Vector2 const& scale, SpriteEffects effects, float layerDepth) {
		
		assert(texture != nullptr && "texture is null.");
		assert((layerDepth >= 0.0f && layerDepth <= 1.0f) && "layerDepth out of bounds.");
		assert(((!sourceRectangle.has_value() || (sourceRectangle->Width > 0 && sourceRectangle->Height > 0))) && "sourceRectangle is empty.");

		//Aqui teria uma exceção ("The Begin method was not called before the Draw method.") caso o  campo 'beginCalled' fosse false.
		//A validação ficou a cargo do backend, mas um assert ou um 'return' caso false é mais do que necessário.		

		const auto& texBackend = texture.GetBackend();
		backend->Draw(
			texBackend,
			position,			
			sourceRectangle ? &sourceRectangle.value() : nullptr,
			origin,
			scale,
			rotation,
			color,
			effects,
			layerDepth);
	}

	void SpriteBatch::DrawString(SpriteFont const& spriteFont, std::string const& text, Vector2 const& position, Color const& color, float rotation, Vector2 const& origin,
		Vector2 const& scale, SpriteEffects effects, float layerDepth) {

		assert(spriteFont != nullptr && "spriteFont is null.");
		assert((layerDepth >= 0.0f && layerDepth <= 1.0f) && "layerDepth out of bounds.");
		
		//Aqui teria uma exceção ("The Begin method was not called before the Draw method.") caso o  campo 'beginCalled' fosse false.
		//A validação ficou a cargo do backend, mas um assert ou um 'return' caso false é mais do que necessário.

		if (text.empty())
			return;

		//Platform::SpriteBatch_DrawString(*this, spriteFont, text, position, color, rotation, origin, scale, effects, layerDepth);
	}
}