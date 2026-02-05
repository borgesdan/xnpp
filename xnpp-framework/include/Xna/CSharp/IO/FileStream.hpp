#ifndef CSHARP_IO_FILESTREAM_HPP
#define CSHARP_IO_FILESTREAM_HPP

#include "Stream.hpp"
#include <fstream>
#include <filesystem>
#include "Xna/Internal/Export.hpp"

namespace Xna::CSharp {
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

	// Contains constants for specifying the access you want for a file.
	// You can have Read, Write or ReadWrite access.
	//
	enum class FileAccess
	{
		// Specifies read access to the file. Data can be read from the file and
		// the file pointer can be moved. Combine with WRITE for read-write access.
		Read = 1,

		// Specifies write access to the file. Data can be written to the file and
		// the file pointer can be moved. Combine with READ for read-write access.
		Write = 2,

		// Specifies read and write access to the file. Data can be written to the
		// file and the file pointer can be moved. Data can also be read from the
		// file.
		ReadWrite = 3,
	};

	// Contains constants for controlling file sharing options while
   // opening files.  You can specify what access other processes trying
   // to open the same file concurrently can have.
   //
   // Note these values currently match the values for FILE_SHARE_READ,
   // FILE_SHARE_WRITE, and FILE_SHARE_DELETE in winnt.h
   //
	enum class FileShare
	{
		// No sharing. Any request to open the file (by this process or another
		// process) will fail until the file is closed.
		None = 0,

		// Allows subsequent opening of the file for reading. If this flag is not
		// specified, any request to open the file for reading (by this process or
		// another process) will fail until the file is closed.
		Read = 1,

		// Allows subsequent opening of the file for writing. If this flag is not
		// specified, any request to open the file for writing (by this process or
		// another process) will fail until the file is closed.
		Write = 2,

		// Allows subsequent opening of the file for writing or reading. If this flag
		// is not specified, any request to open the file for writing or reading (by
		// this process or another process) will fail until the file is closed.
		ReadWrite = 3,

		// Open the file, but allow someone else to delete the file.
		Delete = 4,

		// Whether the file handle should be inheritable by child processes.
		// Note this is not directly supported like this by Win32.
		Inheritable = 0x10,
	};

	// Maps to FILE_FLAG_DELETE_ON_CLOSE and similar values from winbase.h.
	// We didn't expose a number of these values because we didn't believe
	// a number of them made sense in managed code, at least not yet.
	enum class FileOptions
	{
		// NOTE: any change to FileOptions enum needs to be
		// matched in the FileStream ctor for error validation
		None = 0,
		WriteThrough = static_cast<int32_t>(0x80000000),
		Asynchronous = static_cast<int32_t>(0x40000000), // FILE_FLAG_OVERLAPPED
		// NoBuffering = 0x20000000, // FILE_FLAG_NO_BUFFERING
		RandomAccess = 0x10000000,
		DeleteOnClose = 0x04000000,
		SequentialScan = 0x08000000,
		// AllowPosix = 0x01000000,  // FILE_FLAG_POSIX_SEMANTICS
		// BackupOrRestore = 0x02000000, // FILE_FLAG_BACKUP_SEMANTICS
		// DisallowReparsePoint = 0x00200000, // FILE_FLAG_OPEN_REPARSE_POINT
		// NoRemoteRecall = 0x00100000, // FILE_FLAG_OPEN_NO_RECALL
		// FirstPipeInstance = 0x00080000, // FILE_FLAG_FIRST_PIPE_INSTANCE
		Encrypted = 0x00004000, // FILE_ATTRIBUTE_ENCRYPTED
	};

	class FileStream : public Stream {
	public:
		inline ~FileStream() = default;

		inline FileStream(std::filesystem::path const& path, FileMode mode) : FileStream(path, mode, FileShare::None, 0) {}
		inline FileStream(std::filesystem::path const& path, FileMode mode, FileAccess access) : FileStream(path, mode, FileShare::None, 0) {}
		inline FileStream(std::filesystem::path const& path, FileMode mode, FileShare shared) : FileStream(path, mode, shared, 0) {}
		XNPP_API FileStream(std::filesystem::path const& path, FileMode mode, FileShare shared, int32_t bufferLength);

		constexpr bool CanRead() const override {
			return true;
		}

		constexpr bool CanWrite() const override {
			return true;
		}

		constexpr bool CanSeek() const override {
			return true;
		}

		XNPP_API int64_t Length() const override;
		XNPP_API int64_t Position() const override;
		XNPP_API void Position(int64_t value) override;
		XNPP_API void CopyTo(Stream& destination, int32_t bufferLength) override;
		XNPP_API void Close() override;
		XNPP_API void Flush() override { impl->stream.flush(); }
		XNPP_API int64_t Seek(int64_t offset, SeekOrigin origin) override;
		XNPP_API void SetLength(int64_t value) override;
		XNPP_API int32_t Read(uint8_t* buffer, int32_t bufferLength, int32_t offset, int32_t count) override;
		XNPP_API int32_t Read(uint8_t* buffer, int32_t bufferLength) override;
		XNPP_API int32_t ReadByte() override;
		XNPP_API void Write(uint8_t const* buffer, int32_t bufferLength, int32_t offset, int32_t count) override;
		XNPP_API void Write(uint8_t const* buffer, int32_t bufferLength) override;
		XNPP_API void WriteByte(uint8_t value) override;		

		inline FileStream(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(FileStream const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		void SetStreamLength();
		void EnsureNotClosed() const;
		void EnsureWriteable() const;

	public:
		struct Implementation {
			std::fstream stream;
			std::streampos _length{ 0 };
			std::streampos _position{ 0 };
		};

	protected:
		std::shared_ptr<Implementation> impl;		
	};
}

#endif