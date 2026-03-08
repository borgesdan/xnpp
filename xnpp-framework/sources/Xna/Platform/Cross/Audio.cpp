/*
* A biblioteca miniaudio.h é a responsável pela reprodução de músicas e sons no jogo.
* Suporta nativamente MP3, WAV, FLAC, para OGG é necessário a integração de biblioteca externa (libvorbis, libogg).
* A biblioteca não suporta WMA.
* Por padrão temos somente uma instância da engine de áudio que persistirá até o encerramento do programa.
*/

#include "Xna/Platform/Platform.hpp"
#include "Xna/Framework/Audio/SoundEffect.hpp"
#include "Internal.hpp"
#include <functional>
#define MINIAUDIO_IMPLEMENTATION
#include "third-party/miniaudio/miniaudio.h"
#include "third-party/miniaudio/miniaudio_libvorbis.h"

namespace Xna {
	//Gerenciador de áudio é sons
	class AudioEngineManager
	{
	public:
		void Initialize()
		{
			// Configurar os backends personalizados
			pCustomBackendVTables[0] = ma_decoding_backend_libvorbis;

			// Configurar o resource manager
			ma_resource_manager_config resourceManagerConfig = ma_resource_manager_config_init();
			resourceManagerConfig.ppCustomDecodingBackendVTables = pCustomBackendVTables;
			resourceManagerConfig.customDecodingBackendCount = 1;  // Número de backends

			// Inicializar resource manager
			if (ma_resource_manager_init(&resourceManagerConfig, &resourceManager) != MA_SUCCESS) {
				throw std::runtime_error("");
			}

			// Configurar o engine com o resource manager
			ma_engine_config engineConfig = ma_engine_config_init();
			engineConfig.pResourceManager = &resourceManager;

			// Inicializar engine
			if (ma_engine_init(&engineConfig, &engine) != MA_SUCCESS) {
				ma_resource_manager_uninit(&resourceManager);
				throw std::runtime_error("");
			}
		}

		void Shutdown()
		{
			ma_engine_uninit(&engine);
			ma_resource_manager_uninit(&resourceManager);
		}

		ma_engine& GetNative() { return engine; }

	private:
		ma_resource_manager resourceManager{};
		ma_decoding_backend_vtable* pCustomBackendVTables[1];
		ma_engine engine{};
	};

	//Somente uma instância para todo o programa
	static AudioEngineManager AudioEngine = {};	

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

		//Representação interna do SoundEffect com miniaudio
		struct MiniAudioSoundEffect final : public ISoundEffect {
			std::vector<uint8_t> pcmData{};
			ma_audio_buffer audioBuffer{};
			ma_audio_buffer_config bufferConfig{};
			ma_uint64 loopStartFrames{};
			ma_uint64 loopEndFrames{};
			WAVEFORMATEX waveFormatex{};

			static inline float MasterVolume = 1;

			~MiniAudioSoundEffect() override {
				ma_audio_buffer_uninit(&audioBuffer);
			}

			//Carrega o arquivo WAV já decodificado pela engine e transformado em array de bytes.
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

				bufferConfig =
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

		//Representação interna do SoundEffectInstance
		struct MiniAudioSoundEffectInstance final : public ISoundEffectInstance {
			ma_sound sound{};
			ma_audio_buffer buffer{};
			ma_audio_buffer_config bufferConfig{};
			MediaState state{ MediaState::Stoped };

			void Load(ISoundEffect* baseSE) override {
				auto se = reinterpret_cast<MiniAudioSoundEffect*>(baseSE);

				bufferConfig = ma_audio_buffer_config_init(
					se->bufferConfig.format,
					se->bufferConfig.channels,
					se->loopEndFrames - se->loopStartFrames,
					se->pcmData.data(),
					nullptr
				);

				ma_audio_buffer_init(&bufferConfig, &buffer);

				ma_sound_config config = ma_sound_config_init();
				config.pDataSource = &buffer;

				ma_sound_init_ex(&AudioEngine.GetNative(), &config, &sound);

				ma_data_source_set_loop_point_in_pcm_frames(
					&buffer,
					se->loopStartFrames,
					se->loopEndFrames
				);

				SetVolume(MiniAudioSoundEffect::MasterVolume);
			}

			void SetVolume(float value) override {
				ma_sound_set_volume(&sound, value);
			}

			void SetPan(float value) override {
				ma_sound_set_pan(&sound, value);				
			}

			void SetPitch(float value) override {
				ma_sound_set_pitch(&sound, value);				
			}

			void IsLooped(bool value) override {
				ma_sound_set_looping(&sound, static_cast<ma_bool32>(value));
			}

			void Play() override {				
				assert(sound.pDataSource != nullptr);

				ma_sound_start(&sound);
				
				state = MediaState::Playing;
			}

			void Pause() override {
				ma_sound_stop(&sound);
				state = MediaState::Paused;
			}
			
			void Stop() override {
				ma_sound_stop(&sound);
				ma_sound_seek_to_pcm_frame(&sound, 0);
				state = MediaState::Stoped;
			}

			MediaState GetState() override {
				auto isPlaying = ma_sound_is_playing(&sound);				

				if (isPlaying)
					return MediaState::Playing;
				else if (state == MediaState::Paused)
					return MediaState::Paused;

				state = MediaState::Stoped;
				return state;
			}			

			~MiniAudioSoundEffectInstance() override {
				ma_sound_uninit(&sound);
			}
		};

		std::unique_ptr<ISoundEffect> ISoundEffect::Create() {
			return std::make_unique<MiniAudioSoundEffect>();
		}

		std::unique_ptr<ISoundEffectInstance> ISoundEffectInstance::Create() {
			return std::make_unique<MiniAudioSoundEffectInstance>();
		}

		void MasterAudio::SetMasterVolume(float value) {
			MiniAudioSoundEffect::MasterVolume = value;
		}

		//Representação interna do MediaPlayer
		struct MiniAudioMediaPlayer final : public IMediaPlayer {
			std::filesystem::path currentFile;
			float currentVolume = 1.0f;
			bool isMuted = false;
			ma_sound music{};			

			static inline std::unique_ptr<MiniAudioMediaPlayer> MediaPlayer = nullptr;

			std::function<void()> songChanged;
			std::function<void()> mediaStateChanged;			

			void Play(std::filesystem::path const& song) override {
				const auto file = song.string();

				ma_result result = ma_sound_init_from_file(
					&AudioEngine.GetNative(),
					file.c_str(),
					MA_SOUND_FLAG_STREAM,
					nullptr,
					nullptr,
					&music);

				if (result != MA_SUCCESS) {
					std::string error = "MiniAudioMediaPlayer tried to open the file but failed: ";
					error.append(song.string());

					throw std::runtime_error(error);
				}					

				ma_sound_start(&music);

				if(songChanged)
					songChanged();
			}

			void Pause() override {
				ma_sound_stop(&music);
				
				if(mediaStateChanged)
					mediaStateChanged();
			}

			void Resume() override {
				ma_sound_start(&music);

				if (mediaStateChanged)
					mediaStateChanged();
			}

			void Stop() override {
				ma_sound_stop(&music);
				ma_sound_seek_to_pcm_frame(&music, 0);

				if (mediaStateChanged)
					mediaStateChanged();
			}

			void SetVolume(float volume) override {
				currentVolume = volume;

				if(!isMuted)
					ma_sound_set_volume(&music, volume);
			}

			void SetMuted(bool value) override {
				currentVolume = ma_sound_get_volume(&music);
				
				if (value) 
					ma_sound_set_volume(&music, 0);
				else 
					ma_sound_set_volume(&music, currentVolume);

				isMuted = value;
			}

			void SetIsRepeating(bool value) override {
				ma_sound_set_looping(&music, static_cast<ma_bool32>(value));
			}

			//Retorna o tempo em milisegundos
			double GetPlayPosition() override {
				float cursor = 0.0f;
				const auto result = ma_sound_get_cursor_in_seconds(&music, &cursor);

				if(result != MA_SUCCESS)
					throw std::runtime_error("It was not possible to obtain the time of the song.");

				//Tempo em milisegundos
				return cursor * 1000.0;
			}

			void SongChanged(std::function<void()> const& func) override 
			{
				songChanged = func;
			}

			void MediaStateChanged(std::function<void()> const& func) override
			{
				mediaStateChanged = func;
			}

			~MiniAudioMediaPlayer() override {
				ma_sound_uninit(&music);
			}
		};

		//Temos somente uma instância do Media Player para todo o tempo de vida do programa.
		IMediaPlayer& IMediaPlayer::GetInstance() {

			if(!MiniAudioMediaPlayer::MediaPlayer)
				MiniAudioMediaPlayer::MediaPlayer = std::make_unique<MiniAudioMediaPlayer>();
			return *MiniAudioMediaPlayer::MediaPlayer.get();
		}
	}	

	void Cross::System::InitAudio() {
		AudioEngine.Initialize();
	}

	void Cross::System::DisposeAudio() {
		if (PlatformNS::MiniAudioMediaPlayer::MediaPlayer)
			PlatformNS::MiniAudioMediaPlayer::MediaPlayer = nullptr;

		AudioEngine.Shutdown();
	}
}