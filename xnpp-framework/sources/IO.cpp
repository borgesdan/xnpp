#include <Xna/CSharp/IO.hpp>
#include "pch.hpp"

namespace Xna {
	//////////////////////////////////////////////////////////////////
	// STREAM														//
	//////////////////////////////////////////////////////////////////

	int64_t Stream::ReadByte() {
		uint8_t oneByteArray = 0;
		auto span = std::span{ &oneByteArray, 1 };

		auto r = Read(span, 0, 1);

		return r == 0 ? -1 : oneByteArray;
	}

	int64_t Stream::ReadAtLeastCore(std::span<uint8_t> buffer, int64_t minimumBytes) {
		int32_t totalRead = 0;
		while (totalRead < minimumBytes)
		{
			const auto read = Read(buffer);
			XNPP_CORE_ASSERT(read == 0, "Unable to read beyond the end of the stream.");

			totalRead += read;
		}

		return totalRead;
	}

	//////////////////////////////////////////////////////////////////
	// MEMORY STREAM												//
	//////////////////////////////////////////////////////////////////

	static constexpr int64_t MemStreamMaxLength = std::numeric_limits<int64_t>::max();

	bool MemoryStream::EnsureCapacity(int64_t value) {
		XNPP_CORE_ASSERT(value >= 0, "Invalid value");

		if (value > _capacity)
		{
			auto newCapacity = std::max(value, int64_t(256));

			if (newCapacity < _capacity * 2)
				newCapacity = _capacity * 2;

			if ((_capacity * 2) > MemStreamMaxLength)
				newCapacity = std::max(value, MemStreamMaxLength);

			Capacity(newCapacity);

			return true;
		}

		return false;
	}

	void MemoryStream::Capacity(int64_t value) {
		XNPP_CORE_ASSERT(value >= Length(), "Capacity was less than the current size.");

		if (value != _capacity)
		{
			_buffer.resize(value);
			_capacity = value;
		}
	}

	void MemoryStream::Position(int64_t value) noexcept {
		XNPP_CORE_ASSERT(value <= (MemStreamMaxLength - _origin), "Stream length must be non-negative and less than 2^31 - 1 - origin.");
		_position = static_cast<int64_t>(_origin + value);
	}

	int64_t MemoryStream::Read(std::span<uint8_t> buffer, int64_t offset, int64_t count) {
		XNPP_CORE_ASSERT(_isOpen, "Stream is closed.");
		if (!_isOpen) return -1;

		if (_position >= _buffer.size())
			return 0;

		auto available_in_buffer = _buffer.size() - _position;
		auto n = std::min(buffer.size(), available_in_buffer);

		if (n == 0)
			return 0;

		std::copy_n(_buffer.begin() + _position, n, buffer.begin());

		_position += n;

		return static_cast<int64_t>(n);
	}

	int64_t MemoryStream::Seek(int64_t offset, SeekOrigin loc) noexcept {
		int64_t _loc = 0;

		switch (loc)
		{
		case SeekOrigin::Begin:
			_loc = _origin;
			break;
		case SeekOrigin::Current:
			_loc = _position;
			break;
		case SeekOrigin::End:
			_loc = _length;
			break;
		default:
			return -1;
		}

		return SeekCore(offset, _loc);
	}

	int64_t MemoryStream::SeekCore(int64_t offset, int64_t loc) {
		XNPP_CORE_ASSERT(offset <= (MemStreamMaxLength - loc), "Stream length must be non-negative and less than 2^31 - 1 - origin.");
		auto tempPosition = loc + offset;

		XNPP_CORE_ASSERT(!((loc + offset) < _origin || tempPosition < _origin), "An attempt was made to move the position before the beginning of the stream.");

		_position = tempPosition;

		return _position - _origin;
	}

	void MemoryStream::SetLength(int64_t value) noexcept {
		XNPP_CORE_ASSERT(value >= 0 || value <= MemStreamMaxLength || value <= (MemStreamMaxLength - _origin), "Stream length must be non-negative and less than 2^31 - 1 - origin.");

		_buffer.resize(value);

		if (_position > _buffer.size())
			_position = _buffer.size();
	}

	void MemoryStream::Write(std::span<const uint8_t> buffer, int64_t offset, int64_t count) {
		XNPP_CORE_ASSERT(_isOpen, "Stream is closed.");

		if (!_isOpen) return;

		if (buffer.empty())
			return;

		size_t space_needed = _position + buffer.size();

		if (space_needed > _buffer.size())
			_buffer.resize(space_needed);

		std::copy_n(buffer.begin(), buffer.size(), _buffer.begin() + _position);

		_position += buffer.size();
	}

