#ifndef XNA_FRAMEWORK_GRAPHICS_ALPHATESTEFFECT_HPP
#define XNA_FRAMEWORK_GRAPHICS_ALPHATESTEFFECT_HPP

#include "Effect.hpp"
#include "IEffectMatrices.hpp"
#include "IEffectFog.hpp"

namespace Xna {
	class AlphaTestEffect : Effect, IEffectMatrices, IEffectFog {};
}

#endif