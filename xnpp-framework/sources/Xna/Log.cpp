#include <Xna/Log.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace Xna {
    struct InternalLogger : public Logger 
    {
		InternalLogger(std::shared_ptr<spdlog::logger> const& logger) : logger(logger) {}

        void Trace(std::string_view message) override {
            logger->trace(message);
        }

        void Info(std::string_view message) override {
            logger->info(message);
        }
        void Warn(std::string_view message) override {
            logger->warn(message);
        }
        void Error(std::string_view message) override {
            logger->error(message);
        }
        void Critical(std::string_view message) override {
            logger->critical(message);
        }

        std::shared_ptr<spdlog::logger> logger;
    };

    std::unique_ptr<Logger> Log::s_CoreLogger;
    std::unique_ptr<Logger> Log::s_ClientLogger;
    bool Log::s_isInitialized = false;

    void Log::Init()
    {
        if (s_isInitialized) {
            XNPP_ENGINE_WARN("Log já foi inicializado. Ignorando chamada redundante.");
            return;
        }

        // Define o padrão visual da mensagem. 
        // %^ e %$ controlam o bloco de cores
        // %T é a hora, %n é o nome do logger, %v é a mensagem atual
        spdlog::set_pattern("%^[%T] %n: %v%$");

        // Inicializa o logger do Core (Engine)
        auto coreLogger = std::make_unique<InternalLogger>(spdlog::stdout_color_mt("ENGINE"));
		coreLogger->logger->set_level(spdlog::level::trace);
		s_CoreLogger = std::move(coreLogger);

        // Inicializa o logger do Client (Jogo/App)
        auto clientLogger = std::make_unique<InternalLogger>(spdlog::stdout_color_mt("APP"));
        clientLogger->logger->set_level(spdlog::level::trace);
        s_ClientLogger = std::move(clientLogger);

        s_isInitialized = true;
    }
}