	void MemoryStream::WriteByte(uint8_t value) {
		XNPP_CORE_ASSERT(_isOpen, "Stream is closed.");

		if (!_isOpen) return;

		if (_position >= _buffer.size())
			_buffer.resize(size_t(_position) + 1);

		_buffer[_position] = value;
		_position++;
	}

	std::vector<uint8_t> MemoryStream::CloseAndMove() {
		auto buffer = std::move(_buffer);
		Close();
		return std::move(buffer);
	}

	//////////////////////////////////////////////////////////////////
	// FILE STREAM													//
	//////////////////////////////////////////////////////////////////

	FileStream::FileStream(std::filesystem::path const& path, FileMode mode) {
		// 1. Iniciamos apenas como binário. Os modos in/out serão adicionados sob demanda.
		auto flags = std::fstream::binary;
		const auto exists = std::filesystem::exists(path);

		switch (mode)
		{
		case FileMode::Open:
			XNPP_CORE_ASSERT(exists, "O arquivo não existe.");

			flags |= std::fstream::in | std::fstream::out;
			break;

		case FileMode::OpenOrCreate:
			// Se existir, apenas abre (in | out).
			// Se NÃO existir, precisa do trunc para criar o arquivo no C++ (in | out | trunc).
			if (!exists)
				flags |= std::fstream::trunc;

			flags |= std::fstream::in | std::fstream::out;
			break;

		case FileMode::Create:
			// Diferente do OpenOrCreate: o Create SEMPRE destrói os dados antigos.
			flags |= std::fstream::in | std::fstream::out | std::fstream::trunc;
			break;

		case FileMode::CreateNew:
			XNPP_CORE_ASSERT(!exists, "O arquivo já existe.");
			flags |= std::fstream::in | std::fstream::out | std::fstream::trunc;
			break;

		case FileMode::Append:
			// O Append no C# só permite escrita.
			// std::fstream::out | std::fstream::app cria o arquivo automaticamente se não existir.
			flags |= std::fstream::out | std::fstream::app;
			break;

		case FileMode::Truncate:
			XNPP_CORE_ASSERT(exists, "O arquivo não existe.");

			flags |= std::fstream::in | std::fstream::out | std::fstream::trunc;
			break;

		default:
			XNPP_CORE_ASSERT(false, "Opção inválida");

			return;
		}

		_stream = std::fstream();
		_stream.open(path, flags);

		XNPP_CORE_ASSERT(_stream.good(), "Estado inválido da stream.");
	}

	int64_t FileStream::Length() const {
		XNPP_CORE_ASSERT(_stream.is_open() && _stream.good(), "Invalid stream");

		if (!_stream.is_open() || !_stream.good())
			return -1;

		// 1. Salvamos a posição atual
		auto currentPosition = _stream.tellg();

		// 2. Movemos o ponteiro para o final do arquivo
		_stream.seekg(0, std::ios::end);

		// 3. A posição no final do arquivo é, por definição, o seu tamanho
		auto length = _stream.tellg();

		// 4. Restauramos o ponteiro para a posição original onde estava
		_stream.seekg(currentPosition);

		return static_cast<int64_t>(length);
	}

	int64_t FileStream::Position() const {
		XNPP_CORE_ASSERT(_stream.is_open() && _stream.good(), "Invalid stream");

		if (!_stream.is_open() || !_stream.good())
			return -1;

		return static_cast<int64_t>(_stream.tellg());
	}

	void FileStream::Position(int64_t value) {
		XNPP_CORE_ASSERT(_stream.is_open() && _stream.good(), "Invalid stream");

		// Limpa eventuais flags de erro (como EOF) antes de mover o ponteiro
		_stream.clear();

		// Move os ponteiros de leitura (g) e escrita (p) para a posição absoluta
		_stream.seekg(value, std::ios::beg);
		_stream.seekp(value, std::ios::beg);
	}

