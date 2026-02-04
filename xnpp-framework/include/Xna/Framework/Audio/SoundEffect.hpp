#ifndef XNA_FRAMEWORK_AUDIO_SOUNDEFFECT_HPP
#define XNA_FRAMEWORK_AUDIO_SOUNDEFFECT_HPP

#include <cstdint>
#include <string>
#include <stack>
#include <unordered_map>
#include "Shared.hpp"
#include "LoopRegion.hpp"
#include "Xna/CSharp/TimeSpan.hpp"
#include "Xna/CSharp/IO/Stream.hpp"
#include "Xna/Platform/Platform.hpp"

namespace Xna {
	class SoundEffectInstance;

	//Provides a loaded sound resource. You can play multiple instances of the SoundEffect by calling Play.
	class SoundEffect final {
	public:
		//Creates a SoundEffect object based on the specified data stream.
		static inline SoundEffect FromStream(std::shared_ptr<CSharp::Stream> const& stream) {
			return SoundEffect(stream); 
		}
		
		//Initializes a new instance of SoundEffect based on an audio buffer, sample rate, and number of audio channels.
		SoundEffect(std::vector<uint8_t> const& buffer, int32_t sampleRate, AudioChannels channels);

		//nitializes a new instance of SoundEffect with specified parameters such as audio sample rate, channels, looping criteria, and a buffer to hold the audio.
		inline SoundEffect(
			std::vector<uint8_t> const& buffer,
			int32_t offset,
			int32_t count,
			int32_t sampleRate,
			AudioChannels channels,
			int32_t loopStart,
			int32_t loopLength) {
			FromBuffer(buffer, offset, count, sampleRate, channels, loopStart, loopLength);
		}
		
		//Creates a new SoundEffectInstance for this SoundEffect.
		std::shared_ptr<SoundEffectInstance> CreateInstance();
		
		//Plays a sound.
		inline bool Play() { return Play(1.0f, 0, 0); }
		//Plays a sound.
		bool Play(float volume, float pitch, float pan);

		//Returns the size of the audio sample based on duration, sample rate, and audio channels.
		static int GetSampleSizeInBytes(
			CSharp::TimeSpan const& duration,
			int32_t sampleRate,
			AudioChannels channels);

		//Returns the sample duration based on the specified sample size and sample rate.
		static CSharp::TimeSpan GetSampleDuration(
			int32_t sizeInBytes,
			int32_t sampleRate,
			AudioChannels channels);

		//Gets or sets the asset name of the SoundEffect.
		inline std::string Name() const { return impl->effectName; }
		//Gets or sets the asset name of the SoundEffect.
		inline void Name(std::string const& value) { impl->effectName = value; }

		//Gets or sets the duration of the SoundEffect.
		inline CSharp::TimeSpan Duration() { return impl->duration; }

		//Gets or sets the master volume that affects all SoundEffectInstance sounds.
		inline static float MasterVolume() { Implementation::currentVolume; }
		//Gets or sets the master volume that affects all SoundEffectInstance sounds.
		static void MasterVolume(float value);

		//Returns the speed of sound: 343.5 meters per second.
		static float SpeedOfSound() { return Implementation::speedOfSound; }
		//Returns the speed of sound: 343.5 meters per second.
		static void SpeedOfSound(float value);

		//Gets or sets a value that adjusts the effect of doppler calculations on the sound (emitter).
		static float DopplerScale() { return Implementation::dopplerScale; }
		//Gets or sets a value that adjusts the effect of doppler calculations on the sound (emitter).
		static void DopplerScale(float value);

		//Gets or sets a value that adjusts the effect of distance calculations on the sound (emitter).
		static float DistanceScale() { return Implementation::distanceScale; }
		//Gets or sets a value that adjusts the effect of distance calculations on the sound (emitter).
		static void DistanceScale(float value);

		//Internal constructor
		inline SoundEffect() { impl = std::make_shared<Implementation>(); }

		inline SoundEffect(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(SoundEffect const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }
	
	private:		
		SoundEffect(std::shared_ptr<CSharp::Stream> const& stream);

		inline SoundEffect(std::vector<uint8_t> const& format, std::vector<uint8_t> const& data,
			int32_t loopStart, int32_t loopLength, CSharp::TimeSpan const& duration) {
			Create(format, data, 0, data.size(), loopStart, loopLength, duration);
		}

		void FromBuffer(
			std::vector<uint8_t> const& buffer,
			size_t offset,
			size_t count,
			size_t sampleRate,
			AudioChannels channels,
			size_t loopStart,
			size_t loopLength);

		void Create(
			std::vector<uint8_t> const& format,
			std::vector<uint8_t> const& data,
			size_t offset,
			size_t count,
			size_t loopStart,
			size_t loopLength,
			CSharp::TimeSpan const& duration);

		void ChildDestroyed(std::shared_ptr<SoundEffectInstance> const& child);

		static void RecycleStoppedFireAndForgetInstances();
		void AddToInstancePool(std::shared_ptr<SoundEffectInstance> const& instance);
		
		std::shared_ptr<SoundEffectInstance> GetFromInstancePool();

		static bool IsStoppedInstance(std::shared_ptr<SoundEffectInstance> const& instance);

		inline LoopRegion Loop() const { return impl->loopInfo; }

		static inline float MaxVelocityComponent() {
			return Implementation::maxVelocityComponent;
		}

	private:
		struct Implementation {
			LoopRegion loopInfo;
			CSharp::TimeSpan duration;
			std::string effectName;
			std::vector<std::weak_ptr<SoundEffectInstance>> children;
			std::stack<std::shared_ptr<SoundEffectInstance>> instancePool;
			
			static inline float currentVolume = 1.0f;
			static inline float speedOfSound = 343.5f;
			static inline float dopplerScale = 1.0f;
			static inline float distanceScale = 1.0f;
			static inline float maxVelocityComponent = 343.499f;

			static inline std::unordered_map<std::shared_ptr<SoundEffectInstance>, bool> fireAndForgetInstances =
				std::unordered_map<std::shared_ptr<SoundEffectInstance>, bool>();
			static inline std::vector<std::shared_ptr<SoundEffectInstance>> instancesToDispose = 
				std::vector<std::shared_ptr<SoundEffectInstance>>();

			PlatformImpl::SoundEffectImpl platformImpl;
		};
		std::shared_ptr<Implementation> impl = nullptr;

		friend struct Platform;
		friend struct FrameworkDispatcher;
		friend class SoundEffectReader;
	};
}

#endif