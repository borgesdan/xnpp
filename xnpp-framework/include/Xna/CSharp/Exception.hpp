#ifndef CSHARP_EXCEPTION_HPP
#define CSHARP_EXCEPTION_HPP

#include <stdexcept>
#include <string>
#include <source_location>
#include <memory>
#include <optional>
#include "SR.hpp"

namespace Xna::CSharp {
    struct HResults {
        static constexpr size_t HR_S_OK = 0x00000000;
        static constexpr size_t HR_S_FALSE = 0x1;
        static constexpr size_t HR_COR_E_ABANDONEDMUTEX = 0x8013152D;
        static constexpr size_t HR_COR_E_AMBIGUOUSIMPLEMENTATION = 0x8013106A;
        static constexpr size_t HR_COR_E_AMBIGUOUSMATCH = 0x8000211D;
        static constexpr size_t HR_COR_E_APPDOMAINUNLOADED = 0x80131014;
        static constexpr size_t HR_COR_E_APPLICATION = 0x80131600;
        static constexpr size_t HR_COR_E_ARGUMENT = 0x80070057;
        static constexpr size_t HR_COR_E_ARGUMENTOUTOFRANGE = 0x80131502;
        static constexpr size_t HR_COR_E_ARITHMETIC = 0x80070216;
        static constexpr size_t HR_COR_E_ARRAYTYPEMISMATCH = 0x80131503;
        static constexpr size_t HR_COR_E_BADEXEFORMAT = 0x800700C1;
        static constexpr size_t HR_COR_E_BADIMAGEFORMAT = 0x8007000B;
        static constexpr size_t HR_COR_E_CANNOTUNLOADAPPDOMAIN = 0x80131015;
        static constexpr size_t HR_COR_E_CODECONTRACTFAILED = 0x80131542;
        static constexpr size_t HR_COR_E_CONTEXTMARSHAL = 0x80131504;
        static constexpr size_t HR_COR_E_CUSTOMATTRIBUTEFORMAT = 0x80131605;
        static constexpr size_t HR_COR_E_DATAMISALIGNED = 0x80131541;
        static constexpr size_t HR_COR_E_DIRECTORYNOTFOUND = 0x80070003;
        static constexpr size_t HR_COR_E_DIVIDEBYZERO = 0x80020012;
        static constexpr size_t HR_COR_E_DLLNOTFOUND = 0x80131524;
        static constexpr size_t HR_COR_E_DUPLICATEWAITOBJECT = 0x80131529;
        static constexpr size_t HR_COR_E_ENDOFSTREAM = 0x80070026;
        static constexpr size_t HR_COR_E_ENTRYPOINTNOTFOUND = 0x80131523;
        static constexpr size_t HR_COR_E_EXCEPTION = 0x80131500;
        static constexpr size_t HR_COR_E_EXECUTIONENGINE = 0x80131506;
        static constexpr size_t HR_COR_E_FAILFAST = 0x80131623;
        static constexpr size_t HR_COR_E_FIELDACCESS = 0x80131507;
        static constexpr size_t HR_COR_E_FILELOAD = 0x80131621;
        static constexpr size_t HR_COR_E_FILENOTFOUND = 0x80070002;
        static constexpr size_t HR_COR_E_FORMAT = 0x80131537;
        static constexpr size_t HR_COR_E_INDEXOUTOFRANGE = 0x80131508;
        static constexpr size_t HR_COR_E_INSUFFICIENTEXECUTIONSTACK = 0x80131578;
        static constexpr size_t HR_COR_E_INSUFFICIENTMEMORY = 0x8013153D;
        static constexpr size_t HR_COR_E_INVALIDCAST = 0x80004002;
        static constexpr size_t HR_COR_E_INVALIDCOMOBJECT = 0x80131527;
        static constexpr size_t HR_COR_E_INVALIDFILTERCRITERIA = 0x80131601;
        static constexpr size_t HR_COR_E_INVALIDOLEVARIANTTYPE = 0x80131531;
        static constexpr size_t HR_COR_E_INVALIDOPERATION = 0x80131509;
        static constexpr size_t HR_COR_E_INVALIDPROGRAM = 0x8013153A;
        static constexpr size_t HR_COR_E_IO = 0x80131620;
        static constexpr size_t HR_COR_E_KEYNOTFOUND = 0x80131577;
        static constexpr size_t HR_COR_E_MARSHALDIRECTIVE = 0x80131535;
        static constexpr size_t HR_COR_E_MEMBERACCESS = 0x8013151A;
        static constexpr size_t HR_COR_E_METHODACCESS = 0x80131510;
        static constexpr size_t HR_COR_E_MISSINGFIELD = 0x80131511;
        static constexpr size_t HR_COR_E_MISSINGMANIFESTRESOURCE = 0x80131532;
        static constexpr size_t HR_COR_E_MISSINGMEMBER = 0x80131512;
        static constexpr size_t HR_COR_E_MISSINGMETHOD = 0x80131513;
        static constexpr size_t HR_COR_E_MISSINGSATELLITEASSEMBLY = 0x80131536;
        static constexpr size_t HR_COR_E_MULTICASTNOTSUPPORTED = 0x80131514;
        static constexpr size_t HR_COR_E_NOTFINITENUMBER = 0x80131528;
        static constexpr size_t HR_COR_E_NOTSUPPORTED = 0x80131515;
        static constexpr size_t HR_COR_E_OBJECTDISPOSED = 0x80131622;
        static constexpr size_t HR_COR_E_OPERATIONCANCELED = 0x8013153B;
        static constexpr size_t HR_COR_E_OUTOFMEMORY = 0x8007000E;
        static constexpr size_t HR_COR_E_OVERFLOW = 0x80131516;
        static constexpr size_t HR_COR_E_PATHTOOLONG = 0x800700CE;
        static constexpr size_t HR_COR_E_PLATFORMNOTSUPPORTED = 0x80131539;
        static constexpr size_t HR_COR_E_RANK = 0x80131517;
        static constexpr size_t HR_COR_E_REFLECTIONTYPELOAD = 0x80131602;
        static constexpr size_t HR_COR_E_RUNTIMEWRAPPED = 0x8013153E;
        static constexpr size_t HR_COR_E_SAFEARRAYRANKMISMATCH = 0x80131538;
        static constexpr size_t HR_COR_E_SAFEARRAYTYPEMISMATCH = 0x80131533;
        static constexpr size_t HR_COR_E_SECURITY = 0x8013150A;
        static constexpr size_t HR_COR_E_SERIALIZATION = 0x8013150C;
        static constexpr size_t HR_COR_E_STACKOVERFLOW = 0x800703E9;
        static constexpr size_t HR_COR_E_SYNCHRONIZATIONLOCK = 0x80131518;
        static constexpr size_t HR_COR_E_SYSTEM = 0x80131501;
        static constexpr size_t HR_COR_E_TARGET = 0x80131603;
        static constexpr size_t HR_COR_E_TARGETINVOCATION = 0x80131604;
        static constexpr size_t HR_COR_E_TARGETPARAMCOUNT = 0x8002000E;
        static constexpr size_t HR_COR_E_THREADABORTED = 0x80131530;
        static constexpr size_t HR_COR_E_THREADINTERRUPTED = 0x80131519;
        static constexpr size_t HR_COR_E_THREADSTART = 0x80131525;
        static constexpr size_t HR_COR_E_THREADSTATE = 0x80131520;
        static constexpr size_t HR_COR_E_TIMEOUT = 0x80131505;
        static constexpr size_t HR_COR_E_TYPEACCESS = 0x80131543;
        static constexpr size_t HR_COR_E_TYPEINITIALIZATION = 0x80131534;
        static constexpr size_t HR_COR_E_TYPELOAD = 0x80131522;
        static constexpr size_t HR_COR_E_TYPEUNLOADED = 0x80131013;
        static constexpr size_t HR_COR_E_UNAUTHORIZEDACCESS = 0x80070005;
        static constexpr size_t HR_COR_E_VERIFICATION = 0x8013150D;
        static constexpr size_t HR_COR_E_WAITHANDLECANNOTBEOPENED = 0x8013152C;
        static constexpr size_t HR_CO_E_NOTINITIALIZED = 0x800401F0;
        static constexpr size_t HR_DISP_E_PARAMNOTFOUND = 0x80020004;
        static constexpr size_t HR_DISP_E_TYPEMISMATCH = 0x80020005;
        static constexpr size_t HR_DISP_E_BADVARTYPE = 0x80020008;
        static constexpr size_t HR_DISP_E_OVERFLOW = 0x8002000A;
        static constexpr size_t HR_DISP_E_DIVBYZERO = 0x80020012;
        static constexpr size_t HR_E_BOUNDS = 0x8000000B;
        static constexpr size_t HR_E_CHANGED_STATE = 0x8000000C;
        static constexpr size_t HR_E_FILENOTFOUND = 0x80070002;
        static constexpr size_t HR_E_FAIL = 0x80004005;
        static constexpr size_t HR_E_HANDLE = 0x80070006;
        static constexpr size_t HR_E_INVALIDARG = 0x80070057;
        static constexpr size_t HR_E_NOTIMPL = 0x80004001;
        static constexpr size_t HR_E_POINTER = 0x80004003;
        static constexpr size_t HR_ERROR_MRM_MAP_NOT_FOUND = 0x80073B1F;
        static constexpr size_t HR_ERROR_TIMEOUT = 0x800705B4;
        static constexpr size_t HR_RO_E_CLOSED = 0x80000013;
        static constexpr size_t HR_RPC_E_CHANGED_MODE = 0x80010106;
        static constexpr size_t HR_TYPE_E_TYPEMISMATCH = 0x80028CA0;
        static constexpr size_t HR_STG_E_PATHNOTFOUND = 0x80030003;
        static constexpr size_t HR_CTL_E_PATHNOTFOUND = 0x800A004C;
        static constexpr size_t HR_CTL_E_FILENOTFOUND = 0x800A0035;
        static constexpr size_t HR_FUSION_E_INVALID_NAME = 0x80131047;
        static constexpr size_t HR_FUSION_E_REF_DEF_MISMATCH = 0x80131040;
        static constexpr size_t HR_ERROR_TOO_MANY_OPEN_FILES = 0x80070004;
        static constexpr size_t HR_ERROR_SHARING_VIOLATION = 0x80070020;
        static constexpr size_t HR_ERROR_LOCK_VIOLATION = 0x80070021;
        static constexpr size_t HR_ERROR_OPEN_FAILED = 0x8007006E;
        static constexpr size_t HR_ERROR_DISK_CORRUPT = 0x80070571;
        static constexpr size_t HR_ERROR_UNRECOGNIZED_VOLUME = 0x800703ED;
        static constexpr size_t HR_ERROR_DLL_INIT_FAILED = 0x8007045A;
        static constexpr size_t HR_MSEE_E_ASSEMBLYLOADINPROGRESS = 0x80131016;
        static constexpr size_t HR_ERROR_FILE_INVALID = 0x800703EE;
    };

    using OptionalString = std::optional<std::string>;

#define EXCEPTION_SOURCE_LOCATION = std::source_location source = std::source_location::current()

    class Exception : public std::runtime_error {
    public:
        Exception(OptionalString const& message = std::nullopt, std::source_location source = std::source_location::current())
            : Source(source), message(message), std::runtime_error(message.value_or(""))
        {
        }
        Exception(OptionalString const& message, std::shared_ptr<Exception> const& innerException, std::source_location const& source = std::source_location::current())
            : Source(source), message(message), innerException(innerException), std::runtime_error(message.value_or(""))
        {
        }

        constexpr virtual std::string Message() const { return message.value_or(""); }

        constexpr virtual std::string FullMessage() const {
            std::string msg;

            msg.append(message.value_or(""));
            msg.append(" In: ");
            msg.append(Source.file_name());
            msg.append(" (");
            msg.append(std::to_string(Source.line()));
            msg.append(",");
            msg.append(std::to_string(Source.column()));
            msg.append("); ");
            msg.append(Source.function_name());

            return msg;

        }

        const std::shared_ptr<Exception>& InnerException() const { return innerException; }

        // Retrieves the lowest exception (inner most) for the given Exception.
        // This will traverse exceptions using the innerException property.
        virtual Exception& GetBaseException() {
            auto inner = innerException.get();
            auto back = this;

            while (inner != nullptr) {
                back = inner;
                inner = inner->innerException.get();
            }

            return *back;
        }

    public:       
        std::source_location Source;
        size_t HRresult{ HResults::HR_COR_E_EXCEPTION };

    private:
        OptionalString message;
        std::shared_ptr<Exception> innerException;
    };

    //The exception that is thrown when one of the arguments provided to a method is not valid.
    class ArgumentException : public Exception {
    public:
        ArgumentException(OptionalString const& message = std::nullopt, std::source_location const& source = std::source_location::current())
            : ArgumentException(std::nullopt, message, nullptr, source) {
        }

        ArgumentException(OptionalString const& paramName, OptionalString const& message, std::source_location const& source = std::source_location::current())
            : ArgumentException(paramName, message, nullptr, source) {
        }

        ArgumentException(OptionalString const& message, std::shared_ptr<Exception> const& innerException, std::source_location const& source = std::source_location::current())
            : ArgumentException(std::nullopt, message, innerException, source) {
        }

        ArgumentException(OptionalString const& paramName, OptionalString const& message, std::shared_ptr<Exception> const& innerException, std::source_location const& source = std::source_location::current())
            : paramName(paramName), Exception(message.value_or(SR::Arg_ArgumentException), innerException, source)
        {
            HRresult = HResults::HR_COR_E_ARGUMENT;
        }

        constexpr std::string Message() const override {
            if (paramName.has_value() && !paramName.value().empty())
                return Exception::Message().append(" Parameter: " + paramName.value() + ".");

            return Exception::Message();
        }

        constexpr std::string FullMessage() const override {
            if (paramName.has_value() && !paramName.value().empty())
                return Exception::FullMessage().append(" Parameter: " + paramName.value() + ".");

            return Exception::Message();
        }

    private:
        std::optional<std::string> paramName;
    };

    //The exception that is thrown when one of the arguments provided to a method is null.
    class ArgumentNullException : public ArgumentException {
    public:
        ArgumentNullException(OptionalString const& message = std::nullopt, std::source_location const& source = std::source_location::current())
            : ArgumentNullException(std::nullopt, message, nullptr, source) {
        }

        ArgumentNullException(OptionalString const& paramName, OptionalString const& message, std::source_location const& source = std::source_location::current())
            : ArgumentNullException(paramName, message, nullptr, source) {
        }

        ArgumentNullException(OptionalString const& message, std::shared_ptr<Exception> const& innerException, std::source_location const& source = std::source_location::current())
            : ArgumentNullException(std::nullopt, message, innerException, source) {
        }

        ArgumentNullException(OptionalString const& paramName, OptionalString const& message, std::shared_ptr<Exception> const& innerException, std::source_location const& source = std::source_location::current())
            : ArgumentException(paramName, message.value_or(SR::ArgumentNull_Generic), innerException, source)
        {
            HRresult = HResults::HR_E_POINTER;
        }

        static void ThrowIfNull(void const* argument, OptionalString const& paramName, std::source_location const& source = std::source_location::current()) {
            if (argument == nullptr)
            {
                throw ArgumentNullException(paramName, source);
            }
        }
    };

    //The exception that is thrown when the value of an argument is outside the allowable range of values as defined by the invoked method.
    class ArgumentOutOfRangeException : public ArgumentException {
    public:
        ArgumentOutOfRangeException(OptionalString const& message = std::nullopt, std::source_location const& source = std::source_location::current())
            : ArgumentOutOfRangeException(message, std::nullopt, nullptr, source) {
        }

        ArgumentOutOfRangeException(OptionalString const& paramName, OptionalString const& message, std::source_location const& source = std::source_location::current())
            : ArgumentOutOfRangeException(paramName, message, nullptr, source) {
        }

        ArgumentOutOfRangeException(OptionalString const& message, std::shared_ptr<Exception> const& innerException, std::source_location const& source = std::source_location::current())
            : ArgumentOutOfRangeException(message, std::nullopt, innerException, source) {
        }

        ArgumentOutOfRangeException(OptionalString const& paramName, OptionalString const& message, std::shared_ptr<Exception> const& innerException, std::source_location const& source = std::source_location::current())
            : ArgumentException(paramName, message.value_or(SR::Arg_ArgumentOutOfRangeException), innerException, source)
        {
            HRresult = HResults::HR_E_POINTER;
        }

        template <typename T>
        static void ThrowIfNegative(T& value, OptionalString const& paramName, std::source_location const& source = std::source_location::current()) {
            if (value < 0)
                throw ArgumentOutOfRangeException(paramName, source);
        }
    };

    class SystemException : public Exception {
    public:
        SystemException(OptionalString const& message = std::nullopt, std::source_location const& source = std::source_location::current())
            : SystemException(message, nullptr, source) {
        }

        SystemException(OptionalString const& message, std::shared_ptr<Exception> const& innerException, std::source_location const& source = std::source_location::current())
            : Exception(message.value_or(SR::Arg_SystemException), innerException, source)
        {
            HRresult = HResults::HR_COR_E_SYSTEM;
        }
    };

    //The exception that is thrown when a method call is invalid for the object's current state.
    class InvalidOperationException : public SystemException {
    public:
        InvalidOperationException(OptionalString const& message = std::nullopt, std::source_location const& source = std::source_location::current())
            : InvalidOperationException(message, nullptr, source)
        {
        }

        InvalidOperationException(OptionalString const& message, std::shared_ptr<Exception> const& innerException, std::source_location const& source = std::source_location::current())
            : SystemException(message.value_or(SR::Arg_InvalidOperationException), innerException, source)
        {
            HRresult = HResults::HR_COR_E_INVALIDOPERATION;
        }
    };

    //The exception that is thrown when an invoked method is not supported,
    //typically because it should have been implemented on a subclass.
    class NotSupportedException : public SystemException {
    public:
        NotSupportedException(OptionalString const& message = std::nullopt, std::source_location const& source = std::source_location::current())
            : NotSupportedException(message, nullptr, source)
        {
        }

        NotSupportedException(OptionalString const& message, std::shared_ptr<Exception> const& innerException, std::source_location const& source = std::source_location::current())
            : SystemException(message.value_or(SR::Arg_NotSupportedException), innerException, source)
        {
            HRresult = HResults::HR_COR_E_INVALIDOPERATION;
        }
    };

    //The exception that is thrown when the operating system denies access because of an I/O error or a specific type of security error.
    class UnauthorizedAccessException : public SystemException {
    public:
        UnauthorizedAccessException(OptionalString const& message, std::source_location const& source = std::source_location::current())
            : UnauthorizedAccessException(message, nullptr, source)
        {
        }

        UnauthorizedAccessException(OptionalString const& message, std::shared_ptr<Exception> const& innerException, std::source_location const& source = std::source_location::current())
            : SystemException(message.value_or(SR::Arg_UnauthorizedAccessException), innerException, source)
        {
            HRresult = HResults::HR_COR_E_UNAUTHORIZEDACCESS;
        }
    };

    //The exception that is thrown for errors in an arithmetic, casting, or conversion operation.
    class ArithmeticException : public SystemException {
    public:
        ArithmeticException(OptionalString const& message = std::nullopt, std::source_location const& source = std::source_location::current())
            : ArithmeticException(message, nullptr, source)
        {
        }

        ArithmeticException(OptionalString const& message, std::shared_ptr<Exception> const& innerException, std::source_location const& source = std::source_location::current())
            : SystemException(message.value_or(SR::Arg_ArithmeticException), innerException, source)
        {
            HRresult = HResults::HR_COR_E_ARITHMETIC;
        }
    };

    //The exception that is thrown for errors in an arithmetic, casting, or conversion operation.
    class OverflowException : public ArithmeticException {
    public:
        OverflowException(OptionalString const& message = std::nullopt, std::source_location const& source = std::source_location::current())
            : OverflowException(message, nullptr, source)
        {
        }

        OverflowException(OptionalString const& message, std::shared_ptr<Exception> const& innerException, std::source_location const& source = std::source_location::current())
            : ArithmeticException(message.value_or(SR::Arg_OverflowException), innerException, source)
        {
            HRresult = HResults::HR_COR_E_OVERFLOW;
        }
    };

    class IOException : public SystemException {
    public:
        IOException(OptionalString const& message = std::nullopt, std::source_location const& source = std::source_location::current())
            : SystemException(message.value_or(SR::Arg_IOException), source)
        {
            HRresult = HResults::HR_COR_E_IO;
        }

        IOException(OptionalString const& message, size_t hresult, std::source_location const& source = std::source_location::current())
            : SystemException(message.value_or(SR::Arg_IOException), source)
        {
            HRresult = hresult;
        }

        IOException(OptionalString const& message, std::shared_ptr<Exception>& innerException, std::source_location const& source = std::source_location::current())
            : SystemException(message.value_or(SR::Arg_IOException), innerException, source)
        {
            HRresult = HResults::HR_COR_E_IO;
        }
    };

    class EndOfStreamException : public IOException {
    public:
        EndOfStreamException(OptionalString const& message = std::nullopt, std::source_location const& source = std::source_location::current())
            : IOException(message.value_or(SR::Arg_IOException), source)
        {
            HRresult = HResults::HR_COR_E_ENDOFSTREAM;
        }

        EndOfStreamException(OptionalString const& message, std::shared_ptr<Exception>& innerException, std::source_location const& source = std::source_location::current())
            : IOException(message.value_or(SR::Arg_IOException), innerException, source)
        {
            HRresult = HResults::HR_COR_E_ENDOFSTREAM;
        }
    };
}

#endif