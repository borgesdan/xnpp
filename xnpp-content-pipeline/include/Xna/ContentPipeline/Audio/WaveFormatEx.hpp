#ifndef XNA_AUDIO_WAVEFORMATEX_HPP
#define XNA_AUDIO_WAVEFORMATEX_HPP

#include <cstdint>

namespace Xna {
	//desliga o padding automático que o compilador insere em structs para alinhamento de memória,
	//os campos ficam exatamente na ordem e nos offsets esperados, sem bytes extras.
#pragma pack(push, 1)
	struct WaveFormatEx {
		uint16_t FormatTag{ 0 };
		uint16_t Channels{ 0 };
		uint32_t SampleRate{ 0 };
		uint32_t AvgBytesPerSec{ 0 };
		uint16_t BlockAlign{ 0 };
		uint16_t BitsPerSample{ 0 };
		uint16_t ExtraBytes{ 0 };
	};
#pragma pack(pop)
}

#endif