#ifndef XNA_CONTENTPIPELINE_APP_HPP
#define XNA_CONTENTPIPELINE_APP_HPP

#include <Xna/Internal/TypeRegistry.hpp>
#include "Xna/ContentPipeline/ContentProcessor.hpp"
#include "Xna/ContentPipeline/ContentImporter.hpp"
#include "Xna/ContentPipeline/ImporterManager.hpp"
#include "Xna/ContentPipeline/Serialization/Compiler/ContentTypeWriter.hpp"
#include "Xna/Internal/Macros.hpp"

namespace Xna {
	//Class responsible for runtime operations related to the game executable.
	struct App final {
		//Registers the data types required for use at runtime.
		XNPP_API static void Register();
		XNPP_API static void RegisterImporters();
		XNPP_API static void RegisterProcessors();
		XNPP_API static void RegisterWriters();

		XNPP_API static TypeRegistry<IContentImporter>& GetImporterRegistry();
		XNPP_API static TypeRegistry<IContentProcessor>& GetProcessorRegistry();
		XNPP_API static TypeRegistry<ContentTypeWriter>& GetWriterRegistry();

		App() = delete;
		App(App&) = delete;
		App(App&&) = delete;
	};
}

#endif