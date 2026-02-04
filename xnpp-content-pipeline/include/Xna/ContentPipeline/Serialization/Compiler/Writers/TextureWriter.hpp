#ifndef XNA_WRITERS_TEXTUREWRITER_HPP
#define XNA_WRITERS_TEXTUREWRITER_HPP

#include "../BuiltinTypeWriter.hpp"
#include "Xna/ContentPipeline/Graphics/TextureContent.hpp"
#include <memory>

namespace Xna {
	struct TextureWriter : BuiltinTypeWriter<std::shared_ptr<TextureContent>>{
		std::string GetRuntimeReader(TargetPlatform targetPlatform) override {
			return "TextureReader";
		}
	protected:
		void WriteT(ContentWriter& output, const std::shared_ptr<TextureContent>* value) override {
		}
	};
}

#endif