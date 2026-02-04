#ifndef XNA_PROCESSORS_SOUNDEFFECTPROCESSOR_HPP
#define XNA_PROCESSORS_SOUNDEFFECTPROCESSOR_HPP

#include "Xna/ContentPipeline/ContentProcessor.hpp"
#include "Xna/ContentPipeline/Audio/AudioContent.hpp"
#include "Xna/ContentPipeline/Audio/SoundEffectContent.hpp"
#include "Xna/ContentPipeline/Audio/Shared.hpp"

namespace Xna {
	struct SoundEffectProcessor : public ContentProcessor<AudioContent, SoundEffectContent> {
        SoundEffectContent ProcessT(AudioContent& input, ContentProcessorContext& context) override;

        ConversionQuality Quality{ ConversionQuality::Best };
	};
}

#endif