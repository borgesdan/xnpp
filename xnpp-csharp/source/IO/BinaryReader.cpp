#include "Xna/CSharp/IO/BinaryReader.hpp"

using Xna::CSharp::InvalidOperationException;

namespace Xna::CSharp {
    BinaryReader::BinaryReader(std::shared_ptr<Stream> const& input, bool leaveOpen)
    {
        impl = std::make_shared<Implementation>();
        impl->_stream = input;
        impl->_leaveOpen = leaveOpen;

        if (!input->CanRead())
            throw ArgumentException(SR::Argument_StreamNotReadable);
    }

    std::shared_ptr<Stream> BinaryReader::BaseStream() const { return impl->_stream; }

    void BinaryReader::Close() {
        if (impl->_disposed)
            return;

        if (!impl->_leaveOpen)
            impl->_stream->Close();

        impl->_disposed = true;
    }

    int32_t BinaryReader::PeekChar() {
        if (impl->_disposed)
            throw InvalidOperationException();

        if (!impl->_stream->CanSeek())
        {
            return -1;
        }

        const auto origPos = impl->_stream->Position();
        const auto ch = Read();

        impl->_stream->Position(origPos);

        return ch;
    }

    int32_t BinaryReader::Read(bool twoBytesPerChar) {
        if (impl->_disposed)
            throw InvalidOperationException();

        int32_t charsRead = 0;
        int32_t numBytes;
        int64_t posSav = 0;

        if (impl->_stream->CanSeek())
        {
            posSav = impl->_stream->Position();
        }

        char singleChar = '\0';

        while (charsRead == 0) {
            numBytes = twoBytesPerChar ? 2 : 1;

            auto r = impl->_stream->ReadByte();

            if (r == -1)
            {
                numBytes = 0;
            }
            if (numBytes == 2)
            {
                auto r2 = impl->_stream->ReadByte();
                r |= r2;

                if (r == -1)
                {
                    numBytes = 1;
                }
            }

            if (numBytes == 0)
            {
                return -1;
            }

            const char chars = r;
            const auto decoder = std::string(&chars);
            charsRead = decoder.size();
            singleChar = decoder[0];
        }

        return singleChar;
    }

    uint8_t BinaryReader::InternalReadByte() {
        if (impl->_disposed)
            throw InvalidOperationException();

        const auto b = impl->_stream->ReadByte();
        if (b == -1)
        {
            throw EndOfStreamException(SR::IO_EOF_ReadBeyondEOF);
        }

        return static_cast<uint8_t>(b);
    }

    char BinaryReader::ReadChar(bool twoBytes) {
        const auto value = Read(twoBytes);

        if (value == -1)
        {
            throw EndOfStreamException(SR::IO_EOF_ReadBeyondEOF);
        }

        return static_cast<char>(value);
    }

    void BinaryReader::InternalRead(std::vector<uint8_t>& buffer) {
        if (impl->_disposed)
            throw InvalidOperationException();

        impl->_stream->ReadExactly(buffer.data(), buffer.size());
    }

    std::string BinaryReader::ReadString() {
        if (impl->_disposed)
            throw InvalidOperationException();

        const auto stringLength = Read7BitEncodedInt();
        if (stringLength < 0)
        {
            throw IOException(SR::IO_InvalidStringLen_Len);
        }

        if (stringLength == 0)
        {
            return {};
        }

        auto charBytes = std::vector<uint8_t>(MaxCharBytesSize);
        int32_t currPos = 0;

        std::string sb{};

        do
        {
            const auto readLength = std::min(MaxCharBytesSize, stringLength - currPos);
            const auto n = impl->_stream->Read(charBytes.data(), readLength);

            if (n == 0)
            {
                throw EndOfStreamException(SR::IO_EOF_ReadBeyondEOF);
            }

            const auto chars = reinterpret_cast<std::string::value_type*>(charBytes.data());

            if (currPos == 0 && n == stringLength)
            {
                return std::string(chars);
            }

            sb.append(chars);

            currPos += n;

        } while (currPos < stringLength);

        return sb;
    }

    int32_t BinaryReader::Read(char* buffer, int32_t bufferLength, int32_t index, int32_t count) {
        ArgumentNullException::ThrowIfNull(buffer, "buffer");

        if (index < 0)
            index = 0;

        if (count < 0)
            count = 0;

        if (bufferLength - index < count) {
            throw ArgumentException(SR::Argument_InvalidOffLen);
        }

        if (impl->_disposed)
            throw InvalidOperationException();

        return InternalReadChars(buffer + index, count);
    }

