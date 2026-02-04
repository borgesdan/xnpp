#ifndef XNA_CONTENTPIPELINE_APP_HPP
#define XNA_CONTENTPIPELINE_APP_HPP

#include <Xna/Internal/TypeRegistry.hpp>
#include "Xna/ContentPipeline/ContentProcessor.hpp"
#include "Xna/ContentPipeline/ContentImporter.hpp"
#include "Xna/ContentPipeline/ImporterManager.hpp"
#include "Xna/ContentPipeline/Serialization/Compiler/ContentTypeWriter.hpp"

namespace Xna {
	//Class responsible for runtime operations related to the game executable.
	struct App final {
		//Registers the data types required for use at runtime.
		static void Register();
		static void RegisterImporters();
		static void RegisterProcessors();
		static void RegisterWriters();

		static TypeRegistry<IContentImporter>& GetImporterRegistry();
		static TypeRegistry<IContentProcessor>& GetProcessorRegistry();
		static TypeRegistry<ContentTypeWriter>& GetWriterRegistry();

		App() = delete;
		App(App&) = delete;
		App(App&&) = delete;
	};
}

#endif