	int64_t FileStream::Seek(int64_t offset, SeekOrigin origin) {
		XNPP_CORE_ASSERT(_stream.is_open() && _stream.good(), "Invalid stream");

		if (!_stream.is_open() || !_stream.good())
			return -1;

		// 1. Limpa as flags de erro da stream. 
		// Isso é OBRIGATÓRIO, pois se a stream estiver com a flag EOF (End Of File) 
		// ou failbit ativada, as operações seekg/seekp não farão absolutamente nada.
		_stream.clear();

		// 2. Mapeia o SeekOrigin para a direção nativa do C++
		std::ios_base::seekdir dir;
		switch (origin) {
		case SeekOrigin::Begin:
			dir = std::ios_base::beg;
			break;
		case SeekOrigin::Current:
			dir = std::ios_base::cur;
			break;
		case SeekOrigin::End:
			dir = std::ios_base::end;
			break;
		default:
			return -1; // Origem desconhecida
		}

		// O std::streamoff é o tipo nativo com sinal feito exatamente para offsets
		auto cpp_offset = static_cast<std::streamoff>(offset);

		// 3. Movemos ambos os ponteiros: leitura (g = get) e escrita (p = put).
		// Em std::fstream eles costumam apontar para a mesma posição física, 
		// mas mover os dois garante compatibilidade absoluta entre compiladores.
		_stream.seekg(cpp_offset, dir);
		_stream.seekp(cpp_offset, dir);

		// 4. Validação após o movimento
		// Se tentarmos voltar antes do byte 0 do arquivo, a stream falhará.
		if (_stream.fail()) {
			return -1;
		}

		// 5. Retorna a nova posição absoluta
		return static_cast<int64_t>(_stream.tellg());
	}

	int64_t FileStream::Read(std::span<uint8_t> buffer, int64_t offset, int64_t count) {
		XNPP_CORE_ASSERT(_stream.is_open() && _stream.good(), "Invalid stream");

		if (!_stream.is_open() || !_stream.good())
			return -1;

		if (buffer.empty())
			return 0;

		// O fstream exige um ponteiro do tipo char* (ou equivalente).
		// O reinterpret_cast é a forma correta e segura em C++ para transitar 
		// entre uint8_t* e char* para leitura de memória bruta.
		_stream.read(reinterpret_cast<char*>(buffer.data()), buffer.size());

		// gcount() retorna a quantidade exata de bytes que a stream conseguiu extrair 
		// do arquivo antes de parar, independentemente de ter dado erro ou chegado no EOF.
		auto bytesRead = _stream.gcount();

		// Se a stream estiver 'bad', ocorreu um erro severo
		if (_stream.bad())
			return -1;

		// Nota sobre o EOF em C++: Se o fstream::read tentar ler 10 bytes, mas só existirem
		// 4 restantes no arquivo, ele lerá os 4, definirá bytesRead = 4, e ligará as flags
		// 'eofbit' e 'failbit'. Isso é perfeitamente normal e esperado. Retornamos os 4.
		// Se ele já estava no fim do arquivo antes de ler, retornará 0 (EOF clássico).

		return static_cast<int64_t>(bytesRead);
	}

	int64_t FileStream::ReadByte() {
		XNPP_CORE_ASSERT(_stream.is_open() && _stream.good(), "Invalid stream");

		if (!_stream.is_open() || !_stream.good())
			return -1;

		// fstream::get() lê um único caractere e o retorna como 'int'.
		// Ele converte o byte internamente de 'unsigned char' para 'int' (0 a 255).
		int byteLido = _stream.get();

		// std::char_traits<char>::eof() (que geralmente equivale a -1) é o retorno padrão 
		// caso não consiga ler devido ao fim do arquivo.
		if (byteLido == std::char_traits<char>::eof()) {
			return -1; // -1 indica Fim de Arquivo (EOF)
		}

		// Se leu com sucesso, o valor está entre 0 e 255.
		return static_cast<int32_t>(byteLido);
	}

	//////////////////////////////////////////////////////////////////
	// BINARY READER												//
	//////////////////////////////////////////////////////////////////			

	void BinaryReader::Close() {
		// Se o usuário chamar Close explicitamente, nós fechamos o stream subjacente. 
		if (_stream) {
			_stream->Close();
		}

		if (_ownedStream) {
			_ownedStream->Close();
		}

		_stream = nullptr;
		_ownedStream = nullptr;
	}

	int32_t BinaryReader::PeekChar() {
		if (!GetStream()->CanSeek()) {
			return -1;
		}

		const auto origPos = GetStream()->Position();
		const auto ch = Read();

		GetStream()->Position(origPos);
		return ch;
	}

