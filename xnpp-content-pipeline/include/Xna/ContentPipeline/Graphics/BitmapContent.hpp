#ifndef XNA_CONTENTPIPELINE_GRAPHICS_BITMAPCONTENT_HPP
#define XNA_CONTENTPIPELINE_GRAPHICS_BITMAPCONTENT_HPP

#include "Xna/ContentPipeline/ContentItem.hpp"
#include <Xna/Framework/Graphics/Shared.hpp>
#include <vector>

namespace Xna {
	struct BitmapContent : public ContentItem {	
		virtual ~BitmapContent() = default;

		BitmapContent() = default;
		BitmapContent(size_t width, size_t height)
			: Width(width), Height(height) {}

		size_t Width{ 0 };
		size_t Height{ 0 };

		virtual std::vector<uint8_t> GetPixelData() const = 0;

		virtual void SetPixelData(const uint8_t* sourceData, size_t length) = 0;
		virtual bool TryGetFormat(SurfaceFormat& format) = 0;
	};
}

#endif