    int32_t BinaryReader::Read(char* buffer, int32_t bufferLength) {
        if (impl->_disposed)
            throw InvalidOperationException();

        return InternalReadChars(buffer, bufferLength);
    }

    int32_t BinaryReader::InternalReadChars(char* buffer, int32_t bufferLength) {
        int totalCharsRead = 0;
        auto charBytes = std::vector<uint8_t>(MaxCharBytesSize);

        auto numBytes = bufferLength;

        while (bufferLength > 0)
        {
            auto numBytes = bufferLength;

            std::vector<uint8_t> byteBuffer;

            if (numBytes > MaxCharBytesSize)
            {
                numBytes = MaxCharBytesSize;
            }

            numBytes = impl->_stream->Read(charBytes.data(), numBytes);
            byteBuffer = std::vector<uint8_t>(charBytes.begin(), charBytes.begin() + numBytes);

            if (byteBuffer.empty())
            {
                break;
            }

            auto chars = reinterpret_cast<char*>(byteBuffer.data());
            auto charsRead = std::string(chars);

            bufferLength = charsRead.length();
            totalCharsRead += charsRead.length();
        }

        return totalCharsRead;
    }

    std::vector<char> BinaryReader::ReadChars(int32_t count) {
        if (count < 0)
            throw ArgumentOutOfRangeException();

        if (impl->_disposed)
            throw InvalidOperationException();

        if (count == 0)
            return std::vector<char>();

        auto chars = std::vector<char>(count);
        const auto n = InternalReadChars(chars.data(), chars.size());

        if (n != count) {
            chars = std::vector<char>(chars.begin(), chars.begin() + count);
        }

        return chars;
    }

    int32_t BinaryReader::Read(uint8_t* buffer, int32_t bufferLength, int32_t index, int32_t count) {
        ArgumentNullException::ThrowIfNull(buffer, "buffer");
        ArgumentOutOfRangeException::ThrowIfNegative(index, "index");
        ArgumentOutOfRangeException::ThrowIfNegative(count, "count");

        if (bufferLength - index < count) {
            throw ArgumentException(SR::Argument_InvalidOffLen);
        }

        if (impl->_disposed)
            throw InvalidOperationException();

        return impl->_stream->Read(buffer, bufferLength, index, count);
    }

    int32_t BinaryReader::Read(uint8_t* buffer, int32_t bufferLength) {
        if (impl->_disposed)
            throw InvalidOperationException();

        return impl->_stream->Read(buffer, bufferLength);
    }

    std::vector<uint8_t> BinaryReader::ReadBytes(int32_t count) {
        ArgumentOutOfRangeException::ThrowIfNegative(count, "count");

        if (impl->_disposed)
            throw InvalidOperationException();

        if (count == 0)
            return std::vector<uint8_t>();

        auto  result = std::vector<uint8_t>(count);
        const auto numRead = impl->_stream->ReadAtLeast(result.data(), result.size(), result.size(), false);

        if (numRead != result.size())
        {
            result = std::vector<uint8_t>(result.begin(), result.begin() + numRead);
        }

        return result;
    }

    void BinaryReader::ReadExactly(uint8_t* buffer, int32_t bufferLength) {
        if (impl->_disposed)
            throw InvalidOperationException();

        impl->_stream->ReadExactly(buffer, bufferLength);
    }

    int32_t BinaryReader::Read7BitEncodedInt() {
        uint32_t result = 0;
        uint8_t byteReadJustNow;

        constexpr int32_t MaxBytesWithoutOverflow = 4;

        for (int32_t shift = 0; shift < MaxBytesWithoutOverflow * 7; shift += 7)
        {
            byteReadJustNow = ReadByte();
            result |= (byteReadJustNow & 0x7Fu) << shift;

            if (byteReadJustNow <= 0x7Fu)
            {
                return static_cast<int32_t>(result);
            }
        }

        byteReadJustNow = ReadByte();

        if (byteReadJustNow > 15u)
        {
            //FormatException
            throw InvalidOperationException(SR::Format_Bad7BitInt);
        }

        result |= static_cast<uint32_t>(byteReadJustNow) << (MaxBytesWithoutOverflow * 7);
        return static_cast<int32_t>(result);
    }

    int64_t BinaryReader::Read7BitEncodedInt64() {
        uint64_t result = 0;
        uint8_t byteReadJustNow;

        constexpr int32_t MaxBytesWithoutOverflow = 9;

        for (int32_t shift = 0; shift < MaxBytesWithoutOverflow * 7; shift += 7)
        {
            byteReadJustNow = ReadByte();
            result |= (byteReadJustNow & 0x7Ful) << shift;

            if (byteReadJustNow <= 0x7Fu)
            {
                return static_cast<int64_t>(result);
            }
        }

        byteReadJustNow = ReadByte();

        if (byteReadJustNow > 1u)
        {
            //FormatException
            throw InvalidOperationException(SR::Format_Bad7BitInt);
        }

        result |= static_cast<int64_t>(byteReadJustNow) << (MaxBytesWithoutOverflow * 7);
        return static_cast<int64_t>(result);
    }

    void BinaryReader::FillBuffer(size_t numBytes) {
        int bytesRead = 0;
        int n = 0;   
        impl->_auxBuffer.resize(impl->minBufferSize);

        //if (m_stream == null) __Error.FileNotOpen();
        
        if (numBytes == 1) {
            n = impl->_stream->ReadByte();
            if (n == -1)
                throw CSharp::InvalidOperationException("End of file");

            impl->_auxBuffer[0] = (uint8_t)n;
            return;
        }

        do {
            n = impl->_stream->Read(impl->_auxBuffer.data(), numBytes, bytesRead, numBytes - bytesRead);
            if (n == 0) {
                throw CSharp::InvalidOperationException("End of file");
            }
            bytesRead += n;
        } while (bytesRead < numBytes);
    }

    double BinaryReader::ReadDouble() {
        FillBuffer(8);
        const auto lo = (uint32_t)(impl->_auxBuffer[0] | impl->_auxBuffer[1] << 8 |
            impl->_auxBuffer[2] << 16 | impl->_auxBuffer[3] << 24);
        const auto hi = (uint32_t)(impl->_auxBuffer[4] | impl->_auxBuffer[5] << 8 |
            impl->_auxBuffer[6] << 16 | impl->_auxBuffer[7] << 24);

        const auto tmpBuffer = ((uint64_t)hi) << 32 | lo;
        return *((double*)&tmpBuffer);
    }

    float BinaryReader::ReadSingle() {
        FillBuffer(4);
        const auto tmpBuffer = (uint32_t)(impl->_auxBuffer[0] | impl->_auxBuffer[1] << 8 | impl->_auxBuffer[2] << 16 | impl->_auxBuffer[3] << 24);
        return *((float*)&tmpBuffer);
    }

    uint64_t BinaryReader::ReadUInt64() {
        FillBuffer(8);
        const auto lo = (uint32_t)(impl->_auxBuffer[0] | impl->_auxBuffer[1] << 8 |
            impl->_auxBuffer[2] << 16 | impl->_auxBuffer[3] << 24);
        const auto hi = (uint32_t)(impl->_auxBuffer[4] | impl->_auxBuffer[5] << 8 |
            impl->_auxBuffer[6] << 16 | impl->_auxBuffer[7] << 24);
        return ((uint64_t)hi) << 32 | lo;
    }

    int64_t BinaryReader::ReadInt64() {
        FillBuffer(8);
        const auto lo = (uint32_t)(impl->_auxBuffer[0] | impl->_auxBuffer[1] << 8 |
            impl->_auxBuffer[2] << 16 | impl->_auxBuffer[3] << 24);
        const auto hi = (uint32_t)(impl->_auxBuffer[4] | impl->_auxBuffer[5] << 8 |
            impl->_auxBuffer[6] << 16 | impl->_auxBuffer[7] << 24);
        return (int64_t)((uint32_t)hi) << 32 | lo;
    }

    uint32_t BinaryReader::ReadUInt32() {
        FillBuffer(4);
        return (uint32_t)(impl->_auxBuffer[0] | impl->_auxBuffer[1] << 8
            | impl->_auxBuffer[2] << 16 | impl->_auxBuffer[3] << 24);
    }

    int32_t BinaryReader::ReadInt32() {
        FillBuffer(4);
        return (int32_t)(impl->_auxBuffer[0] | impl->_auxBuffer[1] << 8
            | impl->_auxBuffer[2] << 16 | impl->_auxBuffer[3] << 24);
    }

    uint16_t BinaryReader::ReadUInt16() {
        FillBuffer(2);
        return (uint16_t)(impl->_auxBuffer[0] | impl->_auxBuffer[1] << 8);
    }

    int16_t BinaryReader::ReadInt16() {
        FillBuffer(2);
        return (int16_t)(impl->_auxBuffer[0] | impl->_auxBuffer[1] << 8);
    }
}