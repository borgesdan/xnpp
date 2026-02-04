#include "Xna/Framework/Graphics/VertexBuffer.hpp"
#include "Xna/Framework/Graphics/GraphicsDevice.hpp"

namespace Xna {
	VertexBuffer::VertexBuffer(
		GraphicsDevice const& graphicsDevice,
		Xna::VertexDeclaration const& vertexDeclaration,
		size_t vertexCount,
		Xna::BufferUsage usage){

		impl = std::make_shared<Implementation>();
		impl->usage = usage;
		impl->vertexCount = vertexCount;
		impl->vertexDeclaration = vertexDeclaration;
		impl->size = vertexDeclaration.VertexStride() * vertexCount;
		Bind(graphicsDevice);

		Platform::VertexBuffer_Create(*this);
	}
}