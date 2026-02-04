#ifndef XNA_IMPORTERS_WAVIMPORTERS_HPP
#define XNA_IMPORTERS_WAVIMPORTERS_HPP

#include "Xna/ContentPipeline/ContentImporter.hpp"
#include "Xna/ContentPipeline/Audio/AudioContent.hpp"

namespace Xna {
	struct WavImporter : public ContentImporter<AudioContent> {
		AudioContent ImportT(std::filesystem::path const& filename, ContentImporterContext& context) override;
	};
}

#endif