#include "Xna/Framework/Graphics/SpriteBatch.hpp"
#include "Xna/CSharp/Exception.hpp"

namespace Xna {
	SpriteBatch::SpriteBatch(GraphicsDevice const& device) {
		impl = std::make_shared<Implementation>();
		impl->device = device;
		impl->backend = PlatformNS::ISpriteBatch::Create();
	}

	void SpriteBatch::Begin(SpriteSortMode sortMode, std::optional<BlendState> const& blendState, std::optional<SamplerState> const& samplerState,
		std::optional<DepthStencilState> const& depthStencilState, std::optional<RasterizerState> const& rasterizerState, std::optional<Effect> const& effect, std::optional<Matrix> transformMatrix) {
		
		if (impl->beginCalled)
			throw CSharp::InvalidOperationException("Invalid attempt to call the Begin method twice.");		
		
		impl->backend->Begin(
			sortMode, 
			blendState ? &blendState.value() : nullptr,
			samplerState ? &samplerState.value() : nullptr);

		impl->beginCalled = true;
	}

	void SpriteBatch::End() { 
		if (!impl->beginCalled)
			throw CSharp::InvalidOperationException("Invalid attempt to call the End method before Begin.");

		//Platform::SpriteBatch_End(*this); 
		impl->backend->End();

		impl->beginCalled = false;
	}

	void SpriteBatch::Draw(Texture2D const& texture, Rectangle const& destRect, std::optional<Rectangle> const& sourceRectangle, Color const& color,
		float rotation, Vector2 const& origin, SpriteEffects effects, float layerDepth) {

		if (!impl->beginCalled)
			throw CSharp::InvalidOperationException("The Begin method was not called before the Draw method.");
		
		const auto& texBackend = texture.GetBackend();
		impl->backend->Draw(
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
		if (!impl->beginCalled)
			throw CSharp::InvalidOperationException("The Begin method was not called before the Draw method.");

		const auto& texBackend = texture.GetBackend();
		impl->backend->Draw(
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
		
		if (!impl->beginCalled)
			throw CSharp::InvalidOperationException("The Begin method was not called before the DrawFont method.");

		if (text.empty())
			return;

		Platform::SpriteBatch_DrawString(*this, spriteFont, text, position, color, rotation, origin, scale, effects, layerDepth);
	}
}