#include "Xna/Platform/_Platform.hpp"
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

namespace Xna {
    class AudioEngineManager
    {
    public:
        void Initialize()
        {
            ma_result result = ma_engine_init(nullptr, &_engine);
            if (result != MA_SUCCESS)
                throw std::runtime_error("Falha ao inicializar ma_engine");
        }

        void Shutdown()
        {
            ma_engine_uninit(&_engine);
        }

        ma_engine& GetNative() { return _engine; }

    private:
        ma_engine _engine{};
    };

    static AudioEngineManager AudioEngine = {};

#pragma pack(push, 1)
    struct WAVEFORMATEX
    {
        uint16_t wFormatTag;
        uint16_t nChannels;
        uint32_t nSamplesPerSec;
        uint32_t nAvgBytesPerSec;
        uint16_t nBlockAlign;
        uint16_t wBitsPerSample;
        uint16_t cbSize;
    };
#pragma pack(pop)

    namespace PlatformNS {
        struct SdlPlatformSoundEffect : public ISoundEffect {
            ~SdlPlatformSoundEffect() override = default;

        private:
            ma_sound sound{};
            
            WAVEFORMATEX currentWaveFormatex{};
            std::vector<uint8_t> pcmData;
            ma_audio_buffer audioBuffer{};
            ma_uint64 loopStartFrames{};
            ma_uint64 loopEndFrames{};
        };

        std::unique_ptr<ISoundEffect> ISoundEffect::Create() {
            return std::make_unique<SdlPlatformSoundEffect>();
        }
    }

    /*void Platform::SoundEffect_Create(SoundEffect const&, std::vector<uint8_t> const& format, std::vector<uint8_t> const& data, size_t offset, size_t count, size_t loopStart, size_t loopLength) {

    }*/
}