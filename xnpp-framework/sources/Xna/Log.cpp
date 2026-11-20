#include <Xna/Log.hpp>
#include <cstdio>
#include <memory>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace Xna {
	namespace {
		// Emergency logger if an Assert fires before Log::Init()
		struct FallbackLogger final : public Logger {
		protected:
			void LogMessage(LogLevel level, std::string_view message) override {
				const char* tag = "LOG";
				switch (level) {
				case LogLevel::Trace:    tag = "TRACE"; break;
				case LogLevel::Info:     tag = "INFO"; break;
				case LogLevel::Warn:     tag = "WARN"; break;
				case LogLevel::Error:    tag = "ERROR"; break;
				case LogLevel::Critical: tag = "CRITICAL"; break;
				}
				std::fprintf(stderr, "[UNINITIALIZED %s] %.*s\n",
					tag, static_cast<int>(message.size()), message.data());
			}
		};

		class SpdlogAdapter final : public Logger {
		public:
			explicit SpdlogAdapter(std::shared_ptr<spdlog::logger> logger)
				: m_Logger(std::move(logger)) {}

		protected:
			void LogMessage(LogLevel level, std::string_view message) override {
				switch (level) {
				case LogLevel::Trace:    m_Logger->trace(message); break;
				case LogLevel::Info:     m_Logger->info(message); break;
				case LogLevel::Warn:     m_Logger->warn(message); break;
				case LogLevel::Error:    m_Logger->error(message); break;
				case LogLevel::Critical: m_Logger->critical(message); break;
				}
			}

		private:
			std::shared_ptr<spdlog::logger> m_Logger;
		};

		FallbackLogger s_FallbackLogger;
		std::unique_ptr<Logger> s_CoreLogger;
		std::unique_ptr<Logger> s_ClientLogger;
		bool s_IsInitialized = false;
	}

	void Log::Init() {
		if (s_IsInitialized) {
			XNPP_ENGINE_WARN("Log has already been initialized. Ignoring redundant call.");
			return;
		}

		spdlog::set_pattern("%^[%T] %n: %v%$");

		auto coreSpd = spdlog::stdout_color_mt("ENGINE");
		coreSpd->set_level(spdlog::level::trace);
		s_CoreLogger = std::make_unique<SpdlogAdapter>(std::move(coreSpd));

		auto clientSpd = spdlog::stdout_color_mt("APP");
		clientSpd->set_level(spdlog::level::trace);
		s_ClientLogger = std::make_unique<SpdlogAdapter>(std::move(clientSpd));

		s_IsInitialized = true;
	}

	void Log::Shutdown() {
		s_CoreLogger.reset();
		s_ClientLogger.reset();
		spdlog::shutdown();
		s_IsInitialized = false;
	}

	Logger* Log::GetCoreLogger() noexcept {
		return s_CoreLogger ? s_CoreLogger.get() : &s_FallbackLogger;
	}

	Logger* Log::GetClientLogger() noexcept {
		return s_ClientLogger ? s_ClientLogger.get() : &s_FallbackLogger;
	}
}