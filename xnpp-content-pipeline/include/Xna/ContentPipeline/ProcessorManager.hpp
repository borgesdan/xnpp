#ifndef XNA_CONTENTPIPELINE_PROCESSORMANAGER_HPP
#define XNA_CONTENTPIPELINE_PROCESSORMANAGER_HPP

#include <memory>
#include <any>
#include "OpaqueDataDictionary.hpp"
#include <Xna/CSharp/Type.hpp>
#include "ContentProcessorContext.hpp"
#include "ContentProcessor.hpp"

namespace Xna {
	class ProcessorManager {
	public:
		inline ProcessorManager() { impl = std::make_shared<Implementation>(); }

		std::shared_ptr<IContentProcessor> GetInstance(std::string& processorName, OpaqueDataDictionary const& parameters, ContentIdentity const& contentIdentity, ContentBuildLogger& buildLogger);

		inline ProcessorManager(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(ProcessorManager const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		static void SetProcessorParameters(
			std::shared_ptr<IContentProcessor>& processor,
			OpaqueDataDictionary const& parameters,
			ContentIdentity const& contentIdentity,
			ContentBuildLogger& buildLogger);

	private:
		//TODO: [!] adicionar um cache
		struct Implementation {

		};

		std::shared_ptr<Implementation> impl;
	};
}

#endif