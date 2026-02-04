#include "Xna/Framework/Audio/SoundEffectInstance.hpp"

namespace Xna {
	SoundEffectInstance::SoundEffectInstance(Xna::SoundEffect const& parentEffect, bool fireAndForget) {
		impl = std::make_shared<Implementation>();
		SoundEffect(parentEffect);
		AllocateVoice();
		Volume(1);
		Pitch(0);
		Pan(0);
		impl->isFireAndForget = fireAndForget;
	}

	void SoundEffectInstance::Play() {
		Platform::SoundEffect_SetState(*this, Platform::SoundEffect_State::Play);
	}

	void SoundEffectInstance::Stop(bool immediate) {
		Platform::SoundEffect_SetState(*this, Platform::SoundEffect_State::Stop, immediate);
	}
	void SoundEffectInstance::Pause() {
		Platform::SoundEffect_SetState(*this, Platform::SoundEffect_State::Pause);
	}
	void SoundEffectInstance::Resume() {
		if (State() != SoundState::Paused && State() != SoundState::Stopped)
			return;

		Platform::SoundEffect_SetState(*this, Platform::SoundEffect_State::Resume);
	}
	
	void SoundEffectInstance::Apply3D(std::vector<AudioListener> const& listener, AudioEmitter emitter) {
		Platform::SoundEffect_Apply3D(*this, listener, emitter);
	}
	
	void SoundEffectInstance::Volume(float value) {
		if (value < 0 || value > 1)
			throw CSharp::ArgumentOutOfRangeException("value");

		Platform::SoundEffect_SetAttributes(*this, value, std::nullopt, std::nullopt);
		impl->currentVolume = value;
	}

	void SoundEffectInstance::Pitch(float value) {
		if (value < -1 || value > 1)
			throw CSharp::ArgumentOutOfRangeException("value");

		Platform::SoundEffect_SetAttributes(*this, std::nullopt, std::nullopt, value);
		impl->currentPitch = value;
	}

	void SoundEffectInstance::Pan(float value) {
		if (value < -1 || value > 1)
			throw CSharp::ArgumentOutOfRangeException("value");

		Platform::SoundEffect_SetAttributes(*this, std::nullopt, value, std::nullopt);
		impl->currentPan = value;
	}
	
	void SoundEffectInstance::AllocateVoice() {
		Platform::SoundEffect_CreateInstance(*this);
	}

	void SoundEffectInstance::DeallocateVoice() {
		Platform::SoundEffect_DeleteInstance(*this);
	}

	SoundState SoundEffectInstance::State() const {
		return Platform::SoundEffect_GetState(*this);
	}
}