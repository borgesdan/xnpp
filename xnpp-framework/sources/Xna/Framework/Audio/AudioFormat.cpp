#include "Xna/Framework/Audio/AudioFormat.hpp"
#include <Xna/CSharp/IO.hpp>
#include "AudioHelper.hpp"

namespace Xna {
    AudioFormat AudioFormat::Create(int32_t sampleRate, AudioChannels channels, int16_t bitDepth) {
        return AudioFormat(AudioHelper::MakeFormat(sampleRate, channels, bitDepth));
    }

	AudioFormat::AudioFormat(std::vector<uint8_t> const& buffer) {
        auto _buffer = std::vector<uint8_t>(buffer);
        //Aqui vamos mover o buffer!
        //TODO:avaliar código
        auto stream = std::unique_ptr<Stream>(new MemoryStream(_buffer)); 

        auto binaryReader = BinaryReader(std::move(stream));
        FormatTag = binaryReader.ReadInt16();
        Channels = binaryReader.ReadInt16();
        SampleRate = binaryReader.ReadInt32();
        AvgBytesPerSec = binaryReader.ReadInt32();
        BlockAlign = binaryReader.ReadInt16();
        BitsPerSample = binaryReader.ReadInt16();
        binaryReader.Close();
        
        _buffer = dynamic_cast<MemoryStream*>(stream.get())->CloseAndMove(); //Retorna o buffer

        RawBuffer = AudioHelper::LocalizeFormat(_buffer);
	}
}