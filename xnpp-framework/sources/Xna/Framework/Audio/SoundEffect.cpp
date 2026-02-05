#include "Xna/Framework/Audio/SoundEffect.hpp"
#include "Xna/Framework/Audio/SoundEffectInstance.hpp"
#include "Xna/Framework/Audio/WavFile.hpp"
#include "Xna/Framework/Audio/AudioHelper.hpp"
#include <limits>
#include "Xna/Internal/Misc.hpp"

namespace Xna {
	SoundEffect::SoundEffect(std::shared_ptr<CSharp::Stream> const& stream) {
		const auto wavFile = WavFile::Open(stream);
		Create(wavFile.Format()->RawBuffer, *wavFile.Data(), 0, wavFile.Data()->size(), wavFile.LoopStart(), wavFile.LoopLength(), wavFile.Duration());
	}

	void SoundEffect::Create(std::vector<uint8_t> const& format, std::vector<uint8_t> const& data, size_t offset, size_t count, size_t loopStart, size_t loopLength, CSharp::TimeSpan const& duration) {
		impl = std::make_shared<Implementation>();

		impl->loopInfo.Start = loopStart;
		impl->loopInfo.Length = loopLength;
		impl->duration = duration;

		Platform::SoundEffect_Create(*this, format, data, offset, count, loopStart, loopLength);
	}

	SoundEffect::SoundEffect(std::vector<uint8_t> const& buffer, int32_t sampleRate, AudioChannels channels) {
		if (buffer.empty())
			throw CSharp::ArgumentException("DynamicSoundEffectInvalidBuffer");

		FromBuffer(buffer, 0, buffer.size(), sampleRate, channels, 0, 0);
	}

	void SoundEffect::FromBuffer(std::vector<uint8_t> const& buffer,
		size_t offset,
		size_t count,
		size_t sampleRate,
		AudioChannels channels,
		size_t loopStart,
		size_t loopLength) {

		if (sampleRate < 8000 || sampleRate > 48000)
			throw CSharp::ArgumentOutOfRangeException("sampleRate");

		std::optional<AudioFormat> audioFormat;

		if (channels >= AudioChannels::Mono && channels <= AudioChannels::Stereo)
			audioFormat = AudioFormat::Create(sampleRate, channels, 16);
		else
			throw CSharp::ArgumentOutOfRangeException("channels");

		if (buffer.empty() || buffer.size() == 0 || !audioFormat->IsAligned(buffer.size()))
			throw CSharp::ArgumentException("DynamicSoundEffectInvalidBuffer");

		if (offset >= 0 && offset < buffer.size())
		{
			if (audioFormat->IsAligned(offset))
			{
				size_t num1;
				if (!Xna::Misc::safe_addition(offset, count, num1))
					throw CSharp::ArgumentException("DynamicSoundEffectInvalidPlayLength");

				if (num1 <= buffer.size() && count > 0)
				{
					if (audioFormat->IsAligned(count))
					{
						size_t num2;
						if (!Xna::Misc::safe_addition(loopStart, loopLength, num2))
							throw CSharp::ArgumentException("DynamicSoundEffectInvalidLoopRegion");

						const auto num3 = count / static_cast<size_t>(audioFormat->BlockAlign);

						if (loopStart < 0 || loopLength < 0 || num2 > num3)
							throw CSharp::ArgumentException("DynamicSoundEffectInvalidLoopRegion");

						if (loopLength == 0)
						{
							loopStart = 0;
							loopLength = num3;
						}

						Create(audioFormat->RawBuffer, buffer, offset, count, loopStart, loopLength, audioFormat->DurationFromSize(count));
						return;
					}
				}

				throw CSharp::ArgumentException("DynamicSoundEffectInvalidPlayLength");
			}
		}

		throw CSharp::ArgumentException("DynamicSoundEffectInvalidOffset");
	}

	std::shared_ptr<SoundEffectInstance> SoundEffect::CreateInstance() {
		auto instance = new SoundEffectInstance(*this, true);
		std::shared_ptr<SoundEffectInstance> target = nullptr;
		target.reset(instance);
		impl->children.push_back(target);
		Implementation::fireAndForgetInstances.emplace(target, true); // Adicionar para persistir os dados em memória

		return target;
	}

