#ifndef XNA_FRAMEWORK_GRAPHICS_SPRITEBATCH_HPP
#define XNA_FRAMEWORK_GRAPHICS_SPRITEBATCH_HPP

#include "_GraphicsDevice.hpp"
#include "BlendState.hpp"
#include "SamplerState.hpp"
#include "RasterizerState.hpp"
#include "DepthStencilState.hpp"
#include "Texture2D.hpp"
#include "Effect/Effect.hpp"
#include "Xna/Framework/Matrix.hpp"
#include "Xna/Framework/Color.hpp"
#include "Xna/Framework/Vector2.hpp"
#include "Shared.hpp"
#include <optional>
#include <string>
#include "Xna/Platform/Platform.hpp"
#include "Xna/Internal/Macros.hpp"

namespace Xna {
	class SpriteFont;
	//Enables a group of sprites to be drawn using the same settings.
	class SpriteBatch {
	public:
		//Initializes a new instance of the class, which enables a group of sprites to be drawn using the same settings.
		XNPP_API SpriteBatch(GraphicsDevice const& graphicsDevice);

		//Begins a sprite batch operation using the specified sort, blend, sampler, depth stencil, rasterizer state objects, plus a custom effect and a 2D transformation matrix.
		//Passing null for any of the state objects selects the default default state objects (BlendState.AlphaBlend, DepthStencilState.None, RasterizerState.CullCounterClockwise, SamplerState.LinearClamp). 
		//Passing a null effect selects the default SpriteBatch Class shader.
		XNPP_API void Begin(
			SpriteSortMode sortMode = SpriteSortMode::Deferred,
			const BlendState* blendState = nullptr,
			const SamplerState* samplerState = nullptr,
			const DepthStencilState* depthStencilState = nullptr,
			const RasterizerState* rasterizerState = nullptr,
			const Effect* effect = nullptr,
			Matrix const& transformMatrix = Matrix::Identity());

		//Begins a sprite batch operation using the specified sort, blend, sampler, depth stencil, rasterizer state objects, plus a custom effect and a 2D transformation matrix.
		//Passing null for any of the state objects selects the default default state objects (BlendState.AlphaBlend, DepthStencilState.None, RasterizerState.CullCounterClockwise, SamplerState.LinearClamp). 
		//Passing a null effect selects the default SpriteBatch Class shader.
		XNPP_API void Begin(
			SpriteSortMode sortMode,
			std::optional<std::reference_wrapper<const BlendState>> const& blendState,
			std::optional<std::reference_wrapper<const SamplerState>> const& samplerState,
			std::optional<std::reference_wrapper<const DepthStencilState>> const& depthStencilState,
			std::optional<std::reference_wrapper<const RasterizerState>> const& rasterizerState,
			std::optional<std::reference_wrapper<const Effect>> const& effect,
			std::optional<std::reference_wrapper<const Matrix>> transformMatrix = Matrix::Identity());		

		//Adds a sprite to a batch of sprites for rendering using the specified texture, destination rectangle, source rectangle, color, rotation, origin, effects and layer.
		XNPP_API void Draw(
			Texture2D const& texture,
			Rectangle destinationRectangle,
			Color color,
			std::optional<Rectangle> sourceRectangle = std::nullopt,
			float rotation = 0,
			Vector2 origin = Vector2::Zero(),
			SpriteEffects effects = SpriteEffects::None,
			float layerDepth = 0);

		//Adds a sprite to a batch of sprites for rendering using the specified texture, position, source rectangle, color, rotation, origin, scale, effects and layer.
		XNPP_API void Draw(
			Texture2D const& texture,
			Vector2 position,
			Color color,
			std::optional<Rectangle> sourceRectangle = std::nullopt,
			float rotation = 0,
			Vector2 origin = Vector2::Zero(),
			Vector2 scale = Vector2::One(),
			SpriteEffects effects = SpriteEffects::None,
			float layerDepth = 0);

		//Adds a string to a batch of sprites for rendering using the specified font, text, position, color, rotation, origin, scale, effects and layer.
		XNPP_API void DrawString(
			SpriteFont const& spriteFont,
			std::string_view text,
			Vector2 position,
			Color color,
			float rotation = 0,
			Vector2 origin = Vector2::Zero(),
			Vector2 scale = Vector2::One(),
			SpriteEffects effects = SpriteEffects::None,
			float layerDepth = 0);

		//Adds a string to a batch of sprites for rendering using the specified font, text, position, color, rotation, origin, scale, effects and layer.
		XNPP_API void DrawString(
			SpriteFont const& spriteFont,
			std::u32string_view text,
			Vector2 position,
			Color color,
			float rotation = 0,
			Vector2 origin = Vector2::Zero(),
			Vector2 scale = Vector2::One(),
			SpriteEffects effects = SpriteEffects::None,
			float layerDepth = 0);

		//Flushes the sprite batch and restores the device state to how it was before Begin was called.
		XNPP_API void End();

		XNPP_DECLARE_IMPL_WRAPPER(SpriteBatch, backend);
	private:
		std::shared_ptr<PlatformNS::ISpriteBatch> backend;
	};
}

#endif