#include "Xna/Framework/Graphics/VertexBufferBinding.hpp"
#include "Xna/CSharp/Exception.hpp"

namespace Xna {
	VertexBufferBinding::VertexBufferBinding(Xna::VertexBuffer const& vertexBuffer, size_t vertexOffset, size_t instanceFrequency) 
	: vertexBuffer(vertexBuffer), vertexOffset(vertexOffset), instanceFrequency(instanceFrequency) {
        if (vertexBuffer == nullptr)
            throw CSharp::ArgumentNullException("vertexBuffer");

        if (vertexOffset >= vertexBuffer.impl->vertexCount)
            throw CSharp::ArgumentOutOfRangeException("vertexOffset");
	}
	VertexBufferBinding::VertexBufferBinding(Xna::VertexBuffer const& vertexBuffer, size_t vertexOffset) 
	: VertexBufferBinding(vertexBuffer, vertexOffset, 0){}
	
}