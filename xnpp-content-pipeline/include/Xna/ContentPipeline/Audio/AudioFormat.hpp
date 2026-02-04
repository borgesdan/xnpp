#ifndef XNA_AUDIO_AUDIOFORMAT_HPP
#define XNA_AUDIO_AUDIOFORMAT_HPP

#include <vector>
#include <cstdint>
#include <span>
#include <Xna/Framework/Audio/AudioFormat.hpp>
#include "WaveFormatEx.hpp"

namespace Xna {
	struct InternalAudioFormat {
		inline InternalAudioFormat(AudioFormat const& audioFormat) {
			Convert(audioFormat);
		}

		InternalAudioFormat(std::vector<uint8_t> const& waveFormatEx);
		InternalAudioFormat(WaveFormatEx const& waveFormatEx);

		int32_t Format() const noexcept { return formatTag; }
		int32_t ChannelCount() const noexcept { return channels; }
		int32_t SampleRate() const noexcept { return sampleRate; }
		int32_t AverageBytesPerSecond() const noexcept { return avgBytesPerSec; }
		int32_t BlockAlign() const noexcept { return blockAlign; }
		int32_t BitsPerSample() const noexcept { return bitsPerSample; }

		const std::vector<uint8_t>& NativeWaveFormat() const noexcept {
			return rawFormatBuffer;
		}

		void Convert(AudioFormat const& audioFormat);
	private:

		std::vector<uint8_t> rawFormatBuffer;
		int32_t formatTag{ 0 };
		int32_t channels{ 0 };
		int32_t sampleRate{ 0 };
		int32_t avgBytesPerSec{ 0 };
		int32_t blockAlign{ 0 };
		int32_t bitsPerSample{ 0 };
	};
}

#endif