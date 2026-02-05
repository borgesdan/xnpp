#ifndef XNA_CSHARP_IO_BINARYWRITER_HPP
#define XNA_CSHARP_IO_BINARYWRITER_HPP

#include <memory>
#include <cstdint>
#include "Stream.hpp"
#include "Xna/Internal/Export.hpp"

namespace Xna::CSharp {
    class BinaryWriter {
	public:
		inline BinaryWriter(std::shared_ptr<Stream> const& output)
			: BinaryWriter(output, true) {
		}

		XNPP_API BinaryWriter(std::shared_ptr<Stream> const& output, bool leaveOpen);

		XNPP_API void Close() const;

		inline virtual std::shared_ptr<Stream> BaseStream() {
			Flush();
			return impl->outStream;
		}

		inline virtual void Flush() {
			impl->outStream->Flush();
		}

		inline virtual int64_t Seek(int32_t offset, SeekOrigin origin) {
			return impl->outStream->Seek(offset, origin);
		}

		inline virtual void Write(bool value) {
			impl->outStream->WriteByte(static_cast<uint8_t>(value ? 1 : 0));
		}

		inline virtual void Write(uint8_t value) {
			impl->outStream->WriteByte(value);
		}

		inline virtual void Write(int8_t value) {
			impl->outStream->WriteByte(static_cast<uint8_t>(value));
		}

		inline virtual void Write(uint8_t const* buffer, int32_t bufferLength) {
			impl->outStream->Write(buffer, bufferLength, 0, bufferLength);
		}

		inline virtual void Write(uint8_t const* buffer, int32_t bufferLength, int32_t index, int32_t count) {
			impl->outStream->Write(buffer, bufferLength, index, count);
		}

		inline virtual void Write(char ch) {
			const auto byte = static_cast<uint8_t>(ch);
			impl->outStream->Write(&byte, 1);
		}

		XNPP_API virtual void Write(char* chars, int32_t charsLength);
		XNPP_API virtual void Write(char* chars, int32_t charsLength, int32_t index, int32_t count);

		inline virtual void Write(double value) {
			WriteNumeric(value);
		}

		inline virtual void Write(int16_t value) {
			WriteNumeric(value);
		}

		inline virtual void Write(uint16_t value) {
			WriteNumeric(value);
		}

		inline virtual void Write(int32_t value) {
			WriteNumeric(value);
		}

		inline virtual void Write(uint32_t value) {
			WriteNumeric(value);
		}

		inline virtual void Write(int64_t value) {
			WriteNumeric(value);
		}

		inline virtual void Write(uint64_t value) {
			WriteNumeric(value);
		}

		inline virtual void Write(float value) {
			WriteNumeric(value);
		}

		XNPP_API virtual void Write(std::string const& value);

		XNPP_API void Write7BitEncodedInt(int32_t value);
		XNPP_API void Write7BitEncodedInt(int64_t value);

		inline BinaryWriter(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(BinaryWriter const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	protected:
		BinaryWriter() {
			impl = std::make_shared<Implementation>();
			impl->outStream = nullptr;
		}

		template <typename TNUMERIC>
		void WriteNumeric(TNUMERIC const& value) {
			auto bytes = reinterpret_cast<const uint8_t*>(&value);
			const auto size = sizeof(value);
			impl->outStream->Write(bytes, static_cast<int32_t>(size), 0, size);
		}

    protected:
    	std::shared_ptr<Stream> OutStream() { return impl->outStream; };
		void OutStream(std::shared_ptr<Stream> const& value) { impl->outStream = value; };

	private:
		static constexpr int MaxArrayPoolRentalSize = 64 * 1024;
		static std::shared_ptr<BinaryWriter> Null;

    	struct Implementation {
    		std::shared_ptr<Stream> outStream;
    		bool _leaveOpen{ false };
    		bool _useFastUtf8{ true };
    	};

    	std::shared_ptr<Implementation> impl = nullptr;
	};
}

#endif