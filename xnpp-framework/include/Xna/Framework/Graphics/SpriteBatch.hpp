#ifndef XNA_FRAMEWORK_GRAPHICS_SPRITEBATCH_HPP
#define XNA_FRAMEWORK_GRAPHICS_SPRITEBATCH_HPP

#include "GraphicsResource.hpp"
#include "BlendState.hpp"
#include "SamplerState.hpp"
#include "RasterizerState.hpp"
#include "DepthStencilState.hpp"
#include "SpriteFont.hpp"
#include "Texture2D.hpp"
#include "Effect/Effect.hpp"
#include "Xna/Framework/Matrix.hpp"
#include "Xna/Framework/Color.hpp"
#include "Xna/Framework/Vector2.hpp"
#include "Shared.hpp"
#include <optional>
#include <string>
#include "Xna/Platform/Platform.hpp"
#include "Xna/Internal/Export.hpp"

namespace Xna {
	//Enables a group of sprites to be drawn using the same settings.
	class SpriteBatch final : public GraphicsResource {
	public:
		//Initializes a new instance of the class, which enables a group of sprites to be drawn using the same settings.
		XNPP_API SpriteBatch(GraphicsDevice const& graphicsDevice);

		~SpriteBatch() override = default;		
		
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

		//Adds a sprite to a batch of sprites for rendering using the specified texture, destination rectangle, and color.
		inline void Draw(
			Texture2D const& texture,
			Rectangle const& destinationRectangle,
			Color const& color) {
			Draw(texture, destinationRectangle, std::nullopt, color, 0, Vector2::Zero(), SpriteEffects::None, 0);
		}
		
		//Adds a sprite to a batch of sprites for rendering using the specified texture, destination rectangle, source rectangle, and color.
		inline void Draw(
			Texture2D const& texture,
			Rectangle const& destinationRectangle,
			std::optional<Rectangle> const& sourceRectangle,
			Color const& color) {
			Draw(texture, destinationRectangle, sourceRectangle, color, 0, Vector2::Zero(), SpriteEffects::None, 0);
		}
		
		//Adds a sprite to a batch of sprites for rendering using the specified texture, destination rectangle, source rectangle, color, rotation, origin, effects and layer.
		XNPP_API void Draw(
			Texture2D const& texture,
			Rectangle const& destinationRectangle,
			std::optional<Rectangle> const& sourceRectangle,
			Color const& color,
			float rotation,
			Vector2 const& origin, 
			SpriteEffects effects, 
			float layerDepth);
		
		//Adds a sprite to a batch of sprites for rendering using the specified texture, position and color.
		inline void Draw(
			Texture2D const& texture, 
			Vector2 const& position, 
			Color const& color) {
			Draw(texture, position, std::nullopt, color, 0, Vector2::Zero(), Vector2::One(), SpriteEffects::None, 0);
		}
		
		//Adds a sprite to a batch of sprites for rendering using the specified texture, position, source rectangle, and color.
		inline void Draw(
			Texture2D const& texture,
			Vector2 const& position,
			std::optional<Rectangle> const& sourceRectangle,
			Color const& color) {
			Draw(texture, position, sourceRectangle, color, 0, Vector2::Zero(), Vector2::One(), SpriteEffects::None, 0);
		}
		
		//Adds a sprite to a batch of sprites for rendering using the specified texture, position, source rectangle, color, rotation, origin, scale, effects, and layer.
		inline void Draw(
			Texture2D const& texture,
			Vector2 const& position,
			std::optional<Rectangle> const& sourceRectangle,
			Color const& color,
			float rotation,
			Vector2 const& origin,
			float scale, 
			SpriteEffects effects,
			float layerDepth) {
			Draw(texture, position, sourceRectangle, color, rotation, origin, Vector2(scale), effects, layerDepth);
		}

		//Adds a sprite to a batch of sprites for rendering using the specified texture, position, source rectangle, color, rotation, origin, scale, effects and layer.
		XNPP_API void Draw(
			Texture2D const& texture,
			Vector2 const& position,
			std::optional<Rectangle> const& sourceRectangle,
			Color const& color,
			float rotation,
			Vector2 const& origin,
			Vector2 const& scale, 
			SpriteEffects effects,
			float layerDepth);

		//Adds a string to a batch of sprites for rendering using the specified font, text, position, and color.
		void DrawString(
			SpriteFont const& spriteFont,
			std::string const& text,
			Vector2 const& position,
			Color const& color) {
			DrawString(spriteFont, text, position, color, 0, Vector2::Zero(), Vector2::One(), SpriteEffects::None, 0);
		}

		//Adds a string to a batch of sprites for rendering using the specified font, text, position, color, rotation, origin, scale, effects and layer.
		inline void DrawString(
			SpriteFont const& spriteFont,
			std::string const& text,
			Vector2 const& position,
			Color const& color, 
			float rotation, 
			Vector2 const& origin,
			float scale, 
			SpriteEffects effects, 
			float layerDepth) {
			DrawString(spriteFont, text, position, color, rotation, origin, Vector2(scale), effects, layerDepth);
		}

		//Adds a string to a batch of sprites for rendering using the specified font, text, position, color, rotation, origin, scale, effects and layer.
		XNPP_API void DrawString(
			SpriteFont const& spriteFont,
			std::string const& text,
			Vector2 const& position,
			Color const& color,
			float rotation, 
			Vector2 const& origin,
			Vector2 const& scale,
			SpriteEffects effects,
			float layerDepth);
		
		//Flushes the sprite batch and restores the device state to how it was before Begin was called.
		XNPP_API void End();
		
		inline SpriteBatch(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(SpriteBatch const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		struct Implementation {		
			std::optional<GraphicsDevice> device;
			bool beginCalled{ false };
			PlatformImpl::SpriteBatchImpl platformImpl;
			std::unique_ptr<PlatformNS::ISpriteBatch> backend;
		};

		std::shared_ptr<Implementation> impl;

		friend struct Platform;
	};
}

#endif