	bool SoundEffect::Play(float volume, float pitch, float pan) {
		std::shared_ptr<SoundEffectInstance> soundEffectInstance = nullptr;

		try {
			soundEffectInstance = GetFromInstancePool();

			if (soundEffectInstance != nullptr)
			{
				soundEffectInstance->SoundEffect(*this);
				soundEffectInstance->AllocateVoice();
			}
			else {
				auto instance = new SoundEffectInstance(*this, true);
				soundEffectInstance.reset(instance);
			}				

			impl->children.push_back(soundEffectInstance);

			soundEffectInstance->Volume(volume);
			soundEffectInstance->Pitch(pitch);
			soundEffectInstance->Pan(pan);
			soundEffectInstance->Play();

			Implementation::fireAndForgetInstances.emplace(soundEffectInstance, true);

			return true;
		}
		catch (std::exception& ex) {
			if (soundEffectInstance != nullptr)
				AddToInstancePool(soundEffectInstance);

			return false;
		}
	}

	int SoundEffect::GetSampleSizeInBytes(CSharp::TimeSpan const& duration, int32_t sampleRate, AudioChannels channels) {
		if (duration.TotalMilliseconds() < 0.0 || duration.TotalMilliseconds() > static_cast<double>(std::numeric_limits<int32_t>::max()))
			throw CSharp::ArgumentOutOfRangeException("duration");
		if (sampleRate < 8000 || sampleRate > 48000)
			throw CSharp::ArgumentOutOfRangeException("sampleRate");
		if (channels < AudioChannels::Mono || channels > AudioChannels::Stereo)
			throw CSharp::ArgumentOutOfRangeException("channels");
		if (duration == CSharp::TimeSpan::Zero())
			return 0;

		return AudioHelper::GetSampleSizeInBytes(duration, sampleRate, channels);
	}

	void SoundEffect::MasterVolume(float value) {
		if (value < 0.0 || value > 1.0)
			throw CSharp::ArgumentOutOfRangeException("value");

		Platform::SoundEffect_SetMasterSoundProperties(value, std::nullopt, std::nullopt, std::nullopt);
		Implementation::currentVolume = value;
	}

	void SoundEffect::SpeedOfSound(float value) {
		if (value <= 0)
			throw CSharp::ArgumentOutOfRangeException("value");

		Implementation::speedOfSound = value;
		Implementation::maxVelocityComponent = Implementation::speedOfSound - Implementation::speedOfSound / 1000.0f;

		Platform::SoundEffect_SetMasterSoundProperties(std::nullopt, value, std::nullopt, std::nullopt);
	}

	void SoundEffect::DopplerScale(float value) {
		if (value <= 0)
			throw CSharp::ArgumentOutOfRangeException("value");

		Implementation::dopplerScale = value;
		Platform::SoundEffect_SetMasterSoundProperties(std::nullopt, std::nullopt, value, std::nullopt);
	}

	void SoundEffect::DistanceScale(float value) {
		Implementation::distanceScale = static_cast<double>(value <= 1.4012984643248171E-45) ? std::numeric_limits<float>::epsilon() : value;
		Platform::SoundEffect_SetMasterSoundProperties(std::nullopt, std::nullopt, std::nullopt, value);
	}

	void SoundEffect::ChildDestroyed(std::shared_ptr<SoundEffectInstance> const& child) {
		for (size_t index = 0; index < impl->children.size(); ++index)
		{
			auto& child1 = impl->children[index];
			auto target = child1.lock();

			if (child1.expired() || target == nullptr || target == child)
			{
				impl->children.erase(impl->children.begin() + index);
				--index;
			}
		}
	}

	void SoundEffect::RecycleStoppedFireAndForgetInstances() {
		for (auto& [key, value] : Implementation::fireAndForgetInstances)
		{
			if (IsStoppedInstance(key))
				Implementation::instancesToDispose.push_back(key);
		}

		for (auto& soundEffectInstance : Implementation::instancesToDispose)
		{
			Implementation::fireAndForgetInstances.erase(soundEffectInstance);
			soundEffectInstance->SoundEffect()->AddToInstancePool(soundEffectInstance);
		}

		Implementation::instancesToDispose.clear();
	}

	void SoundEffect::AddToInstancePool(std::shared_ptr<SoundEffectInstance> const& instance) {
		if (!instance->IsFireAndForget())
			throw CSharp::InvalidOperationException();

		instance->DeallocateVoice();
		instance->SoundEffect(std::nullopt);
		impl->instancePool.push(instance);
	}

	std::shared_ptr<SoundEffectInstance> SoundEffect::GetFromInstancePool() {
		if (impl->instancePool.size() == 0)
			return nullptr;

		auto top = impl->instancePool.top();
		impl->instancePool.pop();

		return top;
	}

	bool SoundEffect::IsStoppedInstance(std::shared_ptr<SoundEffectInstance> const& instance) {
		return instance->State() == SoundState::Stopped;
	}
}
