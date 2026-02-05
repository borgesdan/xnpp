#ifndef CSHARP_IO_MEMORYSTREAM_HPP
#define CSHARP_IO_MEMORYSTREAM_HPP

#include "Stream.hpp"
#include "Xna/Internal/Export.hpp"

namespace Xna::CSharp {
	class MemoryStream : public Stream {
	public:
		~MemoryStream() override = default;

		inline MemoryStream() : MemoryStream(0) {}
		XNPP_API MemoryStream(int32_t capacity);
		inline MemoryStream(std::vector<uint8_t> const& buffer) : MemoryStream(buffer, true) {}
		XNPP_API MemoryStream(std::vector<uint8_t> const& buffer, bool writable);
		inline MemoryStream(std::vector<uint8_t> const& buffer, int32_t index, int32_t count) : MemoryStream(buffer, index, count, true, false) {}
		inline MemoryStream(std::vector<uint8_t> const& buffer, int32_t index, int32_t count, bool writable) : MemoryStream(buffer, index, count, writable, false) {}
		XNPP_API MemoryStream(std::vector<uint8_t> const& buffer, int32_t index, int32_t count, bool writable, bool publiclyVisible);

		inline MemoryStream(std::vector<uint8_t>& buffer) : MemoryStream(buffer, true) {}
		XNPP_API MemoryStream(std::vector<uint8_t>& buffer, bool writable);
		inline MemoryStream(std::vector<uint8_t>& buffer, int32_t index, int32_t count) : MemoryStream(buffer, index, count, true, false) {}
		inline MemoryStream(std::vector<uint8_t>& buffer, int32_t index, int32_t count, bool writable) : MemoryStream(buffer, index, count, writable, false) {}
		XNPP_API MemoryStream(std::vector<uint8_t>& buffer, int32_t index, int32_t count, bool writable, bool publiclyVisible);

		inline bool CanRead() const override { return impl->_isOpen; }
		inline bool CanSeek() const override { return impl->_isOpen; }
		inline bool CanWrite() const override { return impl->_writable; }
		XNPP_API void Flush() override {}
		XNPP_API virtual std::vector<uint8_t>& GetBuffer();
		XNPP_API virtual bool TryGetBuffer(std::vector<uint8_t>& buffer);
		XNPP_API virtual int32_t Capacity() const;
		XNPP_API virtual void Capacity(int32_t value);
		XNPP_API int64_t Length() const override;
		XNPP_API int64_t Position() const override;
		XNPP_API void Position(int64_t value) override;
		XNPP_API int32_t Read(uint8_t* buffer, int32_t bufferLength, int32_t offset, int32_t count) override;
		XNPP_API int32_t Read(uint8_t* buffer, int32_t bufferLength) override;
		XNPP_API int32_t ReadByte() override;
		XNPP_API void CopyTo(Stream& destination, int32_t bufferLength) override;
		XNPP_API int32_t InternalEmulateRead(int32_t count);
		XNPP_API int64_t Seek(int64_t offset, SeekOrigin loc) override;
		XNPP_API void SetLength(int64_t value) override;
		XNPP_API void Write(uint8_t const* buffer, int32_t bufferLength, int32_t offset, int32_t count) override;
		XNPP_API void Write(uint8_t const* buffer, int32_t bufferLength) override;
		XNPP_API void WriteByte(uint8_t value) override;
		XNPP_API virtual void WriteTo(Stream& stream);

		inline void Close() override {
			if (!impl->_isOpen)
				return;

			impl->_isOpen = false;
			impl->_writable = false;
			impl->_expandable = false;
			impl->_buffer.clear();
		}

		XNPP_API std::vector<uint8_t> CloseAndMove();

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