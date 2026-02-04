#ifndef XNA_FRAMEWORK_GRAPHICS_VERTEXBUFFERBINDING_HPP
#define XNA_FRAMEWORK_GRAPHICS_VERTEXBUFFERBINDING_HPP

#include "VertexBuffer.hpp"

namespace Xna {
	//Binding structure that specifies a vertex buffer and other per-vertex parameters (such as offset and instancing) for a graphics device.
	struct VertexBufferBinding {
		//Creates an instance of this object.
		constexpr VertexBufferBinding() = default;
		//Creates an instance of this object.
		VertexBufferBinding(Xna::VertexBuffer const& vertexBuffer, size_t vertexOffset, size_t instanceFrequency);
		//Creates an instance of this object.
		VertexBufferBinding(Xna::VertexBuffer const& vertexBuffer, size_t vertexOffset);
		//Creates an instance of this object.
		inline VertexBufferBinding(Xna::VertexBuffer const& vertexBuffer) 
			: vertexBuffer(vertexBuffer) {}

		//Gets the offset between the beginning of the buffer and the vertex data to use.
		inline Xna::VertexBuffer VertexBuffer() const noexcept { return vertexBuffer; }
		//Gets a vertex buffer.
		inline size_t VertexOffset() const noexcept { return vertexOffset; }
		//Gets the instancing frequency.
		inline size_t InstanceFrequency() const noexcept { return instanceFrequency; }

	private:
		Xna::VertexBuffer vertexBuffer{nullptr};
		size_t vertexOffset{0};
		size_t instanceFrequency{0};
	};
}

#endif