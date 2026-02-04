#include "Xna/Framework/Graphics/SpriteBatch.hpp"
#include "Xna/CSharp/Exception.hpp"

namespace Xna {
	SpriteBatch::SpriteBatch(GraphicsDevice const& device) {
		impl = std::make_shared<Implementation>();
		impl->device = device;
		Platform::SpriteBatch_InitializeSpriteBatch(*this, device);
	}

	void SpriteBatch::Begin(SpriteSortMode sortMode, std::optional<BlendState> const& blendState, std::optional<SamplerState> const& samplerState,
		std::optional<DepthStencilState> const& depthStencilState, std::optional<RasterizerState> const& rasterizerState, std::optional<Effect> const& effect, std::optional<Matrix> transformMatrix) {
		
		if (impl->beginCalled)
			throw CSharp::InvalidOperationException("Invalid attempt to call the Begin method twice.");
		
		Platform::SpriteBatch_Begin(*this, sortMode, blendState, samplerState, depthStencilState, rasterizerState, effect, transformMatrix);

		impl->beginCalled = true;
	}

	void SpriteBatch::End() { 
		if (!impl->beginCalled)
			throw CSharp::InvalidOperationException("Invalid attempt to call the End method before Begin.");

		Platform::SpriteBatch_End(*this); 

		impl->beginCalled = false;
	}

	void SpriteBatch::Draw(Texture2D const& texture, Rectangle const& destinationRectangle, std::optional<Rectangle> const& sourceRectangle, Color const& color,
		float rotation, Vector2 const& origin, SpriteEffects effects, float layerDepth) {

		if (!impl->beginCalled)
			throw CSharp::InvalidOperationException("The Begin method was not called before the Draw method.");

		Platform::SpriteBatch_Draw(*this, texture, destinationRectangle, sourceRectangle, color, rotation, origin, effects, layerDepth);
	}

	void SpriteBatch::Draw(Texture2D const& texture, Vector2 const& position, std::optional<Rectangle> const& sourceRectangle, Color const& color,
		float rotation, Vector2 const& origin, Vector2 const& scale, SpriteEffects effects, float layerDepth) {
		if (!impl->beginCalled)
			throw CSharp::InvalidOperationException("The Begin method was not called before the Draw method.");

		Platform::SpriteBatch_Draw(*this, texture, position, sourceRectangle, color, rotation, origin, scale, effects, layerDepth);
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