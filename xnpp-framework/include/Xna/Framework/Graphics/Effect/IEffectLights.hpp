#ifndef XNA_FRAMEWORK_GRAPHICS_IFFECTLIGHTS_HPP
#define XNA_FRAMEWORK_GRAPHICS_IFFECTLIGHTS_HPP

#include "Xna/Framework/Vector3.hpp"
#include "DirectionalLight.hpp"

namespace Xna {
	struct IEffectLights {
		virtual ~IEffectLights() = default;

		virtual DirectionalLight DirectionalLight0() const = 0;
		virtual DirectionalLight DirectionalLight1() const = 0;
		virtual DirectionalLight DirectionalLight2() const = 0;
		virtual Vector3 AmbientLightColor() const = 0;
		virtual void AmbientLightColor(Vector3 const& value) = 0;
		virtual bool LightingEnabled() const = 0;
		virtual void LightingEnabled(bool const& value) = 0;
		virtual void EnableDefaultLighting() = 0;
	};
}

#endif