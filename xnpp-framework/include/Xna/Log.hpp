#pragma once

#include <format>
#include <string>
#include <string_view>
#include <utility>
#include "Xna/Macros/Api.hpp"
#include "Xna/Macros/Utils.hpp"

namespace Xna {
	enum class LogLevel {
		Trace,
		Info,
		Warn,
		Error,
		Critical
	};

	struct Logger {
		virtual ~Logger() = default;

		// Overloads for plain text (without parsing of curly braces {})
		void Trace(std::string_view message) { LogMessage(LogLevel::Trace, message); }
		void Info(std::string_view message) { LogMessage(LogLevel::Info, message); }
		void Warn(std::string_view message) { LogMessage(LogLevel::Warn, message); }
		void Error(std::string_view message) { LogMessage(LogLevel::Error, message); }
		void Critical(std::string_view message) { LogMessage(LogLevel::Critical, message); }

		// Overloads for formatted strings (with parsing of curly braces {}) - C++20
		template <typename... Args>
			requires (sizeof...(Args) > 0)
		void Trace(std::format_string<Args...> fmt, Args&&... args) {
			LogMessage(LogLevel::Trace, std::format(fmt, std::forward<Args>(args)...));
		}

		template <typename... Args>
			requires (sizeof...(Args) > 0)
		void Info(std::format_string<Args...> fmt, Args&&... args) {
			LogMessage(LogLevel::Info, std::format(fmt, std::forward<Args>(args)...));
		}

		template <typename... Args>
			requires (sizeof...(Args) > 0)
		void Warn(std::format_string<Args...> fmt, Args&&... args) {
			LogMessage(LogLevel::Warn, std::format(fmt, std::forward<Args>(args)...));
		}

		template <typename... Args>
			requires (sizeof...(Args) > 0)
		void Error(std::format_string<Args...> fmt, Args&&... args) {
			LogMessage(LogLevel::Error, std::format(fmt, std::forward<Args>(args)...));
		}

		template <typename... Args>
			requires (sizeof...(Args) > 0)
		void Critical(std::format_string<Args...> fmt, Args&&... args) {
			LogMessage(LogLevel::Critical, std::format(fmt, std::forward<Args>(args)...));
		}

	protected:
		virtual void LogMessage(LogLevel level, std::string_view message) = 0;
	};

	class XNPP_API Log {
		XNPP_STATIC_CLASS(Log)
	public:
		static void Init();
		static void Shutdown();

		[[nodiscard]] static Logger* GetCoreLogger() noexcept;
		[[nodiscard]] static Logger* GetClientLogger() noexcept;
	};
}

// ========================================================================
// Macros para uso interno da Engine (Core)
// ========================================================================
#define XNPP_ENGINE_TRACE(...)    ::Xna::Log::GetCoreLogger()->Trace(__VA_ARGS__)
#define XNPP_ENGINE_INFO(...)     ::Xna::Log::GetCoreLogger()->Info(__VA_ARGS__)
#define XNPP_ENGINE_WARN(...)     ::Xna::Log::GetCoreLogger()->Warn(__VA_ARGS__)
#define XNPP_ENGINE_ERROR(...)    ::Xna::Log::GetCoreLogger()->Error(__VA_ARGS__)
#define XNPP_ENGINE_CRITICAL(...) ::Xna::Log::GetCoreLogger()->Critical(__VA_ARGS__)

// ========================================================================
// Macros para uso do Jogo (Client)
// ========================================================================
#define XNPP_CLIENT_TRACE(...)    ::Xna::Log::GetClientLogger()->Trace(__VA_ARGS__)
#define XNPP_CLIENT_INFO(...)     ::Xna::Log::GetClientLogger()->Info(__VA_ARGS__)
#define XNPP_CLIENT_WARN(...)     ::Xna::Log::GetClientLogger()->Warn(__VA_ARGS__)
#define XNPP_CLIENT_ERROR(...)    ::Xna::Log::GetClientLogger()->Error(__VA_ARGS__)
#define XNPP_CLIENT_CRITICAL(...) ::Xna::Log::GetClientLogger()->Critical(__VA_ARGS__)