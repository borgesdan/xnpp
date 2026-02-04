#ifndef XNA_FRAMEWORK_AUDIO_AUDIOFORMAT_HPP
#define XNA_FRAMEWORK_AUDIO_AUDIOFORMAT_HPP

#include <vector>
#include <cstdint>
#include "Shared.hpp"
#include "Xna/CSharp/TimeSpan.hpp"

namespace Xna {
	class AudioFormat {
	public:
        static AudioFormat Create(int32_t sampleRate, AudioChannels channels, int16_t bitDepth);
        AudioFormat(std::vector<uint8_t> const& buffer);

        constexpr bool IsAligned(int32_t value) const {
            return (value % static_cast<int32_t>(BlockAlign)) == 0;
        }

        constexpr CSharp::TimeSpan DurationFromSize(int32_t sizeInBytes) const {
            return CSharp::TimeSpan::FromMilliseconds(static_cast<double>(sizeInBytes / BlockAlign) * 1000.0 / static_cast<double>(SampleRate));
        }

        constexpr int32_t SizeFromDuration(CSharp::TimeSpan const& duration) const {
            //TODO: uso de checked e unchecked
            const auto num = static_cast<int32_t>(duration.TotalMilliseconds() * static_cast<double>(SampleRate) / 1000.0);
            return (num + (num % static_cast<int32_t>(Channels)) * static_cast<int32_t>(BlockAlign));
        }

	public:
        std::vector<uint8_t> RawBuffer;
        int16_t FormatTag;
        int16_t Channels;
        int32_t SampleRate;
        int32_t AvgBytesPerSec;
        int16_t BlockAlign;
        int16_t BitsPerSample;
	};
}

#endif