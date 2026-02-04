#ifndef CSHARP_IO_MEMORYSTREAM_HPP
#define CSHARP_IO_MEMORYSTREAM_HPP

#include "Stream.hpp"

namespace Xna::CSharp {
	class MemoryStream : public Stream {
	public:
		~MemoryStream() override = default;

		inline MemoryStream() : MemoryStream(0) {}
		MemoryStream(int32_t capacity);		
		inline MemoryStream(std::vector<uint8_t> const& buffer) : MemoryStream(buffer, true) {}
		MemoryStream(std::vector<uint8_t> const& buffer, bool writable);
		inline MemoryStream(std::vector<uint8_t> const& buffer, int32_t index, int32_t count) : MemoryStream(buffer, index, count, true, false) {}
		inline MemoryStream(std::vector<uint8_t> const& buffer, int32_t index, int32_t count, bool writable) : MemoryStream(buffer, index, count, writable, false) {}
		MemoryStream(std::vector<uint8_t> const& buffer, int32_t index, int32_t count, bool writable, bool publiclyVisible);

		inline MemoryStream(std::vector<uint8_t>& buffer) : MemoryStream(buffer, true) {}
		MemoryStream(std::vector<uint8_t>& buffer, bool writable);
		inline MemoryStream(std::vector<uint8_t>& buffer, int32_t index, int32_t count) : MemoryStream(buffer, index, count, true, false) {}
		inline MemoryStream(std::vector<uint8_t>& buffer, int32_t index, int32_t count, bool writable) : MemoryStream(buffer, index, count, writable, false) {}
		MemoryStream(std::vector<uint8_t>& buffer, int32_t index, int32_t count, bool writable, bool publiclyVisible);

		bool CanRead() const override { return impl->_isOpen; }
		bool CanSeek() const override { return impl->_isOpen; }
		bool CanWrite() const override { return impl->_writable; }
		void Flush() override {}
		virtual std::vector<uint8_t>& GetBuffer();
		virtual bool TryGetBuffer(std::vector<uint8_t>& buffer);
		virtual int32_t Capacity() const;
		virtual void Capacity(int32_t value);
		int64_t Length() const override;
		int64_t Position() const override;
		void Position(int64_t value) override;
		int32_t Read(uint8_t* buffer, int32_t bufferLength, int32_t offset, int32_t count) override;
		int32_t Read(uint8_t* buffer, int32_t bufferLength) override;
		int32_t ReadByte() override;
		void CopyTo(Stream& destination, int32_t bufferLength) override;
		int32_t InternalEmulateRead(int32_t count);
		int64_t Seek(int64_t offset, SeekOrigin loc) override;
		void SetLength(int64_t value) override;
		void Write(uint8_t const* buffer, int32_t bufferLength, int32_t offset, int32_t count) override;
		void Write(uint8_t const* buffer, int32_t bufferLength) override;
		void WriteByte(uint8_t value) override;
		virtual void WriteTo(Stream& stream);

		void Close() override {
			if (!impl->_isOpen)
				return;

			impl->_isOpen = false;
			impl->_writable = false;
			impl->_expandable = false;
			impl->_buffer.clear();
		}

		std::vector<uint8_t> CloseAndMove();

		inline MemoryStream(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(MemoryStream const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		void EnsureNotClosed() const;
		void EnsureWriteable() const;
		bool EnsureCapacity(int32_t value);
		int64_t SeekCore(int64_t offset, int32_t loc);

		inline static constexpr int32_t MemStreamMaxLength = std::numeric_limits<int32_t>::max();

	public:
		struct Implementation {
			std::vector<uint8_t> _buffer;
			int32_t _origin{ 0 };
			int32_t _position{ 0 };
			int32_t _length{ 0 };
			int32_t _capacity{ 0 };
			bool _expandable{ false };
			bool _writable{ false };
			bool _exposable{ false };
			bool _isOpen{ false };
		};

	protected:
		std::shared_ptr<Implementation> impl;
	};

}

#endif