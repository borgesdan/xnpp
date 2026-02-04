#ifndef XNA_IMPORTERS_FONTDESCRIPTIONIMPORTER_HPP
#define XNA_IMPORTERS_FONTDESCRIPTIONIMPORTER_HPP

#include "../ContentImporter.hpp"
#include "../Graphics/FontDescription.hpp"

namespace Xna {
	struct FontDescriptionImporter : public ContentImporter<FontDescription> {
		FontDescription ImportT(std::filesystem::path const& filename, ContentImporterContext& context) override;
	};
}

#endif