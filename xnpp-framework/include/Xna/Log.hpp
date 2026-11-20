#pragma once

#include <memory>
#include <string>
#include "Xna/Macros/Api.hpp"
#include "Xna/Macros/Utils.hpp"

namespace Xna {
	struct Logger {
		virtual ~Logger() = default;
		virtual void Trace(std::string_view message) = 0;
		virtual void Info(std::string_view message) = 0;
		virtual void Warn(std::string_view message) = 0;
		virtual void Error(std::string_view message) = 0;
		virtual void Critical(std::string_view message) = 0;
	 };

	// Sistema de log que deve ser integrado e utilizado em toda a extensão do projeto, substituindo o assert padrão.
	// Contém Core (mensagens internas da engine) e Client (mensagens do jogo sendo desenvolvido nela).
	class XNPP_API Log
	{		
	public:
		XNPP_STATIC_CLASS(Log)
		//Chamado no início do bloco 'main' da aplicação
		static void Init();

		//Obtém o ponteiro interno do logger da Engine (Core).
		inline static Logger* GetCoreLogger() { return s_CoreLogger.get(); }
		// Obtém o ponteiro interno do logger do Jogo (Client)
		inline static Logger* GetClientLogger() { return s_ClientLogger.get(); }
	private:
		static std::unique_ptr<Logger> s_CoreLogger;
		static std::unique_ptr<Logger> s_ClientLogger;
		static bool s_isInitialized;
	};
}

// ========================================================================
// Macros para uso interno da Engine (Core)
// ========================================================================
#define XNPP_ENGINE_TRACE(...)    Xna::Log::GetCoreLogger()->Trace(__VA_ARGS__)
#define XNPP_ENGINE_INFO(...)     Xna::Log::GetCoreLogger()->Info(__VA_ARGS__)
#define XNPP_ENGINE_WARN(...)     Xna::Log::GetCoreLogger()->Warn(__VA_ARGS__)
#define XNPP_ENGINE_ERROR(...)    Xna::Log::GetCoreLogger()->Error(__VA_ARGS__)
#define XNPP_ENGINE_CRITICAL(...) Xna::Log::GetCoreLogger()->Critical(__VA_ARGS__)

// ========================================================================
// Macros para uso do Jogo (Client)
// ========================================================================
#define XNPP_CLIENT_TRACE(...)    Xna::Log::GetClientLogger()->Trace(__VA_ARGS__)
#define XNPP_CLIENT_INFO(...)     Xna::Log::GetClientLogger()->Info(__VA_ARGS__)
#define XNPP_CLIENT_WARN(...)     Xna::Log::GetClientLogger()->Warn(__VA_ARGS__)
#define XNPP_CLIENT_ERROR(...)    Xna::Log::GetClientLogger()->Error(__VA_ARGS__)
#define XNPP_CLIENT_CRITICAL(...) Xna::Log::GetClientLogger()->Critical(__VA_ARGS__)


