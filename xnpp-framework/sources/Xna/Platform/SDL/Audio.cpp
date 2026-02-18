#include "Xna/Platform/_Platform.hpp"
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>
#include "Xna/Framework/Audio/SoundEffect.hpp"

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
    //The WAVEFORMATEX structure as implemented in the mmeapi.h header.
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
            ma_sound sound{};            
            std::vector<uint8_t> pcmData{};
            ma_audio_buffer audioBuffer{};
            ma_uint64 loopStartFrames{};
            ma_uint64 loopEndFrames{};
            WAVEFORMATEX currentWaveFormatex{};

            ~SdlPlatformSoundEffect() override {
                ma_sound_uninit(&sound);
                ma_audio_buffer_uninit(&audioBuffer);
            }
        };

        std::unique_ptr<ISoundEffect> ISoundEffect::Create() {
            return std::make_unique<SdlPlatformSoundEffect>();
        }
    }

    void Platform::SoundEffect_Create(SoundEffect const& se, std::vector<uint8_t> const& format, std::vector<uint8_t> const& data, size_t offset, size_t count, size_t loopStart, size_t loopLength) {
        auto wfx = reinterpret_cast<const WAVEFORMATEX*>(format.data());

        // Persistir WAVEFORMATEX
        auto sdlSoundEffect = reinterpret_cast<PlatformNS::SdlPlatformSoundEffect*>(se.impl->platform.get());
        sdlSoundEffect->currentWaveFormatex = *wfx;

        // Copiar PCM (precisa persistir)
        sdlSoundEffect->pcmData.resize(count);
        std::memcpy(
            sdlSoundEffect->pcmData.data(),
            data.data() + offset,
            count);        

        // Converter formato
        ma_format maFormat;

        switch (wfx->wBitsPerSample)
        {
        case 8:  maFormat = ma_format_u8;  break;
        case 16: maFormat = ma_format_s16; break;
        case 24: maFormat = ma_format_s24; break;
        case 32:
            if (wfx->wFormatTag == WAVE_FORMAT_IEEE_FLOAT)
                maFormat = ma_format_f32;
            else
                maFormat = ma_format_s32;
            break;
        default:
            throw std::runtime_error("Platform::SoundEffect_Create: PCM format not supported.");
        }

        ma_uint64 totalFrames = count / wfx->nBlockAlign;
        
        ma_audio_buffer_config bufferConfig =
            ma_audio_buffer_config_init(
                maFormat,
                wfx->nChannels,
                totalFrames,
                sdlSoundEffect->pcmData.data(),
                nullptr
            );

        ma_result result =
            ma_audio_buffer_init(
                &bufferConfig,
                &sdlSoundEffect->audioBuffer);

        if (result != MA_SUCCESS)
            throw std::runtime_error("Platform::SoundEffect_Create: Failed to create ma_audio_buffer");
        
        sdlSoundEffect->loopStartFrames = static_cast<ma_uint64>(loopStart);
        sdlSoundEffect->loopEndFrames = static_cast<ma_uint64>(loopStart + loopLength);
    }
}