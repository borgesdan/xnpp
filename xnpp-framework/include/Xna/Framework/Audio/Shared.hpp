#ifndef XNA_FRAMEWORK_AUDIO_SHARED_HPP
#define XNA_FRAMEWORK_AUDIO_SHARED_HPP

namespace Xna {
    enum class AudioChannels
    {
        Mono = 1,
        Stereo = 2,
    };

    enum class VoiceState
    {
        Paused = 8,
        Stopped = 4,
        Stopping = 2,
        Playing = 1,
    };

    enum class SoundState
    {
        Playing,
        Paused,
        Stopped,
    };

    enum class MicrophoneState
    {
        Started,
        Stopped,
    };

    enum class MicrophoneCaptureState
    {
        MicStopped = 2,
        MicStarted = 1,
    };
}

#endif