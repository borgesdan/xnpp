#include "Xna/ContentPipeline/Audio/AudioFormat.hpp"
#include "Xna/ContentPipeline/Audio/WaveFormatEx.hpp"

namespace Xna {
    InternalAudioFormat::InternalAudioFormat(std::vector<uint8_t> const & waveFormatEx) {
        // copia o buffer (imutável externamente)
        rawFormatBuffer.assign(
            waveFormatEx.begin(),
            waveFormatEx.end()
        );

        // interpreta como WaveFormatEx
        const auto* wf = reinterpret_cast<const WaveFormatEx*>(rawFormatBuffer.data());

        formatTag = wf->FormatTag;
        channels = wf->Channels;
        sampleRate = wf->SampleRate;
        avgBytesPerSec = wf->AvgBytesPerSec;
        blockAlign = wf->BlockAlign;
        bitsPerSample = wf->BitsPerSample;
	}

    InternalAudioFormat::InternalAudioFormat(WaveFormatEx const& wf) {
        formatTag = wf.FormatTag;
        channels = wf.Channels;
        sampleRate = wf.SampleRate;
        avgBytesPerSec = wf.AvgBytesPerSec;
        blockAlign = wf.BlockAlign;
        bitsPerSample = wf.BitsPerSample;
    }

    void InternalAudioFormat::Convert(AudioFormat const& af) {
        formatTag = af.FormatTag;
        channels = af.Channels;
        sampleRate = af.SampleRate;
        avgBytesPerSec = af.AvgBytesPerSec;
        blockAlign = af.BlockAlign;
        bitsPerSample = af.BitsPerSample;
        rawFormatBuffer = af.RawBuffer;
    }
}