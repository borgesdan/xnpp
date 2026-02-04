#ifndef XNA_FRAMEWORK_GRAPHICS_IVERTEXTYPE_HPP
#define XNA_FRAMEWORK_GRAPHICS_IVERTEXTYPE_HPP

#include "VertexDeclaration.hpp"

namespace Xna {
	//Vertex type interface which is implemented by a custom vertex type structure.
	struct IVertexType {
		virtual ~IVertexType() = default;
		//Vertex declaration, which defines per-vertex data.
		virtual VertexDeclaration VertexDeclaration() const = 0;
	};
}

#endif