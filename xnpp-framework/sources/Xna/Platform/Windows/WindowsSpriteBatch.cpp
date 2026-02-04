#include "Xna/Platform/Platform.hpp"
#ifdef PLATFORM_WINDOWS
#include "Xna/Framework/Graphics/GraphicsDevice.hpp"
#include "Xna/Framework/Graphics/SpriteBatch.hpp"
#include "Xna/Framework/Graphics/SpriteFont.hpp"
#include "Xna/Misc.hpp"

namespace Xna {
	//
	// SpriteBatch
	//

	void Platform::SpriteBatch_InitializeSpriteBatch(SpriteBatch const& sb, GraphicsDevice const& device) {
		sb.impl->platformImpl.spriteBatch = std::make_unique<DirectX::SpriteBatch>(device.implGraphicsDevice->platformImpl.context.Get());
	}

	void Platform::SpriteBatch_Begin(SpriteBatch const& sb, SpriteSortMode sortMode, std::optional<BlendState> const& blendState, std::optional<SamplerState> const& samplerState,
		std::optional<DepthStencilState> const& depthStencilState, std::optional<RasterizerState> const& rasterizerState, std::optional<Effect> const& effect, std::optional<Matrix> transformMatrix) {

		auto _blend = blendState.value_or(BlendState::AlphaBlend());
		auto _depth = depthStencilState.value_or(DepthStencilState::None());
		auto _sampler = samplerState.value_or(SamplerState::LinearClamp());
		auto _rasterizer = rasterizerState.value_or(RasterizerState::CullCounterClockwise());
		auto _matrix = transformMatrix.value_or(Matrix::Identity());
		std::function<void __cdecl()> _effect = nullptr; //TODO: aplicar efeitos

		DirectX::SpriteSortMode sort = DxHelpers::SpriteSortModeToDx(sortMode);

		sb.impl->platformImpl.spriteBatch->Begin(
			sort,
			_blend.impl->platformImpl.blendState.Get(),
			_sampler.impl->platformImpl.samplerState.Get(),
			_depth.impl->platformImpl.depthStencil.Get(),
			_rasterizer.impl->platformImpl.rasterizerState.Get(),
			_effect,
			DxHelpers::MatrixToDx(_matrix));
	}

	void Platform::SpriteBatch_End(SpriteBatch const& sb) {
		sb.impl->platformImpl.spriteBatch->End();
	}

	void Platform::SpriteBatch_Draw(SpriteBatch const& sb, Texture2D const& texture, Vector2 const& position, std::optional<Rectangle> const& sourceRectangle, Color const& color,
		float rotation, Vector2 const& origin, Vector2 const& scale, SpriteEffects effects, float layerDepth) {

		const bool rectangleHasValue = sourceRectangle.has_value();
		RECT _sourceRect = rectangleHasValue ? DxHelpers::RectangleToDx(*sourceRectangle) : RECT();

		DirectX::XMFLOAT2 _position{ position.X, position.Y };
		DirectX::XMFLOAT2 _origin{ origin.X, origin.Y };
		DirectX::XMFLOAT2 _scale{ scale.X, scale.Y };
		const auto v4 = color.ToVector4();
		DirectX::XMFLOAT4 _color{ v4.X, v4.Y, v4.Z, v4.W };

		sb.impl->platformImpl.spriteBatch->Draw(
			texture.impl->platformImpl.shaderResource.Get(),
			DirectX::XMLoadFloat2(&_position),
			rectangleHasValue ? &_sourceRect : nullptr,
			DirectX::XMLoadFloat4(&_color),
			rotation,
			DirectX::XMLoadFloat2(&_origin),
			DirectX::XMLoadFloat2(&_scale),
			DxHelpers::SpriteEffectsToDx(effects),
			layerDepth
		);
	}

	void Platform::SpriteBatch_Draw(SpriteBatch const& sb, Texture2D const& texture, Rectangle const& destinationRectangle, std::optional<Rectangle> const& sourceRectangle, Color const& color,
		float rotation, Vector2 const& origin, SpriteEffects effects, float layerDepth) {
		RECT _destinationRect = DxHelpers::RectangleToDx(destinationRectangle);

		const bool sourceRectHasValue = sourceRectangle.has_value();
		RECT _sourceRect = sourceRectHasValue ? DxHelpers::RectangleToDx(*sourceRectangle) : RECT();

		const auto v4 = color.ToVector4();
		DirectX::XMFLOAT4 f4(v4.X, v4.Y, v4.Z, v4.W);
		DirectX::XMFLOAT2 _origin{ origin.X, origin.Y };

		sb.impl->platformImpl.spriteBatch->Draw(
			texture.impl->platformImpl.shaderResource.Get(),
			_destinationRect,
			sourceRectHasValue ? &_sourceRect : nullptr,
			DirectX::XMLoadFloat4(&f4),
			rotation,
			_origin,
			DxHelpers::SpriteEffectsToDx(effects),
			layerDepth
		);
	}

	void Platform::SpriteBatch_DrawString(SpriteBatch const& sb, SpriteFont const& sf, std::string const& text, Vector2 const& position, Color const& color, float rotation, Vector2 const& origin,
		Vector2 const& scale, SpriteEffects effects, float layerDepth) {

		DirectX::XMFLOAT2 _position{ position.X, position.Y };
		DirectX::XMFLOAT2 _origin{ origin.X, origin.Y };
		DirectX::XMFLOAT2 _scale{ scale.X, scale.Y };
		const auto v4 = color.ToVector4();
		DirectX::XMFLOAT4 _color{ v4.X, v4.Y, v4.Z, v4.W };

		sf.impl->platformImpl.spriteFont->DrawString(
			sb.impl->platformImpl.spriteBatch.get(),
			text.c_str(),
			DirectX::XMLoadFloat2(&_position),
			DirectX::XMLoadFloat4(&_color),
			rotation,
			DirectX::XMLoadFloat2(&_origin),
			DirectX::XMLoadFloat2(&_scale),
			DxHelpers::SpriteEffectsToDx(effects),
			layerDepth);
	}

	//
	// SpriteFont
	//

	void Platform::SpriteFont_Create(SpriteFont const& sf, Texture2D const& texture,
		std::vector<Rectangle> const& glyphs, std::vector<Rectangle> const& cropping,
		std::vector<char32_t> const& charMap, int32_t lineSpacing, float spacing,
		std::vector<Vector3> const& kerning, std::optional<char32_t> const& defaultCharacter) {

		using Glyph = DirectX::SpriteFont::Glyph;

		std::vector<Glyph> dxGlyps(glyphs.size());

		for (size_t i = 0; i < dxGlyps.size(); ++i) {
			Glyph g;
			g.Subrect.left = static_cast<LONG>(glyphs[i].Left());
			g.Subrect.right = static_cast<LONG>(glyphs[i].Right());
			g.Subrect.top = static_cast<LONG>(glyphs[i].Top());
			g.Subrect.bottom = static_cast<LONG>(glyphs[i].Bottom());
			g.Character = static_cast<uint32_t>(charMap[i]);

			if (!kerning.empty()) {
				g.XOffset = kerning[i].X;
				g.YOffset = static_cast<float>(cropping[i].Y);
				g.XAdvance = kerning[i].Z + spacing;
			}
			else {
				g.XOffset = 0;
				g.YOffset = 0;
				g.XAdvance = spacing;
			}

			dxGlyps[i] = g;
		}

		sf.impl->platformImpl.spriteFont = std::make_unique<DirectX::SpriteFont>(
			//ID3D11ShaderResourceView* texture
			texture.impl->platformImpl.shaderResource.Get(),
			//Glyph const* glyphs
			dxGlyps.data(),
			//size_t glyphCount
			glyphs.size(),
			//float lineSpacing
			static_cast<float>(lineSpacing)
		);

		if (defaultCharacter.has_value()) {
			const auto wchar = Xna::Misc::Char32ToWString(*defaultCharacter);
			sf.impl->platformImpl.spriteFont->SetDefaultCharacter(wchar[0]);
		}
	}

	Vector2 Platform::SpriteFont_MeasureString(SpriteFont const& sf, std::string const& text, bool ignoreWhiteSpace) {
		const auto size = sf.impl->platformImpl.spriteFont->MeasureString(text.c_str(), ignoreWhiteSpace);

		DirectX::XMFLOAT2 temp;
		DirectX::XMStoreFloat2(&temp, size);

		return Vector2(temp.x, temp.y);
	}

	Vector2 Platform::SpriteFont_MeasureString(SpriteFont const& sf, std::wstring const& text, bool ignoreWhiteSpace) {
		const auto size = sf.impl->platformImpl.spriteFont->MeasureString(text.c_str(), ignoreWhiteSpace);

		DirectX::XMFLOAT2 temp;
		DirectX::XMStoreFloat2(&temp, size);

		return Vector2(temp.x, temp.y);
	}
}

#endif