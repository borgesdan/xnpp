#ifndef XNA_CONTENTPIPELINE_GRAPHICS_MIPMAPCHAIN_HPP
#define XNA_CONTENTPIPELINE_GRAPHICS_MIPMAPCHAIN_HPP

#include <Xna/CSharp/Collection.hpp>
#include "BitmapContent.hpp"

namespace Xna {
	class MipmapChain : public CSharp::Collection<std::shared_ptr<BitmapContent>>{
	public:
		inline MipmapChain(): CSharp::Collection<std::shared_ptr<BitmapContent>>() {}

		inline MipmapChain(std::shared_ptr<BitmapContent> const& bitmap) : CSharp::Collection<std::shared_ptr<BitmapContent>>() {
			Add(bitmap);
		}	
	};
}

#endif