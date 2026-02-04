#include "Xna/CSharp/IO/FileStream.hpp"
#include <filesystem>

namespace Xna::CSharp {
	FileStream::FileStream(std::filesystem::path const& path, FileMode mode, FileShare shared, int32_t bufferLength) {
		impl = std::make_shared<Implementation>();

		auto flags = std::fstream::in
			| std::fstream::out
			| std::fstream::binary;

		const auto exists = std::filesystem::exists(path);

		switch (mode)
		{
			//Especifica se deve abrir um arquivo existente.
		case FileMode::Open:
			if (!exists)
				throw InvalidOperationException("The specified file does not exist.");
			break;
			//Especifica que se deve abrir um arquivo, se existir;
			// caso contrário, um novo arquivo deverá ser criado.
		case FileMode::OpenOrCreate:
		case FileMode::Create:
			if (!exists)
				flags |= std::fstream::trunc;
			break;
			//Especifica que o sistema operacional deve criar um novo arquivo.
			//Se o arquivo já existir, não abre o arquivo.
		case FileMode::CreateNew:
			if (!exists)
				flags |= std::fstream::trunc;
			else
				throw InvalidOperationException("The specified file already exists.");
			break;
			//Abre o arquivo, se existir, e busca o final do arquivo ou cria um novo arquivo.
		case FileMode::Append:
			if (!exists)
				flags |= std::fstream::trunc;
			else
				flags |= std::fstream::app;
			break;
			//Especifica que se deve abrir um arquivo existente.
			//Quando o arquivo for aberto, ele deverá ser truncado
			//para que seu tamanho seja zero bytes.
		case FileMode::Truncate:
			if (!exists)
				throw InvalidOperationException("The specified file does not exist.");

			flags |= std::fstream::trunc;
			break;
		default:
			throw InvalidOperationException();
			break;
		}

		impl->stream = std::fstream();
		impl->stream.open(path, flags);	

		if (!impl->stream.good())
			throw InvalidOperationException("Failed to open file: " + path.string());

		SetStreamLength();
		impl->_position = impl->stream.tellg();
	}

	void FileStream::SetStreamLength() {
		const auto pos = impl->stream.tellg();
		impl->stream.seekg(0, std::ios_base::end);

		const auto end = impl->stream.tellg();
		impl->stream.seekg(pos);

		impl->_length = end;
	}

	int64_t FileStream::Length() const {
		EnsureNotClosed();
		return static_cast<int64_t>(impl->_length);
	}

	void FileStream::EnsureNotClosed() const {
		if (!impl->stream.is_open())
			throw InvalidOperationException(SR::ObjectDisposed_StreamClosed);
	}

	int64_t FileStream::Position() const {
		EnsureNotClosed();
		return static_cast<int64_t>(impl->_position);
	}

	void FileStream::Position(int64_t value) {
		EnsureNotClosed();
		const auto _position = static_cast<std::streampos>(value);
		impl->stream.seekg(_position);
	}

	void FileStream::CopyTo(Stream& destination, int32_t bufferLength) {
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

		while ((bytesRead = Read(buffer.data(), bufferLength, 0, bufferLength)) != 0)
		{
			destination.Write(buffer.data(), bufferLength, 0, bytesRead);
		}
	}

	void FileStream::Close() {
		if (!impl->stream.is_open())
			return;

		impl->stream.close();
		impl->_position = 0;
		impl->_length = 0;
	}

	int64_t FileStream::Seek(int64_t offset, SeekOrigin origin) {
		EnsureNotClosed();
		impl->stream.seekg(static_cast<std::streamoff>(offset), static_cast<int>(origin));
		impl->_position = impl->stream.tellg();
		return static_cast<int64_t>(impl->_position);
	}

	void FileStream::SetLength(int64_t value) {
		EnsureNotClosed();
		EnsureWriteable();

		throw NotSupportedException();
	}

	void FileStream::EnsureWriteable() const {
		if (!CanWrite())
			throw NotSupportedException(SR::NotSupported_UnwritableStream);
	}

	int32_t FileStream::Read(uint8_t* buffer, int32_t bufferLength, int32_t offset, int32_t count) {
		EnsureNotClosed();
		ValidateBuffer(buffer, bufferLength);

		auto buff = reinterpret_cast<char*>(buffer);
		impl->stream.read(buff + offset, count);

		if (impl->stream.rdstate() != std::fstream::goodbit) {
			return -1;
		}

		impl->_position = impl->stream.tellg();

		return static_cast<int32_t>(impl->stream.gcount());
	}

	int32_t FileStream::Read(uint8_t* buffer, int32_t bufferLength) {
		return Read(buffer, bufferLength, 0, bufferLength);
	}

	int32_t FileStream::ReadByte() {
		EnsureNotClosed();

		char c = 0;

		impl->stream.read(&c, 1);

		if (impl->stream.rdstate() != std::fstream::goodbit) {
			return -1;
		}

		const auto uchar = static_cast<unsigned char>(c);
		const auto result = static_cast<int32_t>(uchar);

		return result;
	}

	void FileStream::Write(uint8_t const* buffer, int32_t bufferLength, int32_t offset, int32_t count) {
		EnsureNotClosed();
		EnsureWriteable();

		auto buff = reinterpret_cast<const char*>(buffer);

		impl->stream.write(buff + offset, count);

		if (impl->stream.rdstate() != std::fstream::goodbit) {
			throw InvalidOperationException();
		}

		//TODO: usar ponteiro de escrita ou de leitura?
		impl->_position = impl->stream.tellp();

		SetStreamLength();
	}

	void FileStream::Write(uint8_t const* buffer, int32_t bufferLength) {
		Write(buffer, bufferLength, 0, bufferLength);
	}

	void FileStream::WriteByte(uint8_t value) {
		EnsureNotClosed();
		EnsureWriteable();

		const auto c = reinterpret_cast<const char*>(&value);

		impl->stream.write(c, 1);

		if (impl->stream.rdstate() != std::fstream::goodbit) {
			throw InvalidOperationException();
		}

		SetStreamLength();
	}	
}