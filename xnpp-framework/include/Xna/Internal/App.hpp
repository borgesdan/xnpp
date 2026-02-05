#ifndef XNA_APP_HPP
#define XNA_APP_HPP

#include "Xna/Internal/TypeRegistry.hpp"
#include "Xna/Framework/Content/ContentTypeReader.hpp"
#include "Export.hpp"

namespace Xna {
	//Class responsible for runtime operations related to the game executable.
	struct XNPP_API App final {
		//Registers the data types required for use at runtime.
		static void RegisterContentTypeReaders();

		static TypeRegistry<ContentTypeReader>& GetContentTypeReaderActivator();

		App() = delete;
		App(App&) = delete;
		App(App&&) = delete;		
	};
}

#endif