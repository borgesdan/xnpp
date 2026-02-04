#ifndef XNA_FRAMEWORK_GRAPHICS_TEXTURE_HPP
#define XNA_FRAMEWORK_GRAPHICS_TEXTURE_HPP

#include "GraphicsResource.hpp"
#include "Shared.hpp"
#include <cstdint>

namespace Xna {
	//Represents a texture resource. 
	class Texture : public GraphicsResource {
	public:
		Texture() : GraphicsResource() {}

		virtual ~Texture() = default;

		//Gets the format of the texture data.
		virtual SurfaceFormat Format() const { return SurfaceFormat::Color; }
		//Gets the number of texture levels in a multilevel texture. 
		virtual int32_t LevelCount() const { return 0; }
	};
}

#endif