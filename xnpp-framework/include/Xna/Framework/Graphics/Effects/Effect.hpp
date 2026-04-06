#ifndef XNA_EFFECTS_EFFECT_HPP
#define XNA_EFFECTS_EFFECT_HPP

#include "Xna/Framework/Vector4.hpp"

namespace Xna {
	struct EffectTechnique{};
	struct EffectTechniqueCollection{};
	struct EffectPass{};
	struct EffectPassCollection{};
	struct EffectParameter{};
	struct EffectParameterCollection{};
	struct EffectAnnotation{};
	struct EffectAnnotationCollection{};

	struct Effect {
		virtual ~Effect() = default;

		inline EffectTechnique& CurrentTechnique() { return currentTechnique; }
		inline EffectTechniqueCollection& Techniques() { return techniqueCollection; }
		inline EffectParameterCollection& Parameters() { return paramCollection; }

	private:
		EffectTechniqueCollection techniqueCollection;
		EffectParameterCollection paramCollection;
		EffectTechnique currentTechnique;
	};

	struct DirectionalLight {
		constexpr DirectionalLight() = default;

		constexpr DirectionalLight(
			Vector3 const& directionParameter,
			Vector3 const& diffuseColorParameter,
			Vector3 const& specularColorParameter)
			: Direction(directionParameter),
			DiffuseColor(diffuseColorParameter),
			SpecularColor(specularColorParameter) {
		}

		bool Enabled{ false };
		Vector3 Direction{ Vector3::Down() };
		Vector3 DiffuseColor{ Vector3::One() };
		Vector3 SpecularColor{ Vector3::Zero() };
	};

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

	struct IEffectMatrices {
		virtual ~IEffectMatrices() = default;
		virtual Matrix World() const = 0;
		virtual Matrix View() const = 0;
		virtual Matrix Projection() const = 0;
		virtual void World(Matrix const& value) = 0;
		virtual void View(Matrix const& value) = 0;
		virtual void Projection(Matrix const& value) = 0;
	};
}

#endif