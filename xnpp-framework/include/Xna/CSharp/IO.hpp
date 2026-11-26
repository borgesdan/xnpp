#pragma once

#include <cstdint>
#include <vector>
#include <fstream>
#include <span>
#include <cstddef>
#include <filesystem>
#include <string>
#include <Xna/Macros/Api.hpp>
#include <Xna/Macros/CoreAssert.hpp>

namespace Xna {
	// Provides seek reference points.  To seek to the end of a stream, call stream.Seek(0, SeekOrigin.End).
	enum class SeekOrigin
	{
		// These constants match Win32's FILE_BEGIN, FILE_CURRENT, and FILE_END
		Begin = 0,
		Current = 1,
		End = 2,
	};

	//////////////////////////////////////////////////////////////////
	// STREAM														//
	//////////////////////////////////////////////////////////////////

	struct XNPP_API Stream {
		virtual ~Stream() = default;

		virtual void Close() {}
		virtual void Flush() = 0;
		[[nodiscard]] virtual bool CanRead() const = 0;
		[[nodiscard]] virtual bool CanWrite() const = 0;
		[[nodiscard]] virtual bool CanSeek() const = 0;
		[[nodiscard]] virtual int64_t Length() const = 0;
		virtual void SetLength(int64_t value) = 0;
		[[nodiscard]] virtual int64_t Position() const = 0;
		virtual void Position(int64_t value) = 0;
		[[nodiscard]] virtual int64_t Seek(int64_t offset, SeekOrigin origin) = 0;
		[[nodiscard]] int64_t Read(std::span<uint8_t> buffer) { return Read(buffer, 0, buffer.size()); }
		[[nodiscard]] virtual int64_t Read(std::span<uint8_t> buffer, int64_t offset, int64_t count) = 0;
		[[nodiscard]] virtual int64_t ReadByte();

		[[nodiscard]] int64_t ReadExactly(std::span<uint8_t> buffer) {
			return ReadAtLeastCore(buffer, buffer.size());
		}

		[[nodiscard]] int64_t ReadAtLeast(std::span<uint8_t> buffer, int64_t minimumBytes) {
			return  ReadAtLeastCore(buffer, minimumBytes);
		}

		void Write(std::span<const uint8_t> buffer) { Write(buffer, 0, buffer.size()); }
		virtual void Write(std::span<const uint8_t> buffer, int64_t offset, int64_t count) = 0;
		virtual void WriteByte(uint8_t value) = 0;

		[[nodiscard]] int64_t ReadAtLeastCore(std::span<uint8_t> buffer, int64_t minimumBytes);
	};

	//////////////////////////////////////////////////////////////////
	// MEMORY STREAM												//
	//////////////////////////////////////////////////////////////////

	class XNPP_API MemoryStream final : public Stream {
	public:
		~MemoryStream() override = default;

		MemoryStream() noexcept : MemoryStream(0) {}

		MemoryStream(int64_t capacity) noexcept : _capacity(capacity), _isOpen(true) {}

		MemoryStream(std::vector<uint8_t> buffer) noexcept
			: _buffer(std::move(buffer)), _isOpen(true), _length(static_cast<int64_t>(buffer.size())), _capacity(static_cast<int64_t>(buffer.size())) {}

		constexpr void Flush() noexcept override {}

		constexpr void Close() noexcept override {
			_isOpen = false;
			_buffer.clear();
		}

		[[nodiscard]] constexpr bool CanRead() const noexcept override { return _isOpen; }
		[[nodiscard]] constexpr bool CanSeek() const noexcept override { return _isOpen; }
		[[nodiscard]] constexpr bool CanWrite() const noexcept override { return true; }
		[[nodiscard]] constexpr int64_t Length() const noexcept override { return _length - _origin; }
		[[nodiscard]] constexpr int64_t Position() const noexcept override { return _position - _origin; }

		[[nodiscard]] std::vector<uint8_t>& GetBuffer() noexcept { return _buffer; }
		[[nodiscard]] constexpr int64_t Capacity() const noexcept { return _capacity - _origin; }