	int32_t BinaryReader::Read(bool twoBytesPerChar) {
		if (twoBytesPerChar) {
			FillBuffer(2);

			return static_cast<int32_t>(
				static_cast<uint32_t>(_auxBuffer[0]) |
				(static_cast<uint32_t>(_auxBuffer[1]) << 8)
				);
		}
		else {
			const auto b = GetStream()->ReadByte();

			XNPP_CORE_ASSERT(b != -1, "ReadByte() retornou -1");

			if (b == -1) return -1;

			return b;
		}
	}

	uint8_t BinaryReader::InternalReadByte() {
		const auto b = GetStream()->ReadByte();

		XNPP_CORE_ASSERT(b != -1, "ReadByte() retornou -1");

		if (b == -1) return 0;

		return static_cast<uint8_t>(b);
	}

	char BinaryReader::ReadChar(bool twoBytes) {
		const auto value = Read(twoBytes);

		XNPP_CORE_ASSERT(value != -1, "Read() retornou -1");

		if (value == -1) return 0;

		return static_cast<char>(value);
	}

	std::string BinaryReader::ReadString() {
		const auto stringLength = Read7BitEncodedInt();

		XNPP_CORE_ASSERT(stringLength > 0, "Read7BitEncodedInt() retornou um tamanho 0 para uma string.");

		if (stringLength < 1) return {};

		// 1. Otimização Crítica: Alocamos a string com o tamanho exato preenchida com zeros.
		// Isso faz uma única alocação de memória e elimina a necessidade do std::vector.
		std::string result(stringLength, '\0');

		// 2. Lemos diretamente para o buffer interno da string.
		// Convertendo o ponteiro char* da string para uint8_t* exigido pela sua Stream.
		auto bufferSpan = std::span{
			reinterpret_cast<uint8_t*>(result.data()),
			static_cast<size_t>(stringLength)
		};

		// Lemos de uma vez (ou utilizando a função apropriada do seu IStream para ler o bloco inteiro)
		const auto bytesRead = GetStream()->Read(bufferSpan);

		if (bytesRead < stringLength) {
			// Ajustamos a string para o tamanho que realmente conseguimos ler
			result.resize(std::max(int64_t(0), bytesRead));
		}

		return result;
	}

	int32_t BinaryReader::Read(char* buffer, int32_t bufferLength, int32_t index, int32_t count) {
		if (index < 0) index = 0;
		if (count < 0) count = 0;

		if (bufferLength - index < count) {
			return -1;
		}

		return InternalReadChars(buffer + index, count);
	}

	int32_t BinaryReader::Read(char* buffer, int32_t bufferLength) {
		return InternalReadChars(buffer, bufferLength);
	}

	int32_t BinaryReader::InternalReadChars(char* buffer, int32_t bufferLength) {
		int totalCharsRead = 0;
		auto charBytes = std::vector<uint8_t>(MaxCharBytesSize);

		while (bufferLength > 0) {
			int32_t numBytes = std::min(bufferLength, MaxCharBytesSize);

			numBytes = GetStream()->Read(std::span{ charBytes }.subspan(0, numBytes));

			if (numBytes <= 0) {
				break;
			}

			// Copia diretamente da span de bytes para o buffer final sem string intermediária
			const auto chars = reinterpret_cast<const char*>(charBytes.data());
			std::copy_n(chars, numBytes, buffer + totalCharsRead);

			bufferLength -= numBytes;
			totalCharsRead += numBytes;
		}

		return totalCharsRead;
	}

	std::vector<char> BinaryReader::ReadChars(int32_t count) {
		if (count <= 0) return {};

		auto chars = std::vector<char>(count);
		const auto n = InternalReadChars(chars.data(), static_cast<int32_t>(chars.size()));

		if (n != count) {
			chars.resize(n); // Mais eficiente que criar um novo vector e copiar
		}

		return chars;
	}

	int32_t BinaryReader::Read(std::span<std::uint8_t> buffer) {
		return GetStream()->Read(buffer);
	}

	std::vector<uint8_t> BinaryReader::ReadBytes(int32_t count) {
		if (count <= 0) return {};

		auto result = std::vector<uint8_t>(count);
		const auto numRead = GetStream()->ReadAtLeast(std::span{ result }.subspan(0, count), false);

		if (numRead != result.size()) {
			result.resize(numRead);
		}

		return result;
	}

