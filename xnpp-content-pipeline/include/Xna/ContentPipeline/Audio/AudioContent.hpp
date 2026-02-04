#ifndef XNA_AUDIO_AUDIOCONTENT_HPP
#define XNA_AUDIO_AUDIOCONTENT_HPP

#include "Xna/ContentPipeline/ContentItem.hpp"
#include <Xna/CSharp/TimeSpan.hpp>
#include <optional>
#include <vector>
#include <cstdint>
#include <Xna/Framework/Audio/AudioFormat.hpp>
#include "Xna/ContentPipeline/Audio/AudioFormat.hpp"
#include "Shared.hpp"

namespace Xna {
	struct AudioContent {
		std::optional<InternalAudioFormat> Format;
		CSharp::TimeSpan Duration{0};
		int32_t LoopStart{0};
		int32_t LoopLength{0};
		std::vector<uint8_t> Data;
		std::optional<AudioFileType> FileType;

		Xna::ContentItem ContentItem{};
	};
}

#endif