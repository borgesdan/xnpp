#ifndef XNA_FRAMEWORK_AUDIO_AUDIOHELPER_HPP
#define XNA_FRAMEWORK_AUDIO_AUDIOHELPER_HPP

#include <vector>
#include <bit>
#include "Shared.hpp"
#include "Xna/CSharp/TimeSpan.hpp"

namespace Xna {
	struct AudioHelper final {
	public:
		static constexpr int16_t Byteswap(int16_t value) {
			if constexpr (std::endian::native != std::endian::little) {
				return static_cast<int16_t>(((value & 65280) >> 8) | ((value & 255) << 8));
			}

			return value;
		}

		static constexpr int32_t Byteswap(int32_t value) {
			if constexpr (std::endian::native != std::endian::little) {
				return value = value >> 24 | value << 24 | (value & 65280) << 8 | (value & 16711680) >> 8;
			}

			return value;
		}
		static std::vector<uint8_t> MakeFormat(int32_t sampleRate, AudioChannels channels, int16_t bitDepth);
		static std::vector<uint8_t> LocalizeFormat(std::vector<uint8_t> const& source);

		static CSharp::TimeSpan GetSampleDuration(int32_t sizeInBytes, int32_t sampleRate, AudioChannels channels);
		static int32_t GetSampleSizeInBytes(CSharp::TimeSpan const& duration, int32_t sampleRate, AudioChannels channels);

	public:
		static constexpr int32_t waveFormatExSize = 18;

	private:
		AudioHelper(AudioHelper&) = default;
		AudioHelper(AudioHelper&&) = default;
	};
}

#endif