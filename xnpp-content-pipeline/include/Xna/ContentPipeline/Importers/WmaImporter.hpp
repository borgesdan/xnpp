#ifndef XNA_IMPORTERS_WMAIMPORTER_HPP
#define XNA_IMPORTERS_WMAIMPORTER_HPP

#include "Xna/ContentPipeline/ContentImporter.hpp"
#include "Xna/ContentPipeline/Audio/AudioContent.hpp"
#include <Xna/Internal/Macros.hpp>

namespace Xna {
	struct WmaImporter : public ContentImporter<AudioContent> {
		XNPP_API AudioContent ImportT(std::filesystem::path const& filename, ContentImporterContext& context) override;
	};
}

#endif