#ifndef XNA_WRITERS_TEXTURE2DWRITER_HPP
#define XNA_WRITERS_TEXTURE2DWRITER_HPP

#include "BaseTextureWriter.hpp"
#include <Xna/Framework/Graphics/Texture2D.hpp>
#include <memory>
#include "Xna/ContentPipeline/Graphics/Texture2DContent.hpp"

namespace Xna {
	struct Texture2DWriter : public BaseTextureWriter<Texture2DContent, Texture2D> {
		inline void WriteTextureHeader(ContentWriter& output, SurfaceFormat format,
			size_t width, size_t height, size_t depth, size_t mipLevels) override {		
			ContentWriter::Base& output1 = output;
			output1.Write(static_cast<int32_t>(format));
			output1.Write(static_cast<int32_t>(width));
			output1.Write(static_cast<int32_t>(height));
			output1.Write(static_cast<int32_t>(mipLevels));
		}	

		inline std::string GetRuntimeReader(TargetPlatform targetPlatform) override {
			return "Texture2DReader";
		}
	};
}

#endif