#include "Xna/CSharp/IO/BinaryWriter.hpp"
#include "Xna/CSharp/Exception.hpp"
#include "Xna/CSharp/SR.hpp"

namespace Xna::CSharp {
    BinaryWriter::BinaryWriter(std::shared_ptr<Stream> const &output, bool leaveOpen) {
        ArgumentNullException::ThrowIfNull(output.get(), "output");

        if (!output->CanWrite())
            throw ArgumentException(SR::Argument_StreamNotWritable);

        impl = std::make_shared<Implementation>();
        impl->outStream = output;
        impl->_leaveOpen = leaveOpen;
    }

    void BinaryWriter::Close() const {
        if (impl->_leaveOpen)
            impl->outStream->Flush();
        else
            impl->outStream->Close();
    }

    void BinaryWriter::Write(char *chars, int32_t charsLength) {
        ArgumentNullException::ThrowIfNull(chars, "chars");
        auto bytes = reinterpret_cast<const uint8_t*>(chars);
        impl->outStream->Write(bytes, charsLength);
    }

    void BinaryWriter::Write(char *chars, int32_t charsLength, int32_t index, int32_t count) {
        ArgumentNullException::ThrowIfNull(chars, "chars");
        ArgumentOutOfRangeException::ThrowIfNegative(index, "index");
        ArgumentOutOfRangeException::ThrowIfNegative(count, "count");

        if (index > charsLength - count)
            throw ArgumentOutOfRangeException("index");

        auto bytes = reinterpret_cast<const uint8_t*>(chars);
        impl->outStream->Write(bytes, charsLength, index, count);
    }

    void BinaryWriter::Write(std::string const &value) {
        Write7BitEncodedInt(static_cast<int64_t>(value.size()));
        auto bytes = reinterpret_cast<const uint8_t*>(value.data());
        impl->outStream->Write(bytes, static_cast<int32_t>(value.size()));
    }

    void BinaryWriter::Write7BitEncodedInt(int32_t value) {
        auto uValue = static_cast<uint32_t>(value);

        while (uValue > 0x7Fu)
        {
            Write(static_cast<uint8_t>(uValue | ~0x7Fu));
            uValue >>= 7;
        }

        Write(static_cast<uint8_t>(uValue));
    }

    void BinaryWriter::Write7BitEncodedInt(int64_t value) {
        auto uValue = static_cast<uint32_t>(value);

        while (uValue > 0x7Fu)
        {
            Write(static_cast<uint8_t>(uValue | ~0x7Fu));
            uValue >>= 7;
        }

        Write(static_cast<uint8_t>(uValue));
    }
}
