#ifndef XNA_FRAMEWORK_GRAPHICS_DIRECTIONALLIGHT_HPP
#define XNA_FRAMEWORK_GRAPHICS_DIRECTIONALLIGHT_HPP

#include "Xna/Framework/Vector3.hpp"

namespace Xna {
	struct DirectionalLight final {
	public:
		constexpr DirectionalLight() = default;

		constexpr DirectionalLight(
			Vector3 const& directionParameter,
			Vector3 const& diffuseColorParameter,
			Vector3 const& specularColorParameter)
			: cachedDirection(directionParameter),
			cachedDiffuseColor(diffuseColorParameter),
			cachedSpecularColor(specularColorParameter) {
		}

		constexpr bool Enabled() const noexcept { return enabled; }
		constexpr Vector3 Direction() const noexcept { return cachedDirection; }
		constexpr Vector3 DiffuseColor() const noexcept { return cachedDiffuseColor; }
		constexpr Vector3 SpecularColor() const noexcept { return cachedSpecularColor; }

		constexpr void Enabled(bool value) noexcept { enabled = value; }
		constexpr void Direction(Vector3 const& value) noexcept { cachedDirection = value; }
		constexpr void DiffuseColor(Vector3 const& value) noexcept { cachedDiffuseColor = value; }
		constexpr void SpecularColor(Vector3 const& value) noexcept { cachedSpecularColor = value; }

	private:
		//TODO: [!] não usamos EffectParameter
		//EffectParameter directionParam;
		//EffectParameter diffuseColorParam;
		//EffectParameter specularColorParam;

		bool enabled;
		Vector3 cachedDirection{ Vector3::Down() };
		Vector3 cachedDiffuseColor{ Vector3::One() };
		Vector3 cachedSpecularColor{ Vector3::Zero() };

		friend struct Platform;
	};
}

#endif