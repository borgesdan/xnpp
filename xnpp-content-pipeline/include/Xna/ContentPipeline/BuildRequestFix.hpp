#ifndef XNA_CONTENTPIPELINE_BUILDREQUESTFIX_HPP
#define XNA_CONTENTPIPELINE_BUILDREQUESTFIX_HPP

#include "BuildRequest.hpp"

namespace Xna {
	struct BuildRequestFix {
		static inline const std::vector<std::filesystem::path> TEXTURE2D_EXTENSIONS = {
			".jpeg", ".jpg", ".png", ".tga", ".bmp", ".gif" 
		};

		static void FixRequest(CompileItem const& item, std::string& importer, std::string& processor) {
			if (importer == "TextureImporter") {
				if (FixTexture2D(item, importer, processor))
					return;
			}	
		}

	private:
		static bool FixTexture2D(CompileItem const& item, std::string& importer, std::string& processor) {
			auto extension = item.Include.extension();
			bool ok{ false };
			for (const auto& ex : TEXTURE2D_EXTENSIONS) {
				if (extension == ex) {
					ok = true;
					break;
				}
			}

			if (!ok)
				return false;

			importer = "Texture2DImporter";
			processor = "Texture2DProcessor";
			return true;
		}
	};
}

#endif