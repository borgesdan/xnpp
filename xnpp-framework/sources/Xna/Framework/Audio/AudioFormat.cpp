#include "Xna/Framework/Audio/AudioFormat.hpp"
#include "Xna/CSharp/IO/BinaryReader.hpp"
#include "Xna/CSharp/IO/MemoryStream.hpp"
#include "Xna/Framework/Audio/AudioHelper.hpp"

namespace Xna {
    AudioFormat AudioFormat::Create(int32_t sampleRate, AudioChannels channels, int16_t bitDepth) {
        return AudioFormat(AudioHelper::MakeFormat(sampleRate, channels, bitDepth));
    }

	AudioFormat::AudioFormat(std::vector<uint8_t> const& buffer) {
        auto _buffer = std::vector<uint8_t>(buffer);
        //Aqui vamos mover o buffer!
        auto stream = std::make_shared<CSharp::MemoryStream>(_buffer); 

        auto binaryReader = CSharp::BinaryReader(reinterpret_pointer_cast<CSharp::Stream>(stream), true);
        FormatTag = binaryReader.ReadInt16();
        Channels = binaryReader.ReadInt16();
        SampleRate = binaryReader.ReadInt32();
        AvgBytesPerSec = binaryReader.ReadInt32();
        BlockAlign = binaryReader.ReadInt16();
        BitsPerSample = binaryReader.ReadInt16();
        binaryReader.Close();
        
        _buffer = stream->CloseAndMove(); //Retorna o buffer

        RawBuffer = AudioHelper::LocalizeFormat(_buffer);
	}
}