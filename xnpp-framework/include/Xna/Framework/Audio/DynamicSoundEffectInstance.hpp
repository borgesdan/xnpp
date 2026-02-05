#ifndef XNA_FRAMEWORK_AUDIO_DYNAMICSOUNDEFFECTINSTANCE_HPP
#define XNA_FRAMEWORK_AUDIO_DYNAMICSOUNDEFFECTINSTANCE_HPP

#include "SoundEffectInstance.hpp"
#include "AudioFormat.hpp"
#include "Xna/Platform/Platform.hpp"
#include "Xna/Internal/Export.hpp"

namespace Xna {
	class DynamicSoundEffectInstance final : public SoundEffectInstance {
	public:
		XNPP_API DynamicSoundEffectInstance(size_t sampleRate, AudioChannels channels);
		
		inline void SubmitBuffer(std::vector<uint8_t> const& buffer) {
			SubmitBuffer(buffer, 0, buffer.size());
		}

		XNPP_API void SubmitBuffer(std::vector<uint8_t> const& buffer, size_t offset, size_t count);
		XNPP_API CSharp::TimeSpan GetSampleDuration(size_t sizeInBytes);
		XNPP_API size_t GetSampleSizeInBytes(CSharp::TimeSpan duration);

		inline bool IsLooped() const override { return false; }
		inline void IsLooped(bool value) override { CSharp::InvalidOperationException("InvalidCall"); }
		
		XNPP_API void Play() override;
		//TODO: verificar Stop, Pause e Resume
		
		inline int32_t PendingBufferCount() const { return GetPendingBufferCount(); }

		inline DynamicSoundEffectInstance(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(DynamicSoundEffectInstance const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		void AllocateVoice() override;
		void AllocateData(std::vector<uint8_t> const& buffer, size_t offset, size_t count);
		int32_t GetPendingBufferCount() const;

		struct Implementation {
			std::optional<AudioFormat> format;
			PlatformImpl::DynamicSoundEffectInstanceImpl platformImpl;
		};

		std::shared_ptr<Implementation> impl;

		friend struct Platform;
	};
}

#endif