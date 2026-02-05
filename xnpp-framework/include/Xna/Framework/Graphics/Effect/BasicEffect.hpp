#ifndef XNA_FRAMEWORK_GRAPHICS_BASICEFFECT_HPP
#define XNA_FRAMEWORK_GRAPHICS_BASICEFFECT_HPP

#include "Effect.hpp"
#include "IEffectMatrices.hpp"
#include "IEffectFog.hpp"
#include "IEffectLights.hpp"
#include "Xna/Platform/Platform.hpp"
#include <memory>
#include <optional>
#include "Xna/Internal/Export.hpp"

namespace Xna {
	class BasicEffect final : Effect, IEffectMatrices, IEffectLights, IEffectFog {
	public:
		XNPP_API BasicEffect(GraphicsDevice const& device);
		~BasicEffect() override = default;

		//
		// IEffectMatrices
		//

		inline Matrix World() const override { return impl->world; }
		inline Matrix View() const override { return impl->view; }
		inline Matrix Projection() const override { return impl->projection; }
		XNPP_API void World(Matrix const& value) override;
		XNPP_API void View(Matrix const& value) override;
		XNPP_API void Projection(Matrix const& value) override;

		//
		// IEffectLights
		//

		inline DirectionalLight DirectionalLight0() const override { return impl->light0; }
		inline DirectionalLight DirectionalLight1() const override { return impl->light1; }
		inline DirectionalLight DirectionalLight2() const override { return impl->light2; }
		inline Vector3 AmbientLightColor() const override { return impl->ambientLightColor; }
		inline bool LightingEnabled() const override { return impl->lightingEnabled; }
		XNPP_API void AmbientLightColor(Vector3 const& value) override;
		XNPP_API void LightingEnabled(bool const& value) override;
		XNPP_API void EnableDefaultLighting() override;

		//
		// IEffectFog
		//

		inline bool FogEnabled() const override { return impl->fogEnabled; }
		inline bool FogStart() const override { return impl->fogStart; }
		inline bool FogEnd() const override { return impl->fogEnd; }
		inline Vector3 FogColor() const override {
			return impl->fogColor;
			//TODO: [!] get => this.fogColorParam.GetValueVector3();
		}
		XNPP_API void FogEnabled(bool value) override;
		XNPP_API void FogStart(bool value) override;
		XNPP_API void FogEnd(bool value) override;
		XNPP_API void FogColor(Vector3 const& value) override;

		//
		// BasicEffect
		//

		inline Vector3 DiffuseColor() const { return impl->diffuseColor; }
		inline Vector3 EmissiveColor() const { return impl->emissiveColor; }
		inline Vector3 SpecularColor() const {
			return impl->specularColor;
			//TODO: [!] get => this.specularColorParam.GetValueVector3();
		}
		inline float SpecularPower() const {
			return impl->specularPower;
			//TODO: [!] get => this.specularPowerParam.GetValueSingle();
		}
		inline float Alpha() const { return impl->alpha; }
		inline bool PreferPerPixelLighting() const { return impl->preferPerPixelLighting; }
		inline bool TextureEnabled() const { return impl->textureEnabled; }
		inline std::optional<Texture2D> Texture() const {
			return impl->texture;
			//TODO: [!] get => this.textureParam.GetValueTexture2D();
		}
		inline bool VertexColorEnabled() const { return impl->vertexColorEnabled; }
		XNPP_API void DiffuseColor(Vector3 const& value);
		XNPP_API void EmissiveColor(Vector3 const& value);
		XNPP_API void SpecularColor(Vector3 const& value);
		XNPP_API void SpecularPower(float value);
		XNPP_API void Alpha(float value);
		XNPP_API void PreferPerPixelLighting(bool value);
		XNPP_API void TextureEnabled(bool value);
		void Texture(Texture2D const& value) {
			//TODO: [!] set => this.textureParam.SetValue((Microsoft.Xna.Framework.Graphics.Texture) value);
		}
		XNPP_API void VertexColorEnabled(bool value);

		inline BasicEffect(std::nullptr_t) : Effect(nullptr) { impl = nullptr; }
		inline bool operator==(BasicEffect const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	protected:
		void OnApply() override;

	private:
		struct Implementation {
			//TODO: [!] não usamos EffectParameter
			//Campos adicionados pois não usamos EffectParameter
			Vector3 fogColor = Vector3::Zero();
			Vector3 specularColor = Vector3::Zero();
			float specularPower{0};
			std::optional<Texture2D> texture;

			bool lightingEnabled{ false };
			bool preferPerPixelLighting{false};
			bool fogEnabled{ false };
			bool textureEnabled{ false };
			bool vertexColorEnabled{ false };
			Matrix world{ Matrix::Identity() };
			Matrix view = { Matrix::Identity() };
			Matrix projection = { Matrix::Identity() };
			Vector3 diffuseColor{ Vector3::One() };
			Vector3 emissiveColor{ Vector3::Zero() };
			Vector3 ambientLightColor{ Vector3::Zero() };
			float alpha{ 1.0f };
			DirectionalLight light0{};
			DirectionalLight light1{};
			DirectionalLight light2{};
			float fogStart{ 0 };
			float fogEnd{ 1.0f };
			EffectDirtyFlags dirtyFlags{ EffectDirtyFlags::All };

			PlatformImpl::BasicEffectImpl platformImpl;
		};

		std::shared_ptr<Implementation> impl;

		friend struct Platform;
	};
}

#endif