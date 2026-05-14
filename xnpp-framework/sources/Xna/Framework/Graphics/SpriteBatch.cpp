#include "Xna/Framework/Graphics/SpriteBatch.hpp"
#include "Xna/Framework/Graphics/SpriteFont.hpp"
#include <Xna/CSharp/Exception.hpp>

namespace Xna {
	static void normalizeLayerDepth(float& layerDepth) {
		if (layerDepth < 0)
			layerDepth = 0;
		else if (layerDepth > 1)
			layerDepth = 1;
	}

	SpriteBatch::SpriteBatch(GraphicsDevice const& device) {
		backend = PlatformNS::ISpriteBatch::Create(device);
	}

	void SpriteBatch::Begin(
		SpriteSortMode sortMode,
		const BlendState* blendState,
		const SamplerState* samplerState,
		const DepthStencilState* depthStencilState,
		const RasterizerState* rasterizerState,
		const Effect* effect,
		Matrix const& transformMatrix) {		
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
		
		backend->Begin(
			sortMode,
			blendState ? &blendState.value().get() : nullptr,
			samplerState ? &samplerState.value().get() : nullptr,
			depthStencilState ? &depthStencilState.value().get() : nullptr,
			rasterizerState ? &rasterizerState.value().get() : nullptr,
			transformMatrix ? &transformMatrix.value().get() : nullptr);
	}

	void SpriteBatch::End() {		
		backend->End();
	}

	void SpriteBatch::Draw(Texture2D const& texture, Rectangle destRect, Color color, std::optional<Rectangle> sourceRectangle,
		float rotation, Vector2 origin, SpriteEffects effects, float layerDepth) {

		assert(texture != nullptr && "texture is null.");

		//Se o retângulo de destino está vázio então não há nada a desenhar
		if (destRect.IsEmpty())	return;		

		normalizeLayerDepth(layerDepth);
		
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

	void SpriteBatch::Draw(Texture2D const& texture, Vector2 position, Color color, std::optional<Rectangle> sourceRectangle,
		float rotation, Vector2 origin, Vector2 scale, SpriteEffects effects, float layerDepth) {

		assert(texture != nullptr && "texture is null.");
		
		normalizeLayerDepth(layerDepth);

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

	void SpriteBatch::DrawString(SpriteFont const& spriteFont, std::string_view text, Vector2 position, Color color, float rotation, Vector2 origin,
		Vector2 scale, SpriteEffects effects, float layerDepth) {

		assert(spriteFont != nullptr && "spriteFont is null.");

		if (text.empty()) return;			

		normalizeLayerDepth(layerDepth);

		spriteFont.InternalDraw(
			text,
			*this,
			position,
			color,
			rotation,
			origin,
			scale,
			effects,
			layerDepth);
	}

	void SpriteBatch::DrawString(SpriteFont const& spriteFont, std::u32string_view text, Vector2 position, Color color, float rotation, Vector2 origin,
		Vector2 scale, SpriteEffects effects, float layerDepth) {

		assert(spriteFont != nullptr && "spriteFont is null.");

		if (text.empty()) return;				

		normalizeLayerDepth(layerDepth);

		spriteFont.InternalDraw(
			text,
			*this,
			position,
			color,
			rotation,
			origin,
			scale,
			effects,
			layerDepth);
	}
}