	int32_t BinaryReader::Read7BitEncodedInt() {
		uint32_t result = 0;
		uint8_t byteReadJustNow;
		constexpr int32_t MaxBytesWithoutOverflow = 4;

		for (int32_t shift = 0; shift < MaxBytesWithoutOverflow * 7; shift += 7) {
			byteReadJustNow = ReadByte();

			result |= (byteReadJustNow & 0x7Fu) << shift;

			if (byteReadJustNow <= 0x7Fu) return static_cast<int32_t>(result);
		}

		byteReadJustNow = ReadByte();
		if (byteReadJustNow > 15u) return -1;

		result |= static_cast<uint32_t>(byteReadJustNow) << (MaxBytesWithoutOverflow * 7);
		return static_cast<int32_t>(result);
	}

	int64_t BinaryReader::Read7BitEncodedInt64() {
		uint64_t result = 0;
		uint8_t byteReadJustNow;
		constexpr int32_t MaxBytesWithoutOverflow = 9;

		for (int32_t shift = 0; shift < MaxBytesWithoutOverflow * 7; shift += 7) {
			byteReadJustNow = ReadByte();


			result |= static_cast<uint64_t>(byteReadJustNow & 0x7Ful) << shift;

			if (byteReadJustNow <= 0x7Fu) return static_cast<int64_t>(result);
		}

		byteReadJustNow = ReadByte();

		if (byteReadJustNow > 1u) return -1;

		result |= static_cast<uint64_t>(byteReadJustNow) << (MaxBytesWithoutOverflow * 7);
		return static_cast<int64_t>(result);
	}

	void BinaryReader::FillBuffer(size_t numBytes) {
		if (numBytes == 0) return;

		_auxBuffer.resize(std::max(minBufferSize, numBytes));

		if (numBytes == 1) {
			int n = GetStream()->ReadByte();

			if (n == -1) return;

			_auxBuffer[0] = static_cast<uint8_t>(n);
			return;
		}

		size_t bytesRead = 0;
		do {
			// Correção da sub-faixa: subspan(offset, count)
			int n = GetStream()->Read(std::span{ _auxBuffer }.subspan(bytesRead, numBytes - bytesRead));

			if (n <= 0) return;

			bytesRead += n;
		} while (bytesRead < numBytes);
	}

	int8_t BinaryReader::ReadSByte() {
		FillBuffer(1);

		return static_cast<int8_t>(_auxBuffer[0]);
	}

	double BinaryReader::ReadDouble() {
		FillBuffer(8);

		uint64_t tmpBuffer = ReadUInt64();

		// Evita undefined behavior causado por *(double*)&tmpBuffer
		return std::bit_cast<double>(tmpBuffer);
	}

	float BinaryReader::ReadSingle() {
		FillBuffer(4);

		uint32_t tmpBuffer = ReadUInt32();

		return std::bit_cast<float>(tmpBuffer);
	}

	uint64_t BinaryReader::ReadUInt64() {
		FillBuffer(8);

		const auto lo = static_cast<uint32_t>(
			static_cast<uint32_t>(_auxBuffer[0]) |
			(static_cast<uint32_t>(_auxBuffer[1]) << 8) |
			(static_cast<uint32_t>(_auxBuffer[2]) << 16) |
			(static_cast<uint32_t>(_auxBuffer[3]) << 24));
		const auto hi = static_cast<uint32_t>(
			static_cast<uint32_t>(_auxBuffer[4]) |
			(static_cast<uint32_t>(_auxBuffer[5]) << 8) |
			(static_cast<uint32_t>(_auxBuffer[6]) << 16) |
			(static_cast<uint32_t>(_auxBuffer[7]) << 24));

		return (static_cast<uint64_t>(hi) << 32) | lo;
	}

	uint32_t BinaryReader::ReadUInt32() {
		FillBuffer(4);

		return static_cast<uint32_t>(_auxBuffer[0]) |
			(static_cast<uint32_t>(_auxBuffer[1]) << 8) |
			(static_cast<uint32_t>(_auxBuffer[2]) << 16) |
			(static_cast<uint32_t>(_auxBuffer[3]) << 24);
	}

	uint16_t BinaryReader::ReadUInt16() {
		FillBuffer(2);

		return static_cast<uint16_t>(
			static_cast<uint32_t>(_auxBuffer[0]) |
			(static_cast<uint32_t>(_auxBuffer[1]) << 8)
			);
	}

	//////////////////////////////////////////////////////////////////
	// BINARY WRITER												//
	//////////////////////////////////////////////////////////////////		

	BinaryWriter::~BinaryWriter() {
		if (_ownedStream) {
			_ownedStream->Close();
		}
		else if (_outStream) {
			_outStream->Close();
		}

		_outStream = nullptr;
		_ownedStream = nullptr;
	}

