#ifndef XNA_FRAMEWORK_GRAPHICS_BUFFERS_HPP
#define XNA_FRAMEWORK_GRAPHICS_BUFFERS_HPP

#include <memory>
#include <vector>
#include "GraphicsDevice.hpp"
#include "Shared.hpp"
#include "Xna/Internal/Macros.hpp"
#include "Xna/Platform/Platform.hpp"
#include <Xna/Framework/Graphics/VertexElement.hpp>
#include <Xna/Framework/Graphics/VertexDeclaration.hpp>

namespace Xna {
	//Describes the rendering order of the vertices in a vertex buffer.
	class IndexBuffer {
	public:
		//Initializes a new instance of this class.
		inline IndexBuffer(GraphicsDevice& device, size_t sizeOfIndexType, size_t indexCount, Xna::BufferUsage usage) {
			m_backend = PlatformNS::IIndexBuffer::Create();
			m_backend->Init(device, sizeOfIndexType, indexCount, usage);
		}

		//Initializes a new instance of this class.
		inline IndexBuffer(GraphicsDevice& device, Xna::IndexElementSize indexElementSize, size_t indexCount, Xna::BufferUsage usage) {
			m_backend = PlatformNS::IIndexBuffer::Create();
			m_backend->Init(device, indexElementSize == Xna::IndexElementSize::SixteenBits ? 2 : 4, indexCount, usage);
		}

		virtual ~IndexBuffer() = default;

		//Gets the state of the related BufferUsage enumeration.		
		inline Xna::BufferUsage BufferUsage() const {
			return m_backend->GetStats().Usage;
		}

		//Gets a value indicating the size of this index element.		
		inline Xna::IndexElementSize IndexElementSize() const {
			return m_backend->GetStats().IndexElementSize == 2 ? Xna::IndexElementSize::SixteenBits : Xna::IndexElementSize::ThirtyTwoBits;
		}

		//Gets the number of indices in this buffer.		
		inline size_t IndexCount() const { 
			return m_backend->GetStats().IndexCount; 
		}

		//Copies array data to the index buffer.		
		template<typename T> void SetData(std::vector<T> const& data) {
			const auto length = data.size();
			SetData(0, data, 0, length);
		}

		//Copies array data to the index buffer.		
		template<typename T> void SetData(std::vector<T> const& data, size_t startIndex, size_t elementCount) {			
			SetData(0, data, startIndex, elementCount);
		}

		//Copies array data to the index buffer.		
		template<typename T> void SetData(size_t offsetInBytes, std::vector<T> const& data, size_t startIndex, size_t elementCount) {
			static_assert(std::is_integral<T>::value, "Only integral type.");
			m_backend->SetData(offsetInBytes, data.data(), startIndex, elementCount, sizeof(T), SetDataOptions::None);
		}

		//Gets the index buffer into an array.		
		template<typename T> void GetData(std::vector<T>& data) {			
			GetData(0, data, 0, data.size());
		}
		//Gets the index buffer into an array.		
		template<typename T> void GetData(std::vector<T>& data, size_t startIndex, size_t elementCount) {			
			GetData(0, data, startIndex, elementCount);
		}
		
		//Gets the index buffer into an array.		
		template<typename T> void GetData(size_t offsetInBytes, std::vector<T> const& data, size_t startIndex, size_t elementCount) {
			static_assert(std::is_integral<T>::value, "Only integral type.");
			m_backend->GetData(offsetInBytes, data.data(), 0, elementCount, sizeof(T));
		}

		XNPP_DECLARE_NULL_IMPL_WRAPPER(IndexBuffer, m_backend);
	protected:
		std::shared_ptr<PlatformNS::IIndexBuffer> m_backend;	
	};

	//Describes the rendering order of the vertices in a vertex buffer. 
	//Use DynamicIndexBuffer for storing indices for dynamic vertices and IndexBuffer for indices of non-dynamic arrays.
	class DynamicIndexBuffer final : public IndexBuffer {
	public:
		//Initializes a new instance of DynamicIndexBuffer.		
		inline DynamicIndexBuffer(GraphicsDevice& graphicsDevice, size_t sizeOfIndexType, size_t indexCount, Xna::BufferUsage usage) : IndexBuffer(nullptr)  {
			m_backend = PlatformNS::IIndexBuffer::CreateDynamic();
			m_backend->Init(graphicsDevice, sizeOfIndexType, indexCount, usage);
		}

		//Initializes a new instance of DynamicIndexBuffer.		
		inline DynamicIndexBuffer(GraphicsDevice& graphicsDevice, Xna::IndexElementSize indexElementSize,	size_t indexCount, Xna::BufferUsage usage) : IndexBuffer(nullptr) {
			m_backend = PlatformNS::IIndexBuffer::CreateDynamic();
			m_backend->Init(graphicsDevice, indexElementSize == Xna::IndexElementSize::SixteenBits ? 2 : 4, indexCount, usage);
		}

		~DynamicIndexBuffer() override = default;

		//Overloaded. Copies array data to the index buffer.		
		template <typename T> void SetData(std::vector<T> const& data, size_t startIndex, size_t elementCount, SetDataOptions options) {
			SetData(0, data.data(), startIndex, elementCount, options);
		}

		//Overloaded. Copies array data to the index buffer.		
		template <typename T> void SetData(size_t offsetInBytes, std::vector<T> const& data, size_t startIndex, size_t elementCount, SetDataOptions options) {
			static_assert(std::is_integral<T>::value, "Apenas tipos inteiros");
			m_backend->SetData(offsetInBytes, data.data(), startIndex, elementCount, sizeof(T), options);
		}

		//Determines if the index buffer data has been lost due to a lost device event.		
		bool IsContentLost() { return false; }

		XNPP_DECLARE_NULL_IMPL_BASE_WRAPPER(DynamicIndexBuffer, IndexBuffer, m_backend);
	};

	//Represents a list of 3D vertices to be streamed to the graphics device.
	class VertexBuffer {
	public:
		//Initializes a new instance of the VertexBuffer class.
		inline VertexBuffer(Xna::GraphicsDevice const& graphicsDevice, Xna::VertexDeclaration const& vertexDeclaration,	size_t vertexCount,	Xna::BufferUsage usage) {
			m_backend = PlatformNS::IVertexBuffer::Create();
			m_backend->Init(graphicsDevice, vertexDeclaration, vertexCount, usage);
		}		

		virtual ~VertexBuffer() = default;

		//Sets the vertex buffer data.
		template <typename T>
		void SetData(std::vector<T> const& data) { SetData(data, 0, data.size()); }

		//Sets the vertex buffer data.
		template <typename T>
		void SetData(std::vector<T> const& data, size_t startIndex, size_t elementCount) {
			SetData(0, data, startIndex, elementCount, m_backend->vertexDeclaration.VertexStride());
		}

		//Sets the vertex buffer data.
		template <typename T>
		void SetData(size_t offsetInBytes, std::vector<T> const& data, size_t startIndex, size_t elementCount, size_t vertexStride) {			
			assert(vertexStride == m_backend->vertexDeclaration.VertexStride() && "Vertex stride mismatch");

			const auto dataSize = elementCount * sizeof(T);		
			
			m_backend->SetData(offsetInBytes, data.data(), 0, elementCount, sizeof(T), SetDataOptions::None);
		}

		//Returns a copy of the vertex buffer data.
		template <typename T>
		void GetData(std::vector<T>& data) {
			GetData(data, 0, data.size());
		}

		//Returns a copy of the vertex buffer data.
		template <typename T>
		void GetData(std::vector<T>& data, size_t startIndex, size_t elementCount) {
			GetData(0, data, startIndex, elementCount, m_backend->vertexDeclaration.VertexStride());
		}

		//Returns a copy of the vertex buffer data.
		template <typename T>
		void GetData(size_t offsetInBytes, std::vector<T>& data, size_t startIndex, size_t elementCount, size_t vertexStride) {
			assert(vertexStride == m_backend->vertexDeclaration.VertexStride() && "Vertex stride mismatch");						
			m_backend->GetData(offsetInBytes, data.data(), startIndex, elementCount, vertexStride);
		}

		//Gets the state of the related BufferUsage enumeration.
		inline Xna::BufferUsage BufferUsage() const { return m_backend->GetStats().Usage; }
		//Gets the number of vertices.
		inline size_t VertexCount() const { return m_backend->GetStats().VertexCount; }
		// 	Defines per-vertex data in a buffer.
		inline Xna::VertexDeclaration VertexDeclaration() const { return m_backend->GetStats().VertexDeclaration; }

		XNPP_DECLARE_NULL_IMPL_WRAPPER(VertexBuffer, m_backend);

	protected:
		std::shared_ptr<PlatformNS::IVertexBuffer> m_backend;
	};

	//Represents a list of 3D vertices to be streamed to the graphics device. 
	//Use DynamicVertexBuffer for dynamic vertex arrays and VertexBuffer for non-dynamic vertex arrays.
	class DynamicVertexBuffer final : public VertexBuffer {
	public:
		//Initializes a new instance of DynamicVertexBuffer with the specified parameters.		
		XNPP_API DynamicVertexBuffer(GraphicsDevice const& graphicsDevice, Xna::VertexDeclaration const& vertexDeclaration, size_t vertexCount, Xna::BufferUsage usage) : VertexBuffer(nullptr) {
			m_backend = PlatformNS::IVertexBuffer::CreateDynamic();
			m_backend->Init(graphicsDevice, vertexDeclaration, vertexCount, usage);
		}

		virtual ~DynamicVertexBuffer() override {}

		//Determines if the index buffer data has been lost due to a lost device event.		
		inline bool IsContentLost() { return false; }

		//Copies array data to the vertex buffer.
		template <typename T>
		void SetData(std::vector<T> const& data, size_t startIndex, size_t elementCount, SetDataOptions options) {
			SetData(0, data, startIndex, elementCount, m_backend->vertexDeclaration.VertexStride(), options);
		}
		//Copies array data to the vertex buffer.
		template <typename T>
		void SetData(size_t offsetInBytes, std::vector<T>const& data, size_t startIndex, size_t elementCount, size_t vertexStride, SetDataOptions options) {
			m_backend->SetData(offsetInBytes, data, startIndex, elementCount, vertexStride, options);
		}

		XNPP_DECLARE_NULL_IMPL_BASE_WRAPPER(DynamicVertexBuffer, VertexBuffer, m_backend);
	};

	//Binding structure that specifies a vertex buffer and other per-vertex parameters (such as offset and instancing) for a graphics device.
	struct VertexBufferBinding {
		//Creates an instance of this object.
		constexpr VertexBufferBinding() = default;
		//Creates an instance of this object.
		XNPP_API VertexBufferBinding(Xna::VertexBuffer const& vertexBuffer, size_t vertexOffset, size_t instanceFrequency);
		//Creates an instance of this object.
		XNPP_API VertexBufferBinding(Xna::VertexBuffer const& vertexBuffer, size_t vertexOffset);
		//Creates an instance of this object.
		inline VertexBufferBinding(Xna::VertexBuffer const& vertexBuffer)
			: vertexBuffer(vertexBuffer) {
		}

		//Gets the offset between the beginning of the buffer and the vertex data to use.
		inline Xna::VertexBuffer VertexBuffer() const noexcept { return vertexBuffer; }
		//Gets a vertex buffer.
		inline size_t VertexOffset() const noexcept { return vertexOffset; }
		//Gets the instancing frequency.
		inline size_t InstanceFrequency() const noexcept { return instanceFrequency; }

	private:
		Xna::VertexBuffer vertexBuffer{ nullptr };
		size_t vertexOffset{ 0 };
		size_t instanceFrequency{ 0 };
	};
}

#endif