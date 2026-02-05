#ifndef CSHARP_IO_BINARYREADER_HPP
#define CSHARP_IO_BINARYREADER_HPP

#include "Stream.hpp"
#include "Xna/CSharp/Exception.hpp"
#include <optional>
#include <cstdint>
#include "Xna/Internal/Export.hpp"

namespace Xna::CSharp {
	class BinaryReader {
	public:
		XNPP_API BinaryReader(std::shared_ptr<Stream> const& input, bool leaveOpen = true);
		XNPP_API virtual std::shared_ptr<Stream> BaseStream() const;

		XNPP_API virtual void Close();

		XNPP_API virtual int32_t PeekChar();
		XNPP_API virtual int32_t Read(bool twoBytesPerChar = false);

		inline virtual uint8_t ReadByte() {
			return InternalReadByte();
		}

		virtual inline int8_t ReadSByte() {
			FillBuffer(1);
			return (int8_t)(impl->_auxBuffer[0]);
		}

		virtual inline bool ReadBoolean() {
			return InternalReadByte() != 0;
		}

		XNPP_API virtual char ReadChar(bool twoBytesPerChar = false);

		XNPP_API virtual int16_t ReadInt16();
		XNPP_API virtual uint16_t ReadUInt16();
		XNPP_API virtual int32_t ReadInt32();
		XNPP_API virtual uint32_t ReadUInt32();
		XNPP_API virtual int64_t ReadInt64();
		XNPP_API virtual uint64_t ReadUInt64();
		XNPP_API virtual float ReadSingle();
		XNPP_API virtual double ReadDouble();

		XNPP_API virtual std::string ReadString();
		XNPP_API virtual int32_t Read(char* buffer, int32_t bufferLength, int32_t index, int32_t count);
		XNPP_API virtual int32_t Read(char* buffer, int32_t bufferLength);
		XNPP_API virtual std::vector<char> ReadChars(int32_t count);
		XNPP_API virtual int32_t Read(uint8_t* buffer, int32_t bufferLength, int32_t index, int32_t count);
		XNPP_API virtual int32_t Read(uint8_t* buffer, int32_t bufferLength);
		XNPP_API virtual std::vector<uint8_t> ReadBytes(int32_t count);
		XNPP_API virtual void ReadExactly(uint8_t* buffer, int32_t bufferLength);
		XNPP_API int32_t Read7BitEncodedInt();
		XNPP_API int64_t Read7BitEncodedInt64();
		XNPP_API virtual void FillBuffer(size_t numBytes);		

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