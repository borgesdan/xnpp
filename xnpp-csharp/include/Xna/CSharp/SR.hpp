#ifndef CSHARP_STRING_RESOURCE_HPP
#define CSHARP_STRING_RESOURCE_HPP

#include <string>

namespace Xna::CSharp {
	struct SR {
		inline static const std::string Arg_ArgumentException
			= "Value does not fall within the expected range.";
		inline static const std::string ArgumentNull_Generic
			= "Value cannot be null.";
		inline static const std::string Arg_ArgumentOutOfRangeException
			= "Specified argument was out of the range of valid values.";
		inline static const std::string Arg_SystemException
			= "System error.";
		inline static const std::string Arg_InvalidOperationException
			= "Operation is not valid due to the current state of the object.";
		inline static const std::string InvalidOperation_TimeoutsNotSupported
			= "Timeouts are not supported on this stream.";
		inline static const std::string Arg_NotSupportedException
			= "Specified method is not supported.";
		inline static const std::string NotSupported_UnreadableStream
			= "Stream does not support reading.";
		inline static const std::string ObjectDisposed_StreamClosed
			= "Cannot access a closed Stream.";
		inline static const std::string Arg_IOException
			= "I/O error occurred.";
		inline static const std::string Arg_EndOfStreamException
			= "Attempted to read past the end of the stream.";
		inline static const std::string IO_EOF_ReadBeyondEOF
			= "Unable to read beyond the end of the stream.";
		inline static const std::string IO_StreamTooLong
			= "Stream was too long.";
		inline static const std::string Argument_InvalidOffLen
			= "Offset and length were out of bounds for the array or count is greater than the number of elements from index to the end of the source collection.";
		inline static const std::string NotSupported_UnwritableStream
			= "Stream does not support writing.";
		inline static const std::string Arg_UnauthorizedAccessException
			= "Attempted to perform an unauthorized operation.";
		inline static const std::string UnauthorizedAccess_MemStreamBuffer
			= "MemoryStream's internal buffer cannot be accessed.";
		inline static const std::string ArgumentOutOfRange_SmallCapacity
			= "Capacity was less than the current size.";
		inline static const std::string NotSupported_MemStreamNotExpandable
			= "Memory stream is not expandable.";
		inline static const std::string Argument_InvalidSeekOrigin
			= "Invalid seek origin.";
		inline static const std::string IO_SeekBeforeBegin
			= "An attempt was made to move the position before the beginning of the stream.";
		inline static const std::string ArgumentOutOfRange_StreamLength
			= "Stream length must be non-negative and less than 2^31 - 1 - origin.";
		inline static const std::string net_uri_NotAbsolute
			= "This operation is not supported for a relative URI.";
		inline static const std::string Argument_StreamNotReadable
			= "Stream was not readable.";
		inline static const std::string Format_Bad7BitInt
			= "Too many bytes in what should have been a 7-bit encoded integer.";
		inline static const std::string IO_InvalidStringLen_Len
			= "BinaryReader encountered an invalid string length.";
		inline static const std::string Argument_StreamNotWritable
			= "Stream was not writable.";
		inline static const std::string Overflow_TimeSpanTooLong
			= "TimeSpan overflowed because the duration is too long.";
		inline static const std::string Arg_ArithmeticException
			= "Overflow or underflow in the arithmetic operation.";
		inline static const std::string Arg_OverflowException
			= "Arithmetic operation resulted in an overflow.";
		inline static const std::string Arg_CannotBeNaN
			= "TimeSpan does not accept floating point Not-a-Number values.";
		inline static const std::string Overflow_NegateTwosCompNum
			= "Negating the minimum value of a twos complement number is invalid.";
		inline static const std::string Overflow_Duration
			= "The duration cannot be returned for TimeSpan.MinValue because the absolute value of TimeSpan.MinValue exceeds the value of TimeSpan.MaxValue.";
	};
};

#endif