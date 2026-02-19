#include "Xna/Platform/_Platform.hpp"
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>
#include "Xna/Framework/Audio/SoundEffect.hpp"
#include "InternalSdl.hpp"

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

	void Sdl::System::InitAudio() {
		AudioEngine.Initialize();
	}

	void Sdl::System::DisposeAudio() {
		AudioEngine.Shutdown();
	}

	namespace PlatformNS {
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

		struct SdlSoundEffect : public ISoundEffect {
			std::vector<uint8_t> pcmData{};
			ma_audio_buffer audioBuffer{};
			ma_uint64 loopStartFrames{};
			ma_uint64 loopEndFrames{};
			WAVEFORMATEX waveFormatex{};

			~SdlSoundEffect() override {
				ma_audio_buffer_uninit(&audioBuffer);
			}

			void Load(std::vector<uint8_t> const& format, std::vector<uint8_t> const& data, size_t offset, size_t count, size_t loopStart, size_t loopLength) override {
				auto wfx = reinterpret_cast<const WAVEFORMATEX*>(format.data());				
				waveFormatex = *wfx;

				// Copiar PCM (precisa persistir)
				pcmData.resize(count);
				std::memcpy(
					pcmData.data(),
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
						pcmData.data(),
						nullptr
					);

				//Criar buffer
				ma_result result =
					ma_audio_buffer_init(
						&bufferConfig,
						&audioBuffer);

				if (result != MA_SUCCESS)
					throw std::runtime_error("Platform::SoundEffect_Create: Failed to create ma_audio_buffer");

				loopStartFrames = static_cast<ma_uint64>(loopStart);
				loopEndFrames = static_cast<ma_uint64>(loopStart + loopLength);
			}
		};		

		struct SdlSoundEffectInstance : public ISoundEffectInstance {
			ma_sound sound{};

			~SdlSoundEffectInstance() override {
				ma_sound_uninit(&sound);
			}
		};

		std::unique_ptr<ISoundEffect> ISoundEffect::Create() {
			return std::make_unique<SdlSoundEffect>();
		}

		std::unique_ptr<ISoundEffectInstance> ISoundEffectInstance::Create() {
			return std::make_unique<SdlSoundEffectInstance>();
		}
	}
}