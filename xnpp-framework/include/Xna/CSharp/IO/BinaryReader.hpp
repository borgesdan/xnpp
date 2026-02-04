#ifndef CSHARP_IO_BINARYREADER_HPP
#define CSHARP_IO_BINARYREADER_HPP

#include "Stream.hpp"
#include "Xna/CSharp/Exception.hpp"
#include <optional>
#include <cstdint>

namespace Xna::CSharp {
	class BinaryReader {
	public:
		BinaryReader(std::shared_ptr<Stream> const& input, bool leaveOpen = true);
		virtual std::shared_ptr<Stream> BaseStream() const;

		virtual void Close();

		virtual int32_t PeekChar();
		virtual int32_t Read(bool twoBytesPerChar = false);

		virtual uint8_t ReadByte() {
			return InternalReadByte();
		}

		virtual inline int8_t ReadSByte() {
			FillBuffer(1);
			return (int8_t)(impl->_auxBuffer[0]);
		}

		virtual inline bool ReadBoolean() {
			return InternalReadByte() != 0;
		}

		virtual char ReadChar(bool twoBytesPerChar = false);

		virtual int16_t ReadInt16();
		virtual uint16_t ReadUInt16();
		virtual int32_t ReadInt32();
		virtual uint32_t ReadUInt32();
		virtual int64_t ReadInt64();
		virtual uint64_t ReadUInt64();
		virtual float ReadSingle();
		virtual double ReadDouble();

		virtual std::string ReadString();
		virtual int32_t Read(char* buffer, int32_t bufferLength, int32_t index, int32_t count);
		virtual int32_t Read(char* buffer, int32_t bufferLength);
		virtual std::vector<char> ReadChars(int32_t count);
		virtual int32_t Read(uint8_t* buffer, int32_t bufferLength, int32_t index, int32_t count);
		virtual int32_t Read(uint8_t* buffer, int32_t bufferLength);
		virtual std::vector<uint8_t> ReadBytes(int32_t count);
		virtual void ReadExactly(uint8_t* buffer, int32_t bufferLength);
		int32_t Read7BitEncodedInt();
		int64_t Read7BitEncodedInt64();
		virtual void FillBuffer(size_t numBytes);		

		inline BinaryReader(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(BinaryReader const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		uint8_t InternalReadByte();
		void InternalRead(std::vector<uint8_t>& buffer);
		int32_t InternalReadChars(char* buffer, int32_t bufferLength);		

	public:
		struct Implementation {
			std::shared_ptr<Stream> _stream;
			bool _leaveOpen;
			bool _disposed{ false };
			size_t minBufferSize{16};

			std::vector<uint8_t> _auxBuffer;
		};

	protected:
		std::shared_ptr<Implementation> impl;
		static constexpr int MaxCharBytesSize = 128;
	};
}

#endif