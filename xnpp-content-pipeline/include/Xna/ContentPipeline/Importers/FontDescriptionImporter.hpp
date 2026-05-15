#ifndef XNA_IMPORTERS_FONTDESCRIPTIONIMPORTER_HPP
#define XNA_IMPORTERS_FONTDESCRIPTIONIMPORTER_HPP

#include "../ContentImporter.hpp"
#include "../Graphics/FontDescription.hpp"
#include <Xna/Internal/Macros.hpp>

namespace Xna {
	struct FontDescriptionImporter : public ContentImporter<FontDescription> {
		XNPP_API FontDescription ImportT(std::filesystem::path const& filename, ContentImporterContext& context) override;
	};
}

#endif