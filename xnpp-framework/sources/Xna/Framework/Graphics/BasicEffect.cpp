#include "Xna/Framework/Graphics/Effect/BasicEffect.hpp"
#include "Xna/Framework/Graphics/Effect/EffectHelpers.hpp"

namespace Xna {
	BasicEffect::BasicEffect(GraphicsDevice const& device) : Effect(device, {}) {
		impl = std::make_shared<Implementation>();
		impl->light0.Enabled(true);
		SpecularColor(Vector3::One());
		SpecularPower(16.0f);

		Platform::Effect_BasicEffect_Create(*this, device);
	}

	void BasicEffect::World(Matrix const& value) {
		impl->world = value;
		impl->dirtyFlags = static_cast<EffectDirtyFlags>((int)impl->dirtyFlags | (int)EffectDirtyFlags::WorldViewProj | (int)EffectDirtyFlags::World | (int)EffectDirtyFlags::Fog);
	}

	void BasicEffect::View(Matrix const& value) {
		impl->view = value;
		impl->dirtyFlags = static_cast<EffectDirtyFlags>((int)impl->dirtyFlags | (int)EffectDirtyFlags::WorldViewProj | (int)EffectDirtyFlags::EyePosition | (int)EffectDirtyFlags::Fog);
	}

	void BasicEffect::Projection(Matrix const& value) {
		impl->projection = value;
		impl->dirtyFlags = static_cast<EffectDirtyFlags>((int)impl->dirtyFlags | (int)EffectDirtyFlags::WorldViewProj);
	}

	void BasicEffect::DiffuseColor(Vector3 const& value) {
		impl->diffuseColor= value;
		impl->dirtyFlags = static_cast<EffectDirtyFlags>((int)impl->dirtyFlags | (int)EffectDirtyFlags::MaterialColor);
	}

	void BasicEffect::EmissiveColor(Vector3 const& value) {
		impl->emissiveColor= value;
		impl->dirtyFlags = static_cast<EffectDirtyFlags>((int)impl->dirtyFlags | (int)EffectDirtyFlags::MaterialColor);
	}

	void BasicEffect::SpecularColor(Vector3 const& value) {
		impl->specularColor = value;
		impl->dirtyFlags = static_cast<EffectDirtyFlags>((int)impl->dirtyFlags | (int)EffectDirtyFlags::MaterialColor);
		//TODO: [!] set => this.specularColorParam.SetValue(value);
	}

	void BasicEffect::SpecularPower(float value) {
		impl->specularPower = value;
		impl->dirtyFlags = static_cast<EffectDirtyFlags>((int)impl->dirtyFlags | (int)EffectDirtyFlags::MaterialColor);
		//TODO: [!] set => this.specularPowerParam.SetValue(value);
	}

	void BasicEffect::Alpha(float value) {
		impl->alpha = value;
		impl->dirtyFlags = static_cast<EffectDirtyFlags>((int)impl->dirtyFlags | (int)EffectDirtyFlags::MaterialColor);
	}

	void BasicEffect::LightingEnabled(bool const& value) {
		if (impl->lightingEnabled == value)
			return;

		impl->lightingEnabled = value;
		impl->dirtyFlags = static_cast<EffectDirtyFlags>((int)impl->dirtyFlags | (int)EffectDirtyFlags::MaterialColor | (int)EffectDirtyFlags::ShaderIndex);
	}

	void BasicEffect::PreferPerPixelLighting(bool value) {
		if (impl->preferPerPixelLighting == value)
			return;

		impl->preferPerPixelLighting = value;
		impl->dirtyFlags = static_cast<EffectDirtyFlags>((int)impl->dirtyFlags | (int)EffectDirtyFlags::ShaderIndex);
	}

	void BasicEffect::AmbientLightColor(Vector3 const& value) {
		impl->ambientLightColor = value;
		impl->dirtyFlags = static_cast<EffectDirtyFlags>((int)impl->dirtyFlags | (int)EffectDirtyFlags::MaterialColor);
	}

	void BasicEffect::FogEnabled(bool value) {
		if (impl->fogEnabled == value)
			return;

		impl->fogEnabled = value;
		impl->dirtyFlags = static_cast<EffectDirtyFlags>((int)impl->dirtyFlags | (int)EffectDirtyFlags::FogEnable | (int)EffectDirtyFlags::ShaderIndex);
	}

	void BasicEffect::FogStart(bool value) {
		impl->fogStart = value;
		impl->dirtyFlags = static_cast<EffectDirtyFlags>((int)impl->dirtyFlags | (int)EffectDirtyFlags::Fog);
	}

	void BasicEffect::FogEnd(bool value) {
		impl->fogEnd = value;
		impl->dirtyFlags = static_cast<EffectDirtyFlags>((int)impl->dirtyFlags | (int)EffectDirtyFlags::Fog);
	}

	void BasicEffect::FogColor(Vector3 const& value) {
		impl->fogColor = value;
		impl->dirtyFlags = static_cast<EffectDirtyFlags>((int)impl->dirtyFlags | (int)EffectDirtyFlags::Fog);
		//TODO: [!] set => this.fogColorParam.SetValue(value);
	}

	void BasicEffect::EnableDefaultLighting() {
		impl->lightingEnabled = true;
		//TODO: [!] this.AmbientLightColor = EffectHelpers.EnableDefaultLighting(this.light0, this.light1, this.light2);
	}

	void BasicEffect::TextureEnabled(bool value) {
		if (impl->textureEnabled == value)
			return;

		impl->textureEnabled = value;
		impl->dirtyFlags = static_cast<EffectDirtyFlags>((int)impl->dirtyFlags | (int)EffectDirtyFlags::ShaderIndex);
	}

	void BasicEffect::VertexColorEnabled(bool value) {
		if (impl->vertexColorEnabled == value)
			return;

		impl->vertexColorEnabled = value;
		impl->dirtyFlags = static_cast<EffectDirtyFlags>((int)impl->dirtyFlags | (int)EffectDirtyFlags::ShaderIndex);
	}

	void BasicEffect::OnApply() {
		Platform::Effect_BasicEffect_SetValues(*this);
		Platform::Effect_BasicEffect_Apply(*this);
	}
}