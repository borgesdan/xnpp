#include "Xna/ContentPipeline/Processors/SoundEffectProcessor.hpp"
#include "Xna/ContentPipeline/Audio/Shared.hpp"
#include "Xna/ContentPipeline/Audio/AudioHelper.hpp"

namespace Xna {
    SoundEffectContent SoundEffectProcessor::ProcessT(AudioContent& input, ContentProcessorContext& context) {
        auto formatType = ConversionFormat::Pcm;

        if (context.TargetPlatform() == TargetPlatform::Windows || context.TargetPlatform() == TargetPlatform::WindowsPhone)
            formatType = Quality != ConversionQuality::Best ? ConversionFormat::Adpcm : ConversionFormat::Pcm;
        else if (context.TargetPlatform() == TargetPlatform::Xbox360)
            formatType = ConversionFormat::Xma;

        //TODO: [!] implementar conversão
        //input.ConvertFormat(formatType, this.quality, (string)null);        

        if (input.FileType != AudioFileType::Wav) {
            AudioHelper helper{};
            helper.OpenFile(input.ContentItem.Identity.SourceFileName);
            helper.Convert(formatType, ConversionQuality::Best, std::nullopt, input);
        }       

        return SoundEffectContent(
            input.Format->NativeWaveFormat(),
            input.Data,
            input.LoopStart,
            input.LoopLength,
            static_cast<int32_t>(input.Duration.TotalMilliseconds()));
    }
}