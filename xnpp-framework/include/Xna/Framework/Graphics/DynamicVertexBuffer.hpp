#ifndef XNA_FRAMEWORK_GRAPHICS_DYNAMICINDEXBUFFER_HPP
#define XNA_FRAMEWORK_GRAPHICS_DYNAMICINDEXBUFFER_HPP

#include <memory>
#include <vector>
#include "Shared.hpp"
#include "GraphicsResource.hpp"
#include "Xna/Platform/Platform.hpp"
#include "VertexBuffer.hpp"

namespace Xna {
	//Represents a list of 3D vertices to be streamed to the graphics device. 
	//Use DynamicVertexBuffer for dynamic vertex arrays and VertexBuffer for non-dynamic vertex arrays.
	class DynamicVertexBuffer final : public VertexBuffer {
	public:	
		//Initializes a new instance of DynamicVertexBuffer with the specified parameters.		
		DynamicVertexBuffer(GraphicsDevice const& graphicsDevice, Xna::VertexDeclaration const& vertexDeclaration, size_t vertexCount, Xna::BufferUsage usage);

		~DynamicVertexBuffer() override {}

		//Determines if the index buffer data has been lost due to a lost device event.		
		inline bool IsContentLost() { return false; }		

		//Copies array data to the vertex buffer.
		template <typename T>
		void SetData(std::vector<T> const& data, size_t startIndex, size_t elementCount, SetDataOptions options) {
			SetData(0, data, startIndex, elementCount, impl->vertexDeclaration.VertexStride(), options);
		}
		//Copies array data to the vertex buffer.
		template <typename T>
		void SetData(size_t offsetInBytes, std::vector<T>const& data, size_t startIndex, size_t elementCount, size_t vertexStride, SetDataOptions options) {
			Platform::DynamicVertexBuffer_SetData(*this, offsetInBytes, data.data(), sizeof(T), startIndex, elementCount, vertexStride, options);
		}

		inline DynamicVertexBuffer(std::nullptr_t) : VertexBuffer(nullptr) { }
		inline bool operator==(DynamicVertexBuffer const& other) const noexcept { return GetImpl() == other.GetImpl(); }
		inline bool operator==(std::nullptr_t) const noexcept { return GetImpl() == nullptr; }
		inline explicit operator bool() const noexcept { return GetImpl() != nullptr; }	
	};
}

#endif