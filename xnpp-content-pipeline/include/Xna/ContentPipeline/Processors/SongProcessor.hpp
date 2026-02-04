#ifndef XNA_PROCESSORS_SONGPROCESSOR_HPP
#define XNA_PROCESSORS_SONGPROCESSOR_HPP

#include "Xna/ContentPipeline/ContentProcessor.hpp"
#include "Xna/ContentPipeline/Audio/AudioContent.hpp"
#include "Xna/ContentPipeline/Audio/SongContent.hpp"
#include "Xna/ContentPipeline/Audio/Shared.hpp"

namespace Xna {
    struct SongProcessor : public ContentProcessor<AudioContent, SongContent> {
        SongContent ProcessT(AudioContent& input, ContentProcessorContext& context) override;

        ConversionQuality Quality{ ConversionQuality::Best };
    };
}

#endif