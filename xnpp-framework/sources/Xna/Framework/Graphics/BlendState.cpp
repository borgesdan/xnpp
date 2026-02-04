#include "Xna/Framework/Graphics/BlendState.hpp"

namespace Xna {
	BlendState BlendState::Opaque() {
		auto b = BlendState();
		b.impl->colorSourceBlend = Xna::Blend::One;
		b.impl->colorDestinationBlend = Blend::Zero;
		b.impl->alphaSourceBlend = Blend::One;
		b.impl->alphaDestinationBlend = Blend::Zero;
		return b;
	}

	BlendState BlendState::AlphaBlend() {
		auto b = BlendState();
		b.impl->colorSourceBlend = Xna::Blend::One;
		b.impl->colorDestinationBlend = Blend::InverseSourceAlpha;
		b.impl->alphaSourceBlend = Blend::One;
		b.impl->alphaDestinationBlend = Blend::InverseSourceAlpha;
		return b;
	}

	BlendState BlendState::Additive() {
		auto b = BlendState();
		b.impl->colorSourceBlend = Xna::Blend::SourceAlpha;
		b.impl->colorDestinationBlend = Blend::One;
		b.impl->alphaSourceBlend = Blend::SourceAlpha;
		b.impl->alphaDestinationBlend = Blend::One;
		return b;
	}

	BlendState BlendState::NonPremultiplied() {
		auto b = BlendState();
		b.impl->colorSourceBlend = Xna::Blend::SourceAlpha;
		b.impl->colorDestinationBlend = Blend::InverseSourceAlpha;
		b.impl->alphaSourceBlend = Blend::SourceAlpha;
		b.impl->alphaDestinationBlend = Blend::InverseSourceAlpha;
		return b;
	}			
}