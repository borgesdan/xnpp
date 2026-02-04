#ifndef XNA_FRAMEWORK_GRAPHICS_IFFECTFOG_HPP
#define XNA_FRAMEWORK_GRAPHICS_IFFECTFOG_HPP

#include "Xna/Framework/Vector3.hpp"

namespace Xna {
	struct IEffectFog {
		virtual ~IEffectFog() = default;
		virtual bool FogEnabled() const = 0;
		virtual bool FogStart() const = 0;
		virtual bool FogEnd() const = 0;
		virtual Vector3 FogColor() const = 0;
		virtual void FogEnabled(bool value) = 0;
		virtual void FogStart(bool value) = 0;
		virtual void FogEnd(bool value) = 0;
		virtual void FogColor(Vector3 const& value) = 0;
	};
}

#endif