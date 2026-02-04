#include "Xna/Framework/Graphics/DynamicVertexBuffer.hpp"
#include "Xna/Framework/Graphics/GraphicsDevice.hpp"

namespace Xna {
	DynamicVertexBuffer::DynamicVertexBuffer(GraphicsDevice const& graphicsDevice, Xna::VertexDeclaration const& vertexDeclaration, size_t vertexCount, Xna::BufferUsage usage)
	: VertexBuffer() {		
		usage = Xna::BufferUsage::WriteOnly;
		VertexBuffer::usage(usage);
		VertexBuffer::vertexCount(vertexCount);
		VertexBuffer::vertexDeclaration(vertexDeclaration);
		VertexBuffer::size(vertexDeclaration.VertexStride() * vertexCount);
		Bind(graphicsDevice);

		Platform::VertexBuffer_Create(*this);
	}
}