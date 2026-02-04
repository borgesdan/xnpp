#include "Xna/CSharp/IO/Stream.hpp"

namespace Xna::CSharp {
	void Stream::CopyTo(Stream& destination, int32_t bufferLength)
	{
		if (!CanRead())
		{
			if (CanWrite())
			{
				throw NotSupportedException(SR::NotSupported_UnreadableStream);
			}

			throw InvalidOperationException(SR::ObjectDisposed_StreamClosed);
		}

		auto buffer = std::vector<uint8_t>(bufferLength);

		int32_t bytesRead = 0;

		while ((bytesRead = Read(buffer.data(), static_cast<int32_t>(buffer.size()), 0, static_cast<int32_t>(buffer.size()))) != 0)
		{
			destination.Write(buffer.data(), static_cast<int32_t>(buffer.size()), 0, bytesRead);
		}
	}

	int32_t Stream::GetCopybufferLength() const
	{
		constexpr auto DefaultCopybufferLength = 81920;
		auto bufferLength = DefaultCopybufferLength;

		if (CanSeek())
		{
			auto length = Length();
			auto position = Position();

			if (length <= position)
			{
				bufferLength = 1;
			}
			else
			{
				auto remaining = length - position;
				if (remaining > 0)
				{
					bufferLength = static_cast<int32_t>(std::min(static_cast<int64_t>(bufferLength), remaining));
				}
			}
		}

		return bufferLength;
	}

	int32_t Stream::Read(uint8_t* buffer, int32_t bufferLength) {
		ValidateBuffer(buffer, bufferLength);

		auto sharedBuffer = std::vector<uint8_t>(bufferLength);
		auto numRead = Read(sharedBuffer.data(), bufferLength, 0, bufferLength);

		if (numRead > bufferLength)
		{
			throw IOException(SR::IO_StreamTooLong);
		}

		for (size_t i = 0; i < numRead; ++i)
			buffer[i] = sharedBuffer[i];

		return numRead;
	}

	int32_t Stream::ReadByte() {
		uint8_t oneByteArray = 0;
		auto r = Read(&oneByteArray, 1, 0, 1);

		return r == 0 ? -1 : oneByteArray;
	}

	int32_t Stream::ReadAtLeastCore(uint8_t* buffer, int32_t bufferLength, int32_t minimumBytes, bool throwOnEndOfStream) {
		ValidateBuffer(buffer, bufferLength);

		int32_t totalRead = 0;
		while (totalRead < minimumBytes)
		{
			auto read = Read(buffer, bufferLength);
			if (read == 0)
			{
				if (throwOnEndOfStream)
				{
					throw EndOfStreamException(SR::IO_EOF_ReadBeyondEOF);
				}

				return totalRead;
			}

			totalRead += read;
		}

		return totalRead;
	}

	void Stream::Write(uint8_t const* buffer, int32_t bufferLength) {
		ValidateBuffer(buffer, bufferLength);

		auto sharedBuffer = std::vector<uint8_t>(bufferLength);

		for (size_t i = 0; i < bufferLength; ++i)
			sharedBuffer[i] = buffer[i];

		Write(sharedBuffer.data(), bufferLength, 0, bufferLength);
	}

	void Stream::ValidateBuffer(uint8_t const* buffer, int32_t bufferLength) {
		if (!buffer) {
			throw ArgumentNullException("buffer");
		}

		if (bufferLength < 0) {
			throw ArgumentException("bufferLength");
		}
	}
}