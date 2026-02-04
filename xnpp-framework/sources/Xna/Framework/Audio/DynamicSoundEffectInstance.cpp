#include "Xna/Framework/Audio/DynamicSoundEffectInstance.hpp"
#include "Xna/CSharp/Exception.hpp"
#include "Xna/Misc.hpp"

namespace Xna {
	DynamicSoundEffectInstance::DynamicSoundEffectInstance(size_t sampleRate, AudioChannels channels) {
		if (sampleRate < 8000 || sampleRate > 48000)
			throw CSharp::ArgumentOutOfRangeException("samplerRate");
		
		impl = std::make_shared<Implementation>();

		if (channels >= AudioChannels::Mono && channels <= AudioChannels::Stereo)
			impl->format = AudioFormat::Create(sampleRate, channels, 16);
		else
		 throw CSharp::ArgumentOutOfRangeException("channels");

		AllocateVoice();
	}

	void DynamicSoundEffectInstance::AllocateVoice() {
		Platform::SoundEffect_CreateInstance(*this);
	}

	void DynamicSoundEffectInstance::SubmitBuffer(std::vector<uint8_t> const& buffer, size_t offset, size_t count) {
		if(buffer.size() == 0 || !impl->format->IsAligned(buffer.size()))
			throw CSharp::ArgumentException("DynamicSoundEffectInvalidBuffer");

		if (offset >= 0 && offset < buffer.size()) {
			if (impl->format->IsAligned(offset)) {
				size_t num;
				if (!Xna::Misc::safe_addition(offset, count, num)) {
					throw CSharp::ArgumentException("DynamicSoundEffectInvalidPlayLength");
				}

				if (count <= 0 || num > buffer.size() || !impl->format->IsAligned(count))
					throw CSharp::ArgumentException("DynamicSoundEffectInvalidPlayLength");

				AllocateData(buffer, offset, count);

				return;
			}
		}
	}

	void DynamicSoundEffectInstance::AllocateData(std::vector<uint8_t> const& buffer, size_t offset, size_t count) {
		Platform::SoundEffect_SubmitBuffer(*this, buffer, offset, count);
	}

	CSharp::TimeSpan DynamicSoundEffectInstance::GetSampleDuration(size_t sizeInBytes) {
		if (sizeInBytes < 0)
			throw CSharp::ArgumentException("InvalidBufferSize");

		return sizeInBytes == 0 
			? CSharp::TimeSpan::Zero() 
			: impl->format->DurationFromSize(sizeInBytes);
	}

	size_t DynamicSoundEffectInstance::GetSampleSizeInBytes(CSharp::TimeSpan duration) {
		if (duration.TotalMilliseconds() < 0.0 || duration.TotalMilliseconds() > static_cast<double>(std::numeric_limits<int32_t>::max()))
			throw CSharp::ArgumentOutOfRangeException("duration");

		if (duration == CSharp::TimeSpan::Zero())
			return 0;
		
		return impl->format->SizeFromDuration(duration);
	}

	int32_t DynamicSoundEffectInstance::GetPendingBufferCount() const {
		return Platform::SoundEffect_GetPendingBufferCount(*this);
	}

	void DynamicSoundEffectInstance::Play() {
		Platform::SoundEffect_SetState(*this, Platform::SoundEffect_State::Play);
	}
}