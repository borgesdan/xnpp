#include "Xna/Framework/Audio/WavFile.hpp"
#include "Xna/CSharp/Exception.hpp"
#include "Xna/CSharp/IO/MemoryStream.hpp"
#include "Xna/Framework/Audio/AudioHelper.hpp"

namespace Xna {
    WavFile::WavFile(std::shared_ptr<CSharp::Stream> const &source) {
        if (source->Length() == 0)
            throw CSharp::ArgumentNullException("WavFile::WavFile");

        reader = CSharp::BinaryReader(source);
        ParseWavHeader();

        const auto length = reader->BaseStream()->Length();
        for (size_t position = reader->BaseStream()->Position(); position < length; ) {
            try {
                ReadChunk();
            } catch (std::exception& e){
                break;
            }

            position = reader->BaseStream()->Position();
        }

        if (format == std::nullopt || buffer == std::nullopt)
            throw CSharp::ArgumentException("InvalidWaveStream");

        ParseLoopRegion();
        reader->Close();
    }

    void WavFile::ParseWavHeader() {
        const auto num1 = AudioHelper::Byteswap(reader->ReadInt32());
        const auto num2 = reader->ReadInt32();
        const auto num3 = AudioHelper::Byteswap(reader->ReadInt32());
        const auto strLen = reader->BaseStream()->Length() - 8L;
        if (num1 != WavFile::riffId || strLen != num2 || num3 != WavFile::wavId)
            throw CSharp::InvalidOperationException("WavFile::ParseWavHeader");
    }

    void WavFile::ReadChunk() {
        if (reader->BaseStream()->Position() % 2L > 0L)
            throw CSharp::InvalidOperationException();

        const auto id = AudioHelper::Byteswap(reader->ReadInt32());

        if (id == WavFile::fmtId)
            ParseFormat();

        else if (id == WavFile::dataId)
            ParseData();

        else if (id == WavFile::smplId)
            smplChunk = WavFile::RiffChunk(id, reader->ReadBytes(reader->ReadInt32()));

        else if (id == WavFile::wsmpId)
            wsmpChunk = WavFile::RiffChunk(id, reader->ReadBytes(reader->ReadInt32()));
        else {
            const auto value = reader->ReadInt32();
            reader->ReadBytes(value);
        }            

        if (reader->BaseStream()->Position() % 2L <= 0L)
            return;

        const auto num = reader->ReadByte();
    }

    void WavFile::ParseFormat() {
        const auto count = reader->ReadInt32();

        if (count >= 16)
            format = AudioFormat(reader->ReadBytes(count));
        else
            throw CSharp::InvalidOperationException("WavFile::ParseFormat");

        if (format->FormatTag != 1 
            || (format->Channels != 1 && format->Channels != 2)
            || format->SampleRate < 8000 
            || format->SampleRate > 48000 
            || (format->BitsPerSample != 8 && format->BitsPerSample != 16) 
            || format->BlockAlign != format->Channels * format->BitsPerSample / 8)
        {
            format = std::nullopt;
            throw CSharp::InvalidOperationException();
        }
    }

    void WavFile::ParseData() {
        if (format == std::nullopt)
            throw CSharp::InvalidOperationException();

        const auto num = reader->ReadInt32();

        if (num < format->BlockAlign)
            throw CSharp::InvalidOperationException();

        buffer = reader->ReadBytes(num - num % format->BlockAlign);
        if (buffer->size() + reader->ReadBytes(num % format->BlockAlign).size() < num)
        {
            buffer = std::nullopt;
            throw CSharp::InvalidOperationException();
        }
        if (std::endian::native == std::endian::little || format->BitsPerSample != 16)
            return;

        auto numPtr = buffer->data();
        auto length = buffer->size();

        for (size_t index = 0; index < length / 2; ++index)
        {
            reinterpret_cast<int16_t *>(numPtr)[index] = AudioHelper::Byteswap(reinterpret_cast<int16_t *>(numPtr)[index]);
        }
    }

    void WavFile::ParseLoopRegion() {
        loop.Start = 0;
        loop.Length = SampleLength();
        if (wsmpChunk != std::nullopt) {
            ParseWsmpChunk();
        }
        else {
            if (smplChunk == std::nullopt)
                return;
            ParseSmplChunk();
        }
    }

    void WavFile::ParseWsmpChunk() {
        const auto& data = wsmpChunk->data; //Deixamos wsmpChunk const
        auto stream = std::make_shared<CSharp::MemoryStream>(data); //copia buffer data
        auto binaryReader = CSharp::BinaryReader(std::reinterpret_pointer_cast<CSharp::Stream>(stream));

        binaryReader.ReadBytes(16);
        const auto num1 = binaryReader.ReadInt32();

        for (size_t index = 0; index < num1; ++index)
        {
            binaryReader.ReadInt32();
            switch (binaryReader.ReadInt32())
            {
                case 0:
                case 1:
                    const auto num2 = binaryReader.ReadInt32();
                    const auto num3 = binaryReader.ReadInt32();
                    if (num2 >= 0 && num2 + num3 <= SampleLength())
                    {
                        loop.Start = num2;
                        loop.Length = num3;
                        goto label_6;
                    }
                    else
                        break;
            }
        }
        label_6:
              binaryReader.Close();
    }

    void WavFile::ParseSmplChunk() {
        const auto& data = smplChunk->data; //Deixamos smplChunk const
        auto stream = std::make_shared<CSharp::MemoryStream>(data); //copia o buffer
        auto binaryReader = CSharp::BinaryReader(std::reinterpret_pointer_cast<CSharp::Stream>(stream));

        binaryReader.ReadBytes(28);
        const auto num1 = binaryReader.ReadInt32();
        binaryReader.ReadInt32();

        for (size_t index = 0; index < num1; ++index) {
            binaryReader.ReadInt32();
            if (binaryReader.ReadInt32() == 0) {
                const auto num2 = binaryReader.ReadInt32();
                const auto num3 = binaryReader.ReadInt32();
                if (num2 < 0 || num2 >= SampleLength() || num2 >= num3 || num3 > SampleLength())
                    break;

                loop.Start = num2;
                loop.Length = num3 - num2 + 1;
                break;
            }

            binaryReader.ReadBytes(16);
        }
    }
}