	void BinaryWriter::Close() {
		if (_outStream) {
			_outStream->Flush();
			_outStream->Close();
		}
	}

	void BinaryWriter::Write(std::span<const uint8_t> buffer)
	{
		if (OutStream())
			OutStream()->Write(buffer);
	}	

	void BinaryWriter::Write(uint8_t value) {
		Write(std::span{ &value, 1 });
	}

	void BinaryWriter::Write(char ch) {
		const auto b = static_cast<uint8_t>(ch);
		Write(std::span{ &b, 1 });
	}

	void BinaryWriter::Write(std::span<const char> chars) {
		if (chars.empty()) return;
		const auto bytes = reinterpret_cast<const uint8_t*>(chars.data());
		Write(std::span{ bytes, chars.size() });
	}

	void BinaryWriter::Write(bool value) {
		Write(static_cast<uint8_t>(value ? 1 : 0));
	}

	void BinaryWriter::Write(int8_t value) {
		Write(static_cast<uint8_t>(value));
	}

	void BinaryWriter::Write(uint16_t value) {
		uint8_t buffer[2];
		buffer[0] = static_cast<uint8_t>(value & 0xFF);
		buffer[1] = static_cast<uint8_t>((value >> 8) & 0xFF);
		Write(std::span<const uint8_t>{ buffer });
	}

	void BinaryWriter::Write(int16_t value) {
		Write(static_cast<uint16_t>(value));
	}

	void BinaryWriter::Write(uint32_t value) {
		uint8_t buffer[4];
		buffer[0] = static_cast<uint8_t>(value & 0xFF);
		buffer[1] = static_cast<uint8_t>((value >> 8) & 0xFF);
		buffer[2] = static_cast<uint8_t>((value >> 16) & 0xFF);
		buffer[3] = static_cast<uint8_t>((value >> 24) & 0xFF);
		Write(std::span<const uint8_t>{ buffer });
	}

	void BinaryWriter::Write(int32_t value) {
		Write(static_cast<uint32_t>(value));
	}

	void BinaryWriter::Write(uint64_t value) {
		uint8_t buffer[8];
		buffer[0] = static_cast<uint8_t>(value & 0xFF);
		buffer[1] = static_cast<uint8_t>((value >> 8) & 0xFF);
		buffer[2] = static_cast<uint8_t>((value >> 16) & 0xFF);
		buffer[3] = static_cast<uint8_t>((value >> 24) & 0xFF);
		buffer[4] = static_cast<uint8_t>((value >> 32) & 0xFF);
		buffer[5] = static_cast<uint8_t>((value >> 40) & 0xFF);
		buffer[6] = static_cast<uint8_t>((value >> 48) & 0xFF);
		buffer[7] = static_cast<uint8_t>((value >> 56) & 0xFF);
		Write(std::span<const uint8_t>{ buffer });
	}

	void BinaryWriter::Write(int64_t value) {
		Write(static_cast<uint64_t>(value));
	}

	void BinaryWriter::Write(float value) {
		// std::bit_cast transforma os bytes do float num uint32_t com zero-overhead
		Write(std::bit_cast<uint32_t>(value));
	}

	void BinaryWriter::Write(double value) {
		Write(std::bit_cast<uint64_t>(value));
	}

	void BinaryWriter::Write(std::string_view value) {
		// O C# tipicamente espera o tamanho da string como um int32_t codificado
		Write7BitEncodedInt(static_cast<int32_t>(value.size()));

		if (!value.empty()) {
			const auto bytes = reinterpret_cast<const uint8_t*>(value.data());
			Write(std::span{ bytes, value.size() });
		}
	}

	void BinaryWriter::Write7BitEncodedInt(int32_t value) {
		auto uValue = static_cast<uint32_t>(value);
		while (uValue > 0x7Fu) {
			Write(static_cast<uint8_t>(uValue | ~0x7Fu));
			uValue >>= 7;
		}
		Write(static_cast<uint8_t>(uValue));
	}

	void BinaryWriter::Write7BitEncodedInt(int64_t value) {
		auto uValue = static_cast<uint64_t>(value); // Correção do cast original para 64 bits
		while (uValue > 0x7Fu) {
			Write(static_cast<uint8_t>(uValue | ~0x7Fu));
			uValue >>= 7;
		}
		Write(static_cast<uint8_t>(uValue));
	}
}