		void Position(int64_t value) noexcept override;
		void Capacity(int64_t value);
		[[nodiscard]] int64_t Read(std::span<uint8_t> buffer, int64_t offset, int64_t count) override;
		[[nodiscard]] int64_t ReadByte() override { return _position >= _length ? -1 : _buffer[static_cast<size_t>(_position++)]; }
		[[nodiscard]] int64_t Seek(int64_t offset, SeekOrigin loc) noexcept override;
		void SetLength(int64_t value) noexcept override;
		void Write(std::span< const uint8_t> buffer, int64_t offset, int64_t count) override;
		void WriteByte(uint8_t value) override;
		[[nodiscard]] std::vector<uint8_t> CloseAndMove();

	private:
		[[nodiscard]] bool EnsureCapacity(int64_t value);
		[[nodiscard]] int64_t SeekCore(int64_t offset, int64_t loc);

	public:
		std::vector<uint8_t> _buffer;
		int64_t _origin{ 0 };
		int64_t _position{ 0 };
		int64_t _length{ 0 };
		int64_t _capacity{ 0 };
		bool _isOpen{ false };
	};

	//////////////////////////////////////////////////////////////////
	// FILE STREAM													//
	//////////////////////////////////////////////////////////////////

	// Contains constants for specifying how the OS should open a file.
	// These will control whether you overwrite a file, open an existing
	// file, or some combination thereof.
	// To append to a file, use Append (which maps to OpenOrCreate then we seek
	// to the end of the file).  To truncate a file or create it if it doesn't
	// exist, use Create.
	enum class FileMode
	{
		// Creates a new file. An exception is raised if the file already exists.
		CreateNew = 1,
		// Creates a new file. If the file already exists, it is overwritten.
		Create = 2,
		// Opens an existing file. An exception is raised if the file does not exist.
		Open = 3,
		// Opens the file if it exists. Otherwise, creates a new file.
		OpenOrCreate = 4,
		// Opens an existing file. Once opened, the file is truncated so that its
		// size is zero bytes. The calling process must open the file with at least
		// WRITE access. An exception is raised if the file does not exist.
		Truncate = 5,
		// Opens the file if it exists and seeks to the end.  Otherwise,
		// creates a new file.
		Append = 6,
	};

	class XNPP_API FileStream final : public Stream {
	public:
		~FileStream() override = default;

		FileStream(std::filesystem::path const& path, FileMode mode);

		void Flush() override { _stream.flush(); }
		void Close() override { _stream.close(); }

		[[nodiscard]] constexpr bool CanRead() const noexcept override { return true; }
		[[nodiscard]] constexpr bool CanWrite() const noexcept override { return true; }
		[[nodiscard]] constexpr bool CanSeek() const noexcept override { return true; }

		[[nodiscard]] int64_t Length() const override;
		[[nodiscard]] int64_t Position() const override;
		void Position(int64_t value) override;
		[[nodiscard]] int64_t Seek(int64_t offset, SeekOrigin origin) override;

		void SetLength(int64_t value) override {}
		[[nodiscard]] int64_t Read(std::span<uint8_t> buffer, int64_t offset, int64_t count) override;
		[[nodiscard]] int64_t ReadByte() override;

		void Write(std::span<const uint8_t> buffer, int64_t offset, int64_t count) override {
			_stream.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
		}

		void WriteByte(uint8_t value) override {
			_stream.put(static_cast<char>(value));
		}

	private:
		mutable std::fstream _stream;
	};

	//////////////////////////////////////////////////////////////////
	// BINARY READER												//
	//////////////////////////////////////////////////////////////////

	class XNPP_API BinaryReader {
	public:
		BinaryReader() = default;

		// Construtor Observador: Aceita um ponteiro bruto (ou referência). 
		// O BinaryReader APENAS LÊ. Ele não fecha o stream e não deleta o ponteiro.
		explicit BinaryReader(Stream* stream) : _stream(stream) {}

		// Construtor Dono: Exige a transferência de posse via std::move.
		// O BinaryReader toma posse. Ele FECHA e DELETA o stream ao ser destruído.
		explicit BinaryReader(std::unique_ptr<Stream> stream) : _ownedStream(std::move(stream)) {
			_stream = _ownedStream.get();
		}

		virtual ~BinaryReader() { if (_ownedStream) _ownedStream->Close(); }

		Stream* BaseStream() const { return GetStream(); }

		void BaseStream(std::unique_ptr<Stream> stream) {
			_ownedStream = std::move(stream);
			_stream = _ownedStream.get();
		}

		void BaseStream(Stream* stream) {
			_ownedStream = nullptr;
			_stream = stream;
		}

		virtual void Close();
		virtual int32_t PeekChar();
		virtual int32_t Read(bool twoBytesPerChar = false);
		virtual uint8_t ReadByte() { return InternalReadByte(); }
		virtual int8_t ReadSByte();
		virtual bool ReadBoolean() { return InternalReadByte() != 0; }
		virtual char ReadChar(bool twoBytesPerChar = false);
		virtual int16_t ReadInt16() { return static_cast<int16_t>(ReadUInt16()); }
		virtual uint16_t ReadUInt16();
		virtual int32_t ReadInt32() { return static_cast<int32_t>(ReadUInt32()); }
		virtual uint32_t ReadUInt32();
		virtual int64_t ReadInt64() { return static_cast<int64_t>(ReadUInt64()); }
		virtual uint64_t ReadUInt64();
		virtual float ReadSingle();
		virtual double ReadDouble();
		virtual std::string ReadString();
		virtual int32_t Read(char* buffer, int32_t bufferLength, int32_t index, int32_t count);
		virtual int32_t Read(char* buffer, int32_t bufferLength);
		virtual std::vector<char> ReadChars(int32_t count);
		virtual int32_t Read(std::span<uint8_t> buffer);
		virtual std::vector<uint8_t> ReadBytes(int32_t count);
		virtual void ReadExactly(std::span<uint8_t> buffer) { GetStream()->ReadExactly(buffer); }
		int32_t Read7BitEncodedInt();
		int64_t Read7BitEncodedInt64();
		virtual void FillBuffer(size_t numBytes);

	protected:
		virtual Stream* GetStream() const {
			if (_stream)
				return _stream;

			return _ownedStream.get();
		}
		uint8_t InternalReadByte();
		void InternalRead(std::span<std::uint8_t> buffer) { GetStream()->ReadExactly(buffer); }
		int32_t InternalReadChars(char* buffer, int32_t bufferLength);

	protected:
		size_t minBufferSize{ 16 };
		std::vector<uint8_t> _auxBuffer;
		static constexpr int MaxCharBytesSize = 128;

	private:
		// Ponteiro bruto usado para acesso rápido (sempre aponta para o stream válido)
		Stream* _stream{ nullptr };

		// Ponteiro inteligente usado APENAS se o BinaryReader for dono do Stream
		std::unique_ptr<Stream> _ownedStream{ nullptr };
	};

	//////////////////////////////////////////////////////////////////
	// BINARY WRITER												//
	//////////////////////////////////////////////////////////////////

	class XNPP_API BinaryWriter {
	public:
		BinaryWriter() = default;

		// Construtor Observador: Não toma posse do stream
		explicit BinaryWriter(Stream* stream) : _outStream(stream) {}

		// Construtor Dono: Toma posse e fecha o stream automaticamente
		explicit BinaryWriter(std::unique_ptr<Stream> stream) : _ownedStream(std::move(stream)) {
			_outStream = _ownedStream.get();
		}

		virtual ~BinaryWriter();

		virtual void Close();
		virtual void Flush() { OutStream()->Flush(); }
		virtual int64_t Seek(int32_t offset, SeekOrigin origin) { return OutStream()->Seek(offset, origin); }

		virtual void Write(std::span<const uint8_t> buffer);
		virtual void Write(char ch);
		virtual void Write(std::span<const char> chars);
		virtual void Write(bool value);
		virtual void Write(uint8_t value);
		virtual void Write(int8_t value);
		virtual void Write(int16_t value);
		virtual void Write(uint16_t value);
		virtual void Write(int32_t value);
		virtual void Write(uint32_t value);
		virtual void Write(int64_t value);
		virtual void Write(uint64_t value);
		virtual void Write(float value);
		virtual void Write(double value);
		virtual void Write(std::string_view value);
		virtual void Write7BitEncodedInt(int32_t value);
		virtual void Write7BitEncodedInt(int64_t value);

		Stream* OutStream() { return _outStream; }

		void OutStream(std::unique_ptr<Stream> outStream)
		{
			_ownedStream = std::move(outStream);
			_outStream = _ownedStream.get();
		}

		void OutStream(Stream* outStream)
		{
			_ownedStream = nullptr;
			_outStream = outStream;
		}

	private:
		Stream* _outStream{ nullptr };
		std::unique_ptr<Stream> _ownedStream{ nullptr };
	};
}