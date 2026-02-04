#ifndef CSHARP_IO_STREAM_HPP
#define CSHARP_IO_STREAM_HPP

#include "../Exception.hpp"
#include <cstdint>
#include <vector>
#include <limits>
#include <fstream>
#include <string>

namespace Xna::CSharp {
	// Provides seek reference points.  To seek to the end of a stream,
   // call stream.Seek(0, SeekOrigin.End).
	enum class SeekOrigin
	{
		// These constants match Win32's FILE_BEGIN, FILE_CURRENT, and FILE_END
		Begin = 0,
		Current = 1,
		End = 2,
	};

	class Stream {
	public:
		virtual bool CanRead() const = 0;
		virtual bool CanWrite() const = 0;
		virtual bool CanSeek() const = 0;
		virtual bool CanTimeout() const { return false; }
		virtual int64_t Length() const = 0;
		virtual int64_t Position() const = 0;
		virtual void Position(int64_t value) = 0;

		virtual ~Stream() = default;

		virtual int32_t ReadTimeout() {
			throw InvalidOperationException(SR::InvalidOperation_TimeoutsNotSupported);
			return 0;
		}

		virtual void ReadTimeout(int32_t value) {
			throw InvalidOperationException(SR::InvalidOperation_TimeoutsNotSupported);
		}

		virtual int32_t WriteTimeout() {
			throw InvalidOperationException(SR::InvalidOperation_TimeoutsNotSupported);
			return 0;
		}

		virtual void WriteTimeout(int32_t value) {
			throw InvalidOperationException(SR::InvalidOperation_TimeoutsNotSupported);
		}

		void CopyTo(Stream& destination) {
			CopyTo(destination, GetCopybufferLength());
		}

		virtual void CopyTo(Stream& destination, int32_t bufferLength);
		virtual void Close() {}
		virtual void Flush() = 0;
		virtual int64_t Seek(int64_t offset, SeekOrigin origin) = 0;
		virtual void SetLength(int64_t value) = 0;
		virtual int32_t Read(uint8_t* buffer, int32_t bufferLength, int32_t offset, int32_t count) = 0;
		virtual int32_t Read(uint8_t* buffer, int32_t bufferLength);
		virtual int32_t ReadByte();

		void ReadExactly(uint8_t* buffer, int32_t bufferLength) {
			ReadAtLeastCore(buffer, bufferLength, bufferLength, true);
		}

		virtual void ReadExactly(uint8_t* buffer, int32_t offset, int32_t count) {
			ReadAtLeastCore(buffer + offset, count, count, true);
		}

		int32_t ReadAtLeast(uint8_t* buffer, int32_t bufferLength, int32_t minimumBytes, bool throwOnEndOfStream = true) {
			return  ReadAtLeastCore(buffer, bufferLength, minimumBytes, throwOnEndOfStream);
		}

		virtual void Write(uint8_t const* buffer, int32_t bufferLength, int32_t offset, int32_t count) = 0;
		virtual void Write(uint8_t const* buffer, int32_t bufferLength);
		virtual void WriteByte(uint8_t value) = 0;	

	protected:
		void ValidateBuffer(uint8_t const* buffer, int32_t bufferLength);

	private:
		int32_t ReadAtLeastCore(uint8_t* buffer, int32_t bufferLength, int32_t minimumBytes, bool throwOnEndOfStream);

	private:
		int32_t GetCopybufferLength() const;
	};
}

#endif