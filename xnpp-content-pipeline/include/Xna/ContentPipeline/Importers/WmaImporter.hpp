#ifndef XNA_IMPORTERS_WMAIMPORTER_HPP
#define XNA_IMPORTERS_WMAIMPORTER_HPP

#include "Xna/ContentPipeline/ContentImporter.hpp"
#include "Xna/ContentPipeline/Audio/AudioContent.hpp"

namespace Xna {
	struct WmaImporter : public ContentImporter<AudioContent> {
		AudioContent ImportT(std::filesystem::path const& filename, ContentImporterContext& context) override;
	};
}

#endif