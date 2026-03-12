#ifndef XNA_FRAMEWORK_GRAPHICS_VERTEXBUFFER_HPP
#define XNA_FRAMEWORK_GRAPHICS_VERTEXBUFFER_HPP

#include "VertexDeclaration.hpp"
#include "Xna/CSharp/Type.hpp"
#include "Xna/CSharp/Exception.hpp"
#include "Xna/Framework/Graphics/GraphicsResource.hpp"
#include "Xna/Platform/Platform.hpp"
#include <memory>
#include <vector>
#include "Xna/Internal/Macros.hpp"

namespace Xna {
	//Represents a list of 3D vertices to be streamed to the graphics device.
	class VertexBuffer {
	public:
		//Initializes a new instance of the VertexBuffer class.
		XNPP_API VertexBuffer(
			Xna::GraphicsDevice const& graphicsDevice,
			Xna::VertexDeclaration const& vertexDeclaration,
			size_t vertexCount,
			Xna::BufferUsage usage);		

		inline const Xna::GraphicsDevice& GraphicsDevice() const;

		//Sets the vertex buffer data.
		template <typename T>
		void SetData(std::vector<T> const& data) {
			SetData(data, 0, data.size());
		}

		//Sets the vertex buffer data.
		template <typename T>
		void SetData(std::vector<T> const& data, size_t startIndex, size_t elementCount) {
			SetData(0, data, startIndex, elementCount, impl->vertexDeclaration.VertexStride());
		}
		
		//Sets the vertex buffer data.
		template <typename T>
		void SetData(size_t offsetInBytes, std::vector<T> const& data, size_t startIndex, size_t elementCount, size_t vertexStride) {
			if (vertexStride != impl->vertexDeclaration.VertexStride())
				throw CSharp::InvalidOperationException("Vertex stride mismatch");

			const auto dataSize = elementCount * sizeof(T);

			if (offsetInBytes + dataSize > impl->size)
				throw CSharp::ArgumentOutOfRangeException("SetData out of bounds");

			//Platform::VertexBuffer_SetData(*this, offsetInBytes, data.data(), sizeof(T), startIndex, elementCount, vertexStride);
		}

		//Returns a copy of the vertex buffer data.
		template <typename T>
		void GetData(std::vector<T>& data) {
			GetData(data, 0, data.size());
		}

		//Returns a copy of the vertex buffer data.
		template <typename T>
		void GetData(std::vector<T>& data, size_t startIndex, size_t elementCount) {
			GetData(0, data, startIndex, elementCount, impl->vertexDeclaration.VertexStride());
		}

		//Returns a copy of the vertex buffer data.
		template <typename T>
		void GetData(size_t offsetInBytes, std::vector<T>& data, size_t startIndex,
			size_t elementCount, size_t vertexStride) {

			if (vertexStride != impl->vertexDeclaration.VertexStride())
				throw CSharp::InvalidOperationException("Vertex stride mismatch");

			const auto dataSize = elementCount * sizeof(T);

			if (offsetInBytes + dataSize > impl->size)
				throw CSharp::ArgumentOutOfRangeException("SetData out of bounds");

			//Platform::VertexBuffer_GetData(*this, offsetInBytes, data.data(), sizeof(T), startIndex, elementCount, vertexStride);
		}

		//Gets the state of the related BufferUsage enumeration.
		inline Xna::BufferUsage BufferUsage() const { return impl->usage; }
		//Gets the number of vertices.
		inline size_t VertexCount() const { return impl->vertexCount; }		
		// 	Defines per-vertex data in a buffer.
		inline Xna::VertexDeclaration VertexDeclaration() const { return impl->vertexDeclaration; }

		inline VertexBuffer(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(VertexBuffer const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	protected:
		inline VertexBuffer() {
			impl = std::make_shared<Implementation>();
		}

	private:
		struct Implementation {
			Xna::BufferUsage usage;
			size_t size;
			size_t vertexCount;
			Xna::VertexDeclaration vertexDeclaration;
			Xna::GraphicsDevice device;
		};

		std::shared_ptr<Implementation> impl;

		friend struct VertexBufferBinding;
		friend struct Platform;

	protected:
		inline Xna::BufferUsage usage() { return impl->usage; }
		inline void usage(Xna::BufferUsage value) { impl->usage = value; }
		inline size_t size() { return impl->size; }
		inline void size(size_t value) { impl->size = value; }
		inline size_t vertexCount() { return impl->vertexCount; }
		inline void vertexCount(size_t value) { impl->vertexCount = value; }
		inline Xna::VertexDeclaration vertexDeclaration() { return impl->vertexDeclaration; }
		inline void vertexDeclaration(Xna::VertexDeclaration const& value) { impl->vertexDeclaration = value; }

		const std::shared_ptr<Implementation>& GetImpl() const { return impl; }
	};

	//Represents a list of 3D vertices to be streamed to the graphics device. 
	//Use DynamicVertexBuffer for dynamic vertex arrays and VertexBuffer for non-dynamic vertex arrays.
	class DynamicVertexBuffer final : public VertexBuffer {
	public:
		//Initializes a new instance of DynamicVertexBuffer with the specified parameters.		
		XNPP_API DynamicVertexBuffer(GraphicsDevice const& graphicsDevice, Xna::VertexDeclaration const& vertexDeclaration, size_t vertexCount, Xna::BufferUsage usage);

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

		inline DynamicVertexBuffer(std::nullptr_t) : VertexBuffer(nullptr) {}
		inline bool operator==(DynamicVertexBuffer const& other) const noexcept { return GetImpl() == other.GetImpl(); }
		inline bool operator==(std::nullptr_t) const noexcept { return GetImpl() == nullptr; }
		inline explicit operator bool() const noexcept { return GetImpl() != nullptr; }
	};
}

#endif