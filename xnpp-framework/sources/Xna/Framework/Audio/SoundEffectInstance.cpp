#include "Xna/Framework/Audio/SoundEffectInstance.hpp"

namespace Xna {
	SoundEffectInstance::SoundEffectInstance(Xna::SoundEffect const& parentEffect, bool fireAndForget) {
		impl = std::make_shared<Implementation>();
		impl->platform = PlatformNS::ISoundEffectInstance::Create();
		SoundEffect(parentEffect);
		AllocateVoice();
		Volume(1);
		Pitch(0);
		Pan(0);
		impl->isFireAndForget = fireAndForget;		
	}

	void SoundEffectInstance::Play() {
		impl->platform->Play();
	}

	void SoundEffectInstance::Stop(bool immediate) {
		impl->platform->Stop();
	}
	void SoundEffectInstance::Pause() {
		impl->platform->Pause();
	}
	void SoundEffectInstance::Resume() {
		if (State() != SoundState::Paused && State() != SoundState::Stopped)
			return;

		impl->platform->Play();
	}
	
	void SoundEffectInstance::Apply3D(std::vector<AudioListener> const& listener, AudioEmitter emitter) {
		throw CSharp::NotSupportedException();
	}
	
	void SoundEffectInstance::Volume(float value) {
		if (value < 0 || value > 1)
			throw CSharp::ArgumentOutOfRangeException("value");
		
		impl->platform->SetVolume(value);
		impl->currentVolume = value;
	}

	void SoundEffectInstance::Pitch(float value) {
		if (value < -1 || value > 1)
			throw CSharp::ArgumentOutOfRangeException("value");

		impl->platform->SetPitch(value);
		impl->currentPitch = value;
	}

	void SoundEffectInstance::Pan(float value) {
		if (value < -1 || value > 1)
			throw CSharp::ArgumentOutOfRangeException("value");

		impl->platform->SetPan(value);
		impl->currentPan = value;
	}
	
	void SoundEffectInstance::AllocateVoice() {		
		auto& seImpl = impl->effect->impl->platform;
		impl->platform->Load(
			seImpl.get()
		);
	}

	void SoundEffectInstance::DeallocateVoice() {
		impl->platform = nullptr;
	}

	SoundState SoundEffectInstance::State() const {
		const auto state = impl->platform->GetState();

		switch (state)
		{
		case PlatformNS::MediaState::Playing:
			return SoundState::Playing;
		case PlatformNS::MediaState::Paused:
			return SoundState::Paused;
		default:
			return SoundState::Stopped;
		}
	}

	void SoundEffectInstance::IsLooped(bool value) {
		impl->looped = value;
		impl->platform->IsLooped(value);
	}
}