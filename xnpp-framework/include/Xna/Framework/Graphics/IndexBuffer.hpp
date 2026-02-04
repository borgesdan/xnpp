#ifndef XNA_FRAMEWORK_GRAPHICS_INDEXBUFFER_HPP
#define XNA_FRAMEWORK_GRAPHICS_INDEXBUFFER_HPP

#include <memory>
#include <vector>
#include "GraphicsDevice.hpp"
#include "Shared.hpp"

namespace Xna {
	//Describes the rendering order of the vertices in a vertex buffer.
	class IndexBuffer {
	public:
		//Initializes a new instance of this class.
		//TODO: [!] not implemented
		IndexBuffer(GraphicsDevice& device, size_t sizeOfIndexType, size_t indexCount, Xna::BufferUsage usage);
		//Initializes a new instance of this class.
		//TODO: [!] not implemented
		IndexBuffer(GraphicsDevice& device, Xna::IndexElementSize indexElementSize, size_t indexCount, Xna::BufferUsage usage);

		virtual ~IndexBuffer() = default;

		//Gets the state of the related BufferUsage enumeration.
		//TODO: [!] not implemented
		Xna::BufferUsage BufferUsage() const;		
		//Gets a value indicating the size of this index element.
		//TODO: [!] not implemented
		Xna::IndexElementSize IndexElementSize() const;
		//Gets the number of indices in this buffer.
		//TODO: [!] not implemented
		size_t IndexCount() const;

		//Copies array data to the index buffer.
		//TODO: [!] not implemented
		template<typename T> void SetData(std::vector<T> const& data);
		//Copies array data to the index buffer.
		//TODO: [!] not implemented
		template<typename T> void SetData(std::vector<T> const& data, size_t startIndex, size_t elementCount);
		//Copies array data to the index buffer.
		//TODO: [!] not implemented
		template<typename T> void SetData(size_t offsetInBytes, std::vector<T> const& data, size_t startIndex, size_t elementCount);

		//Gets the index buffer into an array.
		//TODO: [!] not implemented
		template<typename T> void GetData(std::vector<T>& data);
		//Gets the index buffer into an array.
		//TODO: [!] not implemented
		template<typename T> void GetData(std::vector<T>& data, size_t startIndex, size_t elementCount);
		//Gets the index buffer into an array.
		//TODO: [!] not implemented
		template<typename T> void GetData(size_t offsetInBytes, std::vector<T> const& data, size_t startIndex, size_t elementCount);

		inline IndexBuffer(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(IndexBuffer const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		struct Implementation;
		std::shared_ptr<Implementation> impl;	

	protected:
		IndexBuffer();
	};
}

#endif