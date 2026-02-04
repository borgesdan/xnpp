#ifndef XNA_CONTENTIMPORTER_TEXTUREIMPORTER_HPP
#define XNA_CONTENTIMPORTER_TEXTUREIMPORTER_HPP

#include "Xna/ContentPipeline/ContentImporter.hpp"
#include "../Graphics/TextureContent.hpp"
#include "../Graphics/Texture2DContent.hpp"
#include <memory>


namespace Xna {
	struct Texture2DImporter : public ContentImporter<Texture2DContent> {
		Texture2DContent ImportT(std::filesystem::path const& file, ContentImporterContext& context) override;	
	};
}

#endif