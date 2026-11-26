#include "Xna/ContentPipeline/Audio/AudioHelper.hpp"
#include "Xna/ContentPipeline/Audio/WaveFormatEx.hpp"
#include "Xna/ContentPipeline/Audio/AudioContent.hpp"
#include <optional>
#include <Xna/Framework/Audio/WavFile.hpp>
#include <Xna/CSharp/IO.hpp>

namespace Xna {
#pragma pack(push, 1)
	struct RiffHeader {
		char id[4] = { 'R','I','F','F' };       // "RIFF"
		int32_t size = 0;
		char format[4] = { 'W','A','V','E' };   // "WAVE"
	};

	struct ChunkHeader {
		char id[4] = { 'f','m','t',' ' };
		int32_t size = 16;
	};

	struct WaveFormatPCM {
		int16_t audioFormat = 1; // PCM
		int16_t numChannels = 0;
		int32_t sampleRate = 0;
		int32_t byteRate = 0;
		int16_t blockAlign = 0;
		int16_t bitsPerSample = 16;
	};

	struct WavHeader {
		RiffHeader riff{};
		ChunkHeader chunk{};
		WaveFormatPCM pcm{};
		char data[4] = { 'd','a','t','a' };
		int32_t dataSize = 0;
	};	
#pragma pack(pop)		

	struct AudioHelper::Implementation {
		/*std::unique_ptr<AVFormatContext, AVFormatContextDeleter> context{ nullptr };
		std::filesystem::path targetFilename;*/
	};

	AudioHelper::AudioHelper() { 
		impl = std::make_shared<Implementation>(); 
	}

	bool AudioHelper::OpenFile(std::filesystem::path const& filename) {
		//impl->context = nullptr;		
		//AVFormatContext* raw_ctx = nullptr;

		//if (avformat_open_input(&raw_ctx, filename.string().c_str(), nullptr, nullptr) != 0) {
		//	return false; // Erro ao abrir
		//}

		//impl->context.reset(raw_ctx);	
		//impl->targetFilename = filename;

		return true;
	}

	bool AudioHelper::IsSupportedAudioFile(AudioFileType& audioFileType) {
		//if (impl->context == nullptr)
		//	return false;
		//
		//// 2. Lê os fluxos de dados para pegar as informações
		//if (avformat_find_stream_info(impl->context.get(), nullptr) >= 0) {
		//	// Encontra o primeiro stream de áudio
		//	auto streamIndex = av_find_best_stream(impl->context.get(), AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);

		//	std::optional<AudioFileType> currentFileType = std::nullopt;
		//	if (streamIndex >= 0) {
		//		
		//		auto codecParams = impl->context->streams[streamIndex]->codecpar;

		//		switch (codecParams->codec_id) {
		//		case AV_CODEC_ID_WMAV2:
		//			currentFileType = AudioFileType::Wma;
		//			break;
		//		case AV_CODEC_ID_MP3:
		//			currentFileType = AudioFileType::Mp3;
		//			break;
		//		case AV_CODEC_ID_PCM_S16LE:
		//			currentFileType = AudioFileType::Wav;
		//			break;
		//		case AV_CODEC_ID_VORBIS:
		//			currentFileType = AudioFileType::Ogg;
		//			break;
		//		default: 
		//			return false;
		//		}

		//		if (currentFileType) {
		//			audioFileType = *currentFileType;
		//			return true;
		//		}
		//	}
		//}

		return false;
	}

	int64_t AudioHelper::GetDuration() {
		//if (impl->context == nullptr)
		//	return -1;

		//int64_t duration{ -1 };
		//// Duração: O FFmpeg retorna em AV_TIME_BASE (microssegundos)
		//if (impl->context->duration != AV_NOPTS_VALUE) {
		//	duration = ((impl->context->duration * 1000) / AV_TIME_BASE);
		//}

		//return duration;

		return 0;
	}		

	bool AudioHelper::Convert(ConversionFormat conversionFormat, ConversionQuality quality, std::optional<std::filesystem::path> const& targetFileName, AudioContent& audioContent) {
		//AudioFileType audioFileType;
		//if (!IsSupportedAudioFile(audioFileType))
		//	return false;

		//switch (conversionFormat)
		//{
		//case Xna::ConversionFormat::Pcm:
		//	if (audioFileType == AudioFileType::Wma) {
		//		auto vec = ConvertWmaToWavInMemory();
		//		auto stream = std::make_shared<MemoryStream>(vec);
		//		
		//		//Testamos se a conversão foi bem realizada
		//		auto wavFile = WavFile::Open(stream);

		//		audioContent.Format = InternalAudioFormat(*wavFile.Format());
		//		audioContent.Data = *wavFile.Data();
		//		audioContent.Duration = wavFile.Duration();
		//		audioContent.LoopStart = wavFile.LoopStart();
		//		audioContent.LoopLength = wavFile.LoopLength();
		//		audioContent.FileType = AudioFileType::Wav;
		//	}
		//	break;
		//case Xna::ConversionFormat::Adpcm:
		//	throw std::runtime_error("File conversion to Adpcm is not implemented.");
		//	break;
		//case Xna::ConversionFormat::WindowsMedia:
		//	throw std::runtime_error("File conversion to Wma is not implemented.");
		//	break;
		//case Xna::ConversionFormat::Xma:
		//	throw std::runtime_error("File conversion to Xma is not implemented.");
		//	break;
		//default:
		//	break;
		//}

		return true;
	}
}