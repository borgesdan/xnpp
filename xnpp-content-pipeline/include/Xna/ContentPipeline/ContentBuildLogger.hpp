#ifndef XNA_CONTENTPIPELINE_CONTENTBUILDLOGGER_HPP
#define XNA_CONTENTPIPELINE_CONTENTBUILDLOGGER_HPP

#include <string>
#include <format>
#include <iostream>
#include <memory>
#include <filesystem>
#include <stack>
#include "ContentIdentity.hpp"

namespace Xna {
	struct ContentBuildLoggerInterface {	
		virtual ~ContentBuildLoggerInterface() = default;

		virtual void LogMessage(std::string const& message) = 0;
		virtual void LogImportantMessage(std::string const& message) = 0;
		virtual void LogWarning(std::string const& helpLink, ContentIdentity contentIdentity, std::string const& message) = 0;
	};

	struct ConsoleLoggerInterface : public ContentBuildLoggerInterface{
		~ConsoleLoggerInterface() override = default;

		void LogMessage(std::string const& message) override {
			std::cout << message << std::endl;
		}

		void LogImportantMessage(std::string const& message) override {
			std::cout << "[!] -- " << message << std::endl;
		}

		void LogWarning(std::string const& helpLink, ContentIdentity contentIdentity, std::string const& message) override {
			std::cout << "[w] -- " << message << std::endl;
		}
	};

	class ContentBuildLogger final {
	public:
		inline ContentBuildLogger() { 
			impl = std::make_shared<Implementation>(); 
			impl->logger = std::make_unique<ConsoleLoggerInterface>();
		}

		inline ContentBuildLogger(std::unique_ptr<ContentBuildLoggerInterface>& interface)
			: ContentBuildLogger() {
			impl->logger = std::move(interface);
		}

		template <typename... Args>
		void LogMessage(std::string_view message, Args&&... messageArgs) {			
			std::string formatted = std::vformat(message, std::make_format_args(messageArgs...));
			impl->logger->LogMessage(formatted);
		}

		template <typename... Args>
		void LogImportantMessage(std::string_view message, Args&&... messageArgs) {
			std::string formatted = std::vformat(message, std::make_format_args(messageArgs...));
			impl->logger->LogImportantMessage(formatted);
		}

		template <typename... Args>
		void LogWarning(std::string const& helpLink, ContentIdentity contentIdentity, std::string_view message, Args&&... messageArgs) {
			std::string formatted = std::vformat(message, std::make_format_args(messageArgs...));
			impl->logger->LogWarning(formatted);
		}

		inline void PushFile(std::filesystem::path const& filename) {
			impl->fileStack.push(filename);
		}

		inline void PopFile() {
			impl->fileStack.pop();
		}

		std::filesystem::path LoggerRootDirectory() const { return impl->loggerRootDirectory; }
		void LoggerRootDirectory(std::filesystem::path const& value) const { impl->loggerRootDirectory = value; }

		inline ContentBuildLogger(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(ContentBuildLogger const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		struct Implementation {
			std::unique_ptr<ContentBuildLoggerInterface> logger;
			std::stack<std::filesystem::path> fileStack;
			std::filesystem::path loggerRootDirectory;
		};

		std::shared_ptr<Implementation> impl;
	};
}

#endif