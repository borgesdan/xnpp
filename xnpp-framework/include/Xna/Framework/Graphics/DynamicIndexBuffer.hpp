#ifndef XNA_FRAMEWORK_GRAPHICS_DYNAMICINDEXBUFFER_HPP
#define XNA_FRAMEWORK_GRAPHICS_DYNAMICINDEXBUFFER_HPP

#include "IndexBuffer.hpp"
#include "GraphicsDevice.hpp"
#include "Shared.hpp"
#include <vector>
#include <memory>

namespace Xna {
	//Describes the rendering order of the vertices in a vertex buffer. 
	//Use DynamicIndexBuffer for storing indices for dynamic vertices and IndexBuffer for indices of non-dynamic arrays.
	class DynamicIndexBuffer final : public IndexBuffer {
	public:
		//Initializes a new instance of DynamicIndexBuffer.
		//TODO: [!] not implemented.
		DynamicIndexBuffer(GraphicsDevice& graphicsDevice, size_t sizeOfIndexType,
			size_t indexCount, Xna::BufferUsage usage){ }

		//Initializes a new instance of DynamicIndexBuffer.
		//TODO: [!] not implemented.
		DynamicIndexBuffer(GraphicsDevice& graphicsDevice, Xna::IndexElementSize indexElementSize,
			size_t indexCount, Xna::BufferUsage usage){ }

		~DynamicIndexBuffer() override = default;

		//Overloaded. Copies array data to the index buffer.
		//TODO: [!] not implemented.
		template <typename T>
		void SetData(std::vector<T> const& data, size_t startIndex, size_t elementCOunt,
			SetDataOptions options){ }

		//Overloaded. Copies array data to the index buffer.
		//TODO: [!] not implemented.
		template <typename T>
		void SetData(size_t offsetInBytes, std::vector<T> const& data, size_t startIndex,
			size_t elementCount, SetDataOptions options){ }

		//Determines if the index buffer data has been lost due to a lost device event.
		//TODO: [!] not implemented.
		bool IsContentLost() { return false; }

		inline DynamicIndexBuffer(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(DynamicIndexBuffer const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		struct Implementation;
		std::shared_ptr<Implementation> impl;
	};
}

#endif