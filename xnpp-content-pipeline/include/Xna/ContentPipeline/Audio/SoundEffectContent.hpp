#ifndef XNA_AUDIO_SOUNDEFFECTCONTENT_HPP
#define XNA_AUDIO_SOUNDEFFECTCONTENT_HPP

#include <vector>
#include <cstdint>

namespace Xna {
	struct SoundEffectContent {
        std::vector<uint8_t> Format;
        std::vector<uint8_t> Data;
        int32_t LoopStart;
        int32_t LoopLength;
        int32_t Duration;

        SoundEffectContent(
            std::vector<uint8_t> const& format,
            std::vector<uint8_t> const& data,
            int32_t loopStart,
            int32_t loopLength,
            int32_t duration)
            : Format(format), Data(data), 
            LoopStart(loopStart), LoopLength(loopLength), Duration(duration) {
        }
	};
}

#endif