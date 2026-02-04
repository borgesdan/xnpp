#ifndef XNA_AUDIO_SHARED_HPP
#define XNA_AUDIO_SHARED_HPP

namespace Xna {
	enum class ConversionFormat
	{
		Pcm,
		Adpcm,
		WindowsMedia,
		Xma,
	};

	enum class AudioFileType
	{
		Wav,
		Mp3,
		Wma,
	};

	//Low(96 kbps) Medium(128 kbps) Best(192 kbps)
	enum class ConversionQuality
	{
		//Low(96 kbps)
		Low,
		//Medium(128 kbps)
		Medium,
		//Best(192 kbps)
		Best,
	};
}

#endif