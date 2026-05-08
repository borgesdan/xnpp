#include "Xna/CSharp/IO/MemoryStream.hpp"
#include <cstring>

namespace Xna::CSharp {
	MemoryStream::MemoryStream(int32_t capacity) {
		impl = std::make_shared<Implementation>();		
		impl->_buffer.resize(static_cast<size_t>(capacity));
		impl->_capacity = capacity;
		impl->_expandable = true;
		impl->_writable = true;
		impl->_exposable = true;
		impl->_isOpen = true;
	}

	MemoryStream::MemoryStream(std::vector<uint8_t> const& buffer, bool writable) {
		impl = std::make_shared<Implementation>();
		impl->_buffer = buffer;
		impl->_length = impl->_capacity = static_cast<int32_t>(buffer.size());
		impl->_writable = writable;
		impl->_isOpen = true;
	}

	MemoryStream::MemoryStream(std::vector<uint8_t>& buffer, bool writable) {
		impl = std::make_shared<Implementation>();
		impl->_length = impl->_capacity = static_cast<int32_t>(buffer.size());
		impl->_writable = writable;
		impl->_isOpen = true;
		impl->_buffer = std::move(buffer);
	}

	MemoryStream::MemoryStream(std::vector<uint8_t> const& buffer, int32_t index, int32_t count, bool writable, bool publiclyVisible) {
		if (index < 0)
			index = 0;

		if (count < 0)
			count = 0;

		if (buffer.size() - static_cast<size_t>(index < count))
			throw ArgumentException(SR::Argument_InvalidOffLen);
		impl = std::make_shared<Implementation>();

		impl->_buffer = buffer;
		impl->_origin = impl->_position = index;
		impl->_length = impl->_capacity = index + count;
		impl->_writable = writable;
		impl->_exposable = publiclyVisible;
		impl->_isOpen = true;
	}

	MemoryStream::MemoryStream(std::vector<uint8_t>& buffer, int32_t index, int32_t count, bool writable, bool publiclyVisible) {
		if (index < 0)
			index = 0;

		if (count < 0)
			count = 0;

		if (buffer.size() - static_cast<size_t>(index < count))
			throw ArgumentException(SR::Argument_InvalidOffLen);
		impl = std::make_shared<Implementation>();

		impl->_buffer = std::move(buffer);
		impl->_origin = impl->_position = index;
		impl->_length = impl->_capacity = index + count;
		impl->_writable = writable;
		impl->_exposable = publiclyVisible;
		impl->_isOpen = true;
	}

	void MemoryStream::EnsureNotClosed() const {
		if (!impl->_isOpen)
			throw InvalidOperationException(SR::ObjectDisposed_StreamClosed);
	}

	void MemoryStream::EnsureWriteable() const {
		if (!CanWrite())
			throw NotSupportedException(SR::NotSupported_UnwritableStream);
	}

	bool MemoryStream::EnsureCapacity(int32_t value) {
		if (value < 0)
			throw IOException(SR::IO_StreamTooLong);

		if (value > impl->_capacity)
		{
			auto newCapacity = std::max(value, 256);

			if (newCapacity < impl->_capacity * 2)
			{
				newCapacity = impl->_capacity * 2;
			}


			if ((impl->_capacity * 2) > MemStreamMaxLength)
			{
				newCapacity = std::max(value, MemStreamMaxLength);
			}

			Capacity(newCapacity);

			return true;
		}
		return false;
	}

	std::vector<uint8_t>& MemoryStream::GetBuffer() {
		if (!impl->_exposable) {
			throw UnauthorizedAccessException(SR::UnauthorizedAccess_MemStreamBuffer);
		}

		return impl->_buffer;
	}

	bool MemoryStream::TryGetBuffer(std::vector<uint8_t>& buffer) {
		if (!impl->_exposable)
		{
			return false;
		}

		buffer = impl->_buffer;
		return true;
	}

	int32_t MemoryStream::Capacity() const {
		EnsureNotClosed();
		return impl->_capacity - impl->_origin;
	}

	void MemoryStream::Capacity(int32_t value) {
		if (value < Length())
			throw ArgumentOutOfRangeException("value", SR::ArgumentOutOfRange_SmallCapacity);

		EnsureNotClosed();

		if (!impl->_expandable && (value != Capacity()))
			throw NotSupportedException(SR::NotSupported_MemStreamNotExpandable);

		if (impl->_expandable && value != impl->_capacity)
		{
			impl->_buffer.resize(value);
			impl->_capacity = value;
		}
	}

	int64_t MemoryStream::Length() const {
		EnsureNotClosed();
		return impl->_length - impl->_origin;
	}

	int64_t MemoryStream::Position() const {
		EnsureNotClosed();
		return impl->_position - impl->_origin;
	}

	void MemoryStream::Position(int64_t value) {
		EnsureNotClosed();

		if (value > MemStreamMaxLength - impl->_origin)
			throw ArgumentOutOfRangeException("value", SR::ArgumentOutOfRange_StreamLength);

		impl->_position = static_cast<int32_t>(impl->_origin + value);
	}

	int32_t MemoryStream::Read(uint8_t* buffer, int32_t bufferLength, int32_t offset, int32_t count) {
		ValidateBuffer(buffer, bufferLength);
		EnsureNotClosed();

		auto n = impl->_length - impl->_position;

		if (n > count)
			n = count;
		if (n <= 0)
			return 0;

		if (n <= 8)
		{
			auto byteCount = n;
			while (--byteCount >= 0) {							
				buffer[offset + byteCount] = impl->_buffer[impl->_position + byteCount];
			}
		}
		else {
			//Buffer.BlockCopy(impl->_buffer, impl->_position, buffer, offset, n);
			std::memmove(buffer + offset, impl->_buffer.data() + impl->_position, n);
		}

		impl->_position += n;

		return static_cast<int32_t>(n);
	}

	int32_t MemoryStream::Read(uint8_t* buffer, int32_t bufferLength) {
		ValidateBuffer(buffer, bufferLength);
		EnsureNotClosed();

		int n = std::min(impl->_length - impl->_position, bufferLength);

		if (n <= 0)
			return 0;

		for (size_t i = 0; i < n; ++i)
			buffer[i] = impl->_buffer[impl->_position + i];

		impl->_position += n;

		return n;
	}

	int32_t MemoryStream::ReadByte() {
		EnsureNotClosed();

		if (impl->_position >= impl->_length)
			return -1;

		return impl->_buffer[impl->_position++];
	}

	void MemoryStream::CopyTo(Stream& destination, int32_t bufferLength) {
		EnsureNotClosed();

		auto originalPosition = impl->_position;

		auto remaining = InternalEmulateRead(impl->_length - originalPosition);

		if (remaining > 0)
		{
			destination.Write(impl->_buffer.data(), static_cast<int32_t>(impl->_buffer.size()), originalPosition, remaining);
		}
	}

	int32_t MemoryStream::InternalEmulateRead(int32_t count) {
		EnsureNotClosed();

		auto n = impl->_length - impl->_position;

		if (n > count)
			n = count;
		if (n < 0)
			n = 0;
		impl->_position += n;
		return n;
	}

	int64_t MemoryStream::Seek(int64_t offset, SeekOrigin loc) {
		EnsureNotClosed();

		int32_t _loc = 0;

		switch (loc)
		{
		case CSharp::SeekOrigin::Begin:
			_loc = impl->_origin;
			break;
		case CSharp::SeekOrigin::Current:
			_loc = impl->_position;
			break;
		case CSharp::SeekOrigin::End:
			_loc = impl->_length;
			break;
		default:
			throw ArgumentException(SR::Argument_InvalidSeekOrigin);
			break;
		}

		return SeekCore(offset, _loc);
	}

	int64_t MemoryStream::SeekCore(int64_t offset, int32_t loc) {
		if (offset > MemStreamMaxLength - loc)
			throw ArgumentOutOfRangeException("offset", SR::ArgumentOutOfRange_StreamLength);

		auto tempPosition = loc + static_cast<int32_t>(offset);

		if ((loc + offset) < impl->_origin || tempPosition < impl->_origin)
			throw IOException(SR::IO_SeekBeforeBegin);

		impl->_position = tempPosition;

		return impl->_position - impl->_origin;
	}

	void MemoryStream::SetLength(int64_t value) {
		if (value < 0 || value > MemStreamMaxLength)
			throw ArgumentOutOfRangeException("value", SR::ArgumentOutOfRange_StreamLength);

		EnsureWriteable();

		if (value > (MemStreamMaxLength - impl->_origin))
			throw ArgumentOutOfRangeException("value", SR::ArgumentOutOfRange_StreamLength);

		int newLength = impl->_origin + (int)value;
		bool allocatedNewArray = EnsureCapacity(newLength);

		if (!allocatedNewArray && newLength > impl->_length)
		{
			//Array.Clear(impl->_buffer, impl->_length, newLength - impl->_length);
			for (size_t i = impl->_length; i < newLength - impl->_length; ++i) {
				impl->_buffer[i] = 0;
			}
		}

		impl->_length = newLength;

		if (impl->_position > newLength)
			impl->_position = newLength;
	}

	void MemoryStream::Write(uint8_t const* buffer, int32_t bufferLength, int32_t offset, int32_t count) {
		ValidateBuffer(buffer, bufferLength);
		EnsureNotClosed();
		EnsureWriteable();

		auto i = impl->_position + count;

		if (i < 0)
			throw IOException(SR::IO_StreamTooLong);

		if (i > impl->_length)
		{
			auto mustZero = impl->_position > impl->_length;

			if (i > impl->_capacity)
			{
				bool allocatedNewArray = EnsureCapacity(i);
				if (allocatedNewArray)
				{
					mustZero = false;
				}
			}
			if (mustZero)
			{
				//Array.Clear(impl->_buffer, impl->_length, i - impl->_length);
				for (size_t j = impl->_length; j < i - impl->_length; ++j) {
					impl->_buffer[j] = 0;
				}
			}

			impl->_length = i;
		}

		if ((count <= 8) && (buffer != impl->_buffer.data()))
		{
			auto byteCount = count;
			while (--byteCount >= 0)
			{
				impl->_buffer[impl->_position + byteCount] = buffer[offset + byteCount];
			}
		}
		else
		{
			//Buffer.BlockCopy(buffer, offset, impl->_buffer, impl->_position, count);
			std::memmove(impl->_buffer.data() + impl->_position, buffer + offset, count);
		}
		impl->_position = i;
	}

	void MemoryStream::Write(uint8_t const* buffer, int32_t bufferLength) {
		Write(buffer, bufferLength, 0, bufferLength);
	}

	void MemoryStream::WriteByte(uint8_t value) {
		EnsureNotClosed();
		EnsureWriteable();

		if (impl->_position >= impl->_length)
		{
			auto newLength = impl->_position + 1;
			auto mustZero = impl->_position > impl->_length;

			if (newLength >= impl->_capacity)
			{
				bool allocatedNewArray = EnsureCapacity(newLength);
				if (allocatedNewArray)
				{
					mustZero = false;
				}
			}
			if (mustZero)
			{
				//Array.Clear(impl->_buffer, impl->_length, impl->_position - impl->_length);
				for (size_t j = impl->_length; j < impl->_position - impl->_length; ++j) {
					impl->_buffer[j] = 0;
				}
			}
			impl->_length = newLength;
		}
		impl->_buffer[impl->_position++] = value;
	}

	void MemoryStream::WriteTo(Stream& stream) {
		EnsureNotClosed();

		stream.Write(impl->_buffer.data(), static_cast<int32_t>(impl->_buffer.size()), impl->_origin, impl->_length - impl->_origin);
	}

	std::vector<uint8_t> MemoryStream::CloseAndMove() {
		auto buffer = std::move(impl->_buffer);
		Close();
		return std::move(buffer);
	}
}