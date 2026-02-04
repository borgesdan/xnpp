#include "Xna/Framework/Audio/AudioHelper.hpp"
#include "Xna/CSharp/IO/MemoryStream.hpp"
#include "Xna/CSharp/IO/BinaryWriter.hpp"
#include "Xna/CSharp/IO/BinaryReader.hpp"
#include "Xna/Framework/Audio/AudioFormat.hpp"
#include <memory>
namespace Xna {
    std::vector<uint8_t> AudioHelper::MakeFormat(int32_t sampleRate, AudioChannels channels, int16_t bitDepth) {
        auto buffer = std::vector<uint8_t>(18);
        auto stream = std::make_shared<CSharp::MemoryStream>(buffer); //Move o buffer
        auto output = std::dynamic_pointer_cast<CSharp::Stream>(stream);
        auto binaryWriter = CSharp::BinaryWriter(output);

        binaryWriter.Write(static_cast<int16_t>(1));
        binaryWriter.Write(static_cast<int16_t>(channels));
        binaryWriter.Write(sampleRate);
        binaryWriter.Write(sampleRate * static_cast<int>(bitDepth) / 8 * static_cast<int>(channels));
        binaryWriter.Write(static_cast<int16_t>(static_cast<int>(channels) * static_cast<int>(bitDepth) / 8));
        binaryWriter.Write(bitDepth);
        binaryWriter.Write(static_cast<int16_t>(0));
        binaryWriter.Close();

        buffer = stream->CloseAndMove(); //Retorna o buffer
        return buffer;
    }

    std::vector<uint8_t> AudioHelper::LocalizeFormat(std::vector<uint8_t> const &source) {        
        auto sourceStream = std::make_shared<CSharp::MemoryStream>(source);
        auto sourceOutput = std::dynamic_pointer_cast<CSharp::Stream>(sourceStream);
        auto binaryReader = CSharp::BinaryReader(sourceOutput);

        const auto bufferSize = source.size() < 18 ? 18 : source.size();
        auto buffer = std::vector<uint8_t>(bufferSize);
        auto bufferStream = std::make_shared<CSharp::MemoryStream>(buffer); //Move o buffer
        auto bufferOutput = std::dynamic_pointer_cast<CSharp::Stream>(bufferStream);

        auto binaryWriter = CSharp::BinaryWriter(bufferOutput, true);

        binaryWriter.Write(AudioHelper::Byteswap(binaryReader.ReadInt16()));
        binaryWriter.Write(AudioHelper::Byteswap(binaryReader.ReadInt16()));
        binaryWriter.Write(AudioHelper::Byteswap(binaryReader.ReadInt32()));
        binaryWriter.Write(AudioHelper::Byteswap(binaryReader.ReadInt32()));
        binaryWriter.Write(AudioHelper::Byteswap(binaryReader.ReadInt16()));
        binaryWriter.Write(AudioHelper::Byteswap(binaryReader.ReadInt16()));
        
        buffer = bufferStream->CloseAndMove(); //Retorna o buffer

        binaryReader.Close();
        binaryWriter.Close();

        return buffer;
    }

    CSharp::TimeSpan AudioHelper::GetSampleDuration(int32_t sizeInBytes, int32_t sampleRate, AudioChannels channels) {
        return AudioFormat::Create(sampleRate, channels, 16).DurationFromSize(sizeInBytes);
    }

    int32_t AudioHelper::GetSampleSizeInBytes(CSharp::TimeSpan const &duration, int32_t sampleRate, AudioChannels channels) {
        return AudioFormat::Create(sampleRate, channels, 16).SizeFromDuration(duration);
    }
}
