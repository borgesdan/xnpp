extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
#include <libavutil/channel_layout.h>
#include <libavutil/samplefmt.h>
}

#include "Xna/ContentPipeline/Audio/AudioHelper.hpp"
#include "Xna/ContentPipeline/Audio/WaveFormatEx.hpp"
#include "Xna/ContentPipeline/Audio/AudioContent.hpp"
#include <optional>
#include <Xna/Framework/Audio/WavFile.hpp>
#include <Xna/CSharp/IO/MemoryStream.hpp>

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

	struct AVFormatContextDeleter {
		void operator()(AVFormatContext* ctx) const {
			if (ctx) {
				avformat_close_input(&ctx);
			}				
		}
	};

	struct AudioHelper::Implementation {
		std::unique_ptr<AVFormatContext, AVFormatContextDeleter> context{ nullptr };
		std::filesystem::path targetFilename;
	};

	AudioHelper::AudioHelper() { 
		impl = std::make_shared<Implementation>(); 
	}

	bool AudioHelper::OpenFile(std::filesystem::path const& filename) {
		impl->context = nullptr;		
		AVFormatContext* raw_ctx = nullptr;

		if (avformat_open_input(&raw_ctx, filename.string().c_str(), nullptr, nullptr) != 0) {
			return false; // Erro ao abrir
		}

		impl->context.reset(raw_ctx);	
		impl->targetFilename = filename;

		return true;
	}

	bool AudioHelper::IsSupportedAudioFile(AudioFileType& audioFileType) {
		if (impl->context == nullptr)
			return false;
		
		// 2. Lê os fluxos de dados para pegar as informações
		if (avformat_find_stream_info(impl->context.get(), nullptr) >= 0) {
			// Encontra o primeiro stream de áudio
			auto streamIndex = av_find_best_stream(impl->context.get(), AVMEDIA_TYPE_AUDIO, -1, -1, nullptr, 0);

			std::optional<AudioFileType> currentFileType = std::nullopt;
			if (streamIndex >= 0) {
				
				auto codecParams = impl->context->streams[streamIndex]->codecpar;

				switch (codecParams->codec_id) {
				case AV_CODEC_ID_WMAV2:
					currentFileType = AudioFileType::Wma;
					break;
				case AV_CODEC_ID_MP3:
					currentFileType = AudioFileType::Mp3;
					break;
				case AV_CODEC_ID_PCM_S16LE:
					currentFileType = AudioFileType::Wav;
					break;
				default: 
					return false;
				}

				if (currentFileType) {
					audioFileType = *currentFileType;
					return true;
				}
			}
		}

		return false;
	}

	int64_t AudioHelper::GetDuration() {
		if (impl->context == nullptr)
			return -1;

		int64_t duration{ -1 };
		// Duração: O FFmpeg retorna em AV_TIME_BASE (microssegundos)
		if (impl->context->duration != AV_NOPTS_VALUE) {
			duration = ((impl->context->duration * 1000) / AV_TIME_BASE);
		}

		return duration;
	}		

	bool AudioHelper::Convert(ConversionFormat conversionFormat, ConversionQuality quality, std::optional<std::filesystem::path> const& targetFileName, AudioContent& audioContent) {
		AudioFileType audioFileType;
		if (!IsSupportedAudioFile(audioFileType))
			return false;

		switch (conversionFormat)
		{
		case Xna::ConversionFormat::Pcm:
			if (audioFileType == AudioFileType::Wma) {
				auto vec = ConvertWmaToWavInMemory();
				auto stream = std::make_shared<CSharp::MemoryStream>(vec);
				
				//Testamos se a conversão foi bem realizada
				auto wavFile = WavFile::Open(stream);

				audioContent.Format = InternalAudioFormat(*wavFile.Format());
				audioContent.Data = *wavFile.Data();
				audioContent.Duration = wavFile.Duration();
				audioContent.LoopStart = wavFile.LoopStart();
				audioContent.LoopLength = wavFile.LoopLength();
				audioContent.FileType = AudioFileType::Wav;
			}
			break;
		case Xna::ConversionFormat::Adpcm:
			throw std::runtime_error("File conversion to Adpcm is not implemented.");
			break;
		case Xna::ConversionFormat::WindowsMedia:
			throw std::runtime_error("File conversion to Wma is not implemented.");
			break;
		case Xna::ConversionFormat::Xma:
			throw std::runtime_error("File conversion to Xma is not implemented.");
			break;
		default:
			break;
		}

		return true;
	}

	//
	// Conversor
	//

	std::vector<uint8_t> AudioHelper::ConvertWmaToWavInMemory() {
		av_log_set_level(AV_LOG_ERROR);
		AVFormatContext* fmtCtx = nullptr;
		avformat_open_input(&fmtCtx, impl->targetFilename.string().c_str(), nullptr, nullptr);
		avformat_find_stream_info(fmtCtx, nullptr);

		int audioStream = -1;
		for (unsigned i = 0; i < fmtCtx->nb_streams; ++i) {
			if (fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
				audioStream = i;
				break;
			}
		}

		AVCodecParameters* codecpar = fmtCtx->streams[audioStream]->codecpar;
		const AVCodec* codec = avcodec_find_decoder(codecpar->codec_id);

		AVCodecContext* codecCtx = avcodec_alloc_context3(codec);
		avcodec_parameters_to_context(codecCtx, codecpar);
		avcodec_open2(codecCtx, codec, nullptr);

		auto channelLayout = codecCtx->ch_layout;

		SwrContext* swr = nullptr;
		//0 on success, a negative AVERROR code on error.
		auto result = swr_alloc_set_opts2(
			//SwrContext **ps: Pointer to an existing Swr context if available, or to NULL if not.
			//On success, *ps will be set to the allocated context.
			&swr,
			//AVChannelLayout *out_ch_layout: output channel layout (e.g. AV_CHANNEL_LAYOUT_*)
			&channelLayout,
			//enum AVSampleFormat out_sample_fmt: output sample format (AV_SAMPLE_FMT_*).
			AV_SAMPLE_FMT_S16,
			//int out_sample_rate: output sample rate (frequency in Hz)
			codecCtx->sample_rate,
			//const AVChannelLayout *in_ch_layout: input channel layout (e.g. AV_CHANNEL_LAYOUT_*)
			&channelLayout,
			//enum AVSampleFormat  in_sample_fmt: input sample format (AV_SAMPLE_FMT_*).
			codecCtx->sample_fmt,
			//int  in_sample_rate: input sample rate (frequency in Hz)
			codecCtx->sample_rate,
			//int log_offset: logging level offset
			0,
			//void *log_ctx: parent logging context, can be NULL
			nullptr);
		swr_init(swr);

		std::vector<uint8_t> wavData;
		wavData.resize(sizeof(WavHeader)); // reserva espaço para o header

		AVPacket* pkt = av_packet_alloc();
		AVFrame* frame = av_frame_alloc();

		while (av_read_frame(fmtCtx, pkt) >= 0) {
			if (pkt->stream_index == audioStream) {
				avcodec_send_packet(codecCtx, pkt);

				while (avcodec_receive_frame(codecCtx, frame) == 0) {
					int outSamples = swr_get_out_samples(swr, frame->nb_samples);

					uint8_t* outBuffer = nullptr;
					int outLineSize;

					av_samples_alloc(
						&outBuffer,
						&outLineSize,
						channelLayout.nb_channels,
						outSamples,
						AV_SAMPLE_FMT_S16,
						0
					);

					int converted = swr_convert(
						swr,
						&outBuffer,
						outSamples,
						(const uint8_t**)frame->data,
						frame->nb_samples
					);

					int bytesPerSample = av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
					int dataSize = converted * channelLayout.nb_channels * bytesPerSample;

					size_t offset = wavData.size();
					wavData.resize(offset + dataSize);
					memcpy(wavData.data() + offset, outBuffer, dataSize);

					av_free(outBuffer);
				}
			}
			av_packet_unref(pkt);
		}

		// Preencher o cabeçalho WAV
		WavHeader header;		
		header.pcm.numChannels = channelLayout.nb_channels;
		header.pcm.sampleRate = codecCtx->sample_rate;
		header.pcm.blockAlign = header.pcm.numChannels * (header.pcm.bitsPerSample / 8);
		header.pcm.byteRate = header.pcm.sampleRate * header.pcm.blockAlign;
		header.dataSize = int32_t(wavData.size() - sizeof(WavHeader));
		header.riff.size = header.dataSize + sizeof(WavHeader) - 8;

		memcpy(wavData.data(), &header, sizeof(WavHeader));

		// Cleanup
		av_frame_free(&frame);
		av_packet_free(&pkt);
		swr_free(&swr);
		avcodec_free_context(&codecCtx);
		avformat_close_input(&fmtCtx);

		return wavData;
	}
}