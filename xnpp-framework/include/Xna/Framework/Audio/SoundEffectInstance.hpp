#ifndef XNA_FRAMEWORK_AUDIO_SOUNDEFFECTINSTANCE_HPP
#define XNA_FRAMEWORK_AUDIO_SOUNDEFFECTINSTANCE_HPP

#include "Xna/CSharp/IO/Stream.hpp"
#include <cstdint>
#include <limits>
#include "Shared.hpp"
#include "AudioEmitter.hpp"
#include "AudioListener.hpp"
#include "SoundEffect.hpp"
#include "Xna/Platform/Platform.hpp"
#include "Xna/Internal/Export.hpp"

namespace Xna {

	//Provides a single playing, paused, or stopped instance of a SoundEffect sound.
	class SoundEffectInstance {
	public:
		//Plays or resumes a SoundEffectInstance.
		XNPP_API virtual void Play();
		//Stops playing a SoundEffectInstance.
		inline void Stop() { Stop(true); }
		XNPP_API void Stop(bool immediate);
		//Pauses a SoundEffectInstance.
		XNPP_API void Pause();
		//Resumes playback for a SoundEffectInstance.
		XNPP_API void Resume();

		//Applys 3D positioning to the sound using a single listener.
		void Apply3D(AudioListener listener, AudioEmitter emitter) {
			Apply3D(std::vector<AudioListener>({ listener }), emitter);
		}

		//Applys 3D position to the sound using multiple listeners.
		XNPP_API void Apply3D(std::vector<AudioListener> const& listener, AudioEmitter emitter);
		
		//Gets or sets the volume of the SoundEffectInstance.
		inline float Volume() const { return impl->currentVolume; }
		//Gets or sets the volume of the SoundEffectInstance.
		XNPP_API void Volume(float value);
		
		//Gets or sets the pitch adjustment for the SoundEffectInstance.
		inline float Pitch() const { return impl->currentPitch; }
		//Gets or sets the pitch adjustment for the SoundEffectInstance.
		XNPP_API void Pitch(float value);

		//Gets or sets the panning for the SoundEffectInstance.
		inline float Pan() const { return impl->currentPan; }
		//Gets or sets the panning for the SoundEffectInstance.
		XNPP_API void Pan(float value);

		//Gets or sets a value that indicates whether looping is enabled for the SoundEffectInstance.
		inline virtual bool IsLooped() const { return impl->looped; }
		//Gets or sets a value that indicates whether looping is enabled for the SoundEffectInstance.
		inline virtual void IsLooped(bool value) { impl->looped = value; }
		
		//Gets the current state (playing, paused, or stopped) of the SoundEffectInstance.
		XNPP_API SoundState State() const;

		inline SoundEffectInstance(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(SoundEffectInstance const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		//internal
		SoundEffectInstance(Xna::SoundEffect const& parentEffect, bool fireAndForget);

		bool IsFireAndForget() const { return impl->isFireAndForget; }
		virtual void AllocateVoice();
		void DeallocateVoice();

		inline std::optional<Xna::SoundEffect> SoundEffect() const { return impl->effect; }
		void SoundEffect(std::optional<Xna::SoundEffect> const& value) { impl->effect = value; }

	protected:
		SoundEffectInstance() {
			impl = std::make_shared<Implementation>();
		}

	private:
		struct Implementation {
			std::optional<Xna::SoundEffect> effect;
			bool looped{false};
			float currentVolume{ 1 };
			float currentPitch{0};
			float currentPan{0};
			bool is3d{false};
			bool isFireAndForget{false};
			bool isPacketSubmitted{false};
			std::vector<UnsafeNativeStructures::XACT_LISTENER_DATA> listenerData;
			uint32_t voiceHandle{ std::numeric_limits<uint32_t>::max() };

			PlatformImpl::SoundEffectInstanceImpl platformImpl;
		};

		std::shared_ptr<Implementation> impl;

		friend class Xna::SoundEffect;
		friend struct Platform;
	};
}

#endif