#include "Xna/Platform/Platform.hpp"
#include "Xna/Framework/Graphics/GraphicsDevice.hpp"
#include <bgfx/bgfx.h>

namespace Xna {
	struct BgfxIndexBuffer final : public PlatformNS::IIndexBuffer {
		bgfx::IndexBufferHandle m_handle{ BGFX_INVALID_HANDLE };
		size_t m_sizeOfIndexType;
		size_t m_indexCount;
		std::vector<uint8_t> m_shadowData; // ESSENCIAL para GetData
		uint16_t flags{ BGFX_BUFFER_NONE };

		void Init(GraphicsDevice& device, size_t sizeOfIndexType, size_t indexCount, Xna::BufferUsage usage) override{
			assert((sizeOfIndexType == 2 || sizeOfIndexType == 4) && "Invalid size of type.");			

			const uint32_t totalSize = static_cast<uint32_t>(sizeOfIndexType * indexCount);

			// Shadow copy (necessário para GetData)
			m_shadowData.resize(totalSize);

			const bgfx::Memory* mem = bgfx::copy(m_shadowData.data(), totalSize);

			if (usage == Xna::BufferUsage::WriteOnly)
				flags |= BGFX_BUFFER_COMPUTE_WRITE;

			if (sizeOfIndexType == 4)
				flags |= BGFX_BUFFER_INDEX32;	

			m_sizeOfIndexType = sizeOfIndexType;
			m_indexCount = indexCount;
		}


		void SetData(size_t offsetInBytes, const void* data, size_t startIndex, size_t elementCount, size_t elementSize) override {				
			assert(data != nullptr && "Data is null.");

			const size_t copySize = elementSize * elementCount;
			assert(offsetInBytes + copySize <= m_shadowData.size() && "SetData: overflow");

			const auto data1 = reinterpret_cast<const uint8_t*>(data);

			// Copia para shadow buffer
			std::memcpy(
				m_shadowData.data() + offsetInBytes,
				data1 + startIndex,
				copySize
			);

			// Atualiza GPU
			const bgfx::Memory* mem = bgfx::copy(
				m_shadowData.data() + offsetInBytes,
				static_cast<uint32_t>(copySize)
			);

			if (isValid(m_handle))
				bgfx::destroy(m_handle);

			m_handle = bgfx::createIndexBuffer(mem, flags);
		}

		void GetData(size_t offsetInBytes, void* data, size_t startIndex, size_t elementCount, size_t elementSize) override {
			const size_t copySize = elementSize * elementCount;

			assert(offsetInBytes + copySize <= m_shadowData.size() && "GetData: overflow.");
			assert(startIndex + elementCount <= data.size() && "startIndex + elementCount > data.size()");

			// Lê do shadow buffer (bgfx não permite leitura direta da GPU)
			auto data1 = reinterpret_cast<uint8_t*>(data);

			std::memcpy(
				data1 + startIndex,
				m_shadowData.data() + offsetInBytes,
				copySize
			);
		}

		PlatformNS::BufferStats GetStats() override {
			PlatformNS::BufferStats stats{};
			stats.Usage = (flags & static_cast<int>(BGFX_BUFFER_INDEX32)) == static_cast<int>(BGFX_BUFFER_INDEX32) ? BufferUsage::WriteOnly : BufferUsage::None;
			stats.IndexCount = m_indexCount;
			stats.IndexElementSize = m_sizeOfIndexType;

			return stats;
		}

		~BgfxIndexBuffer() override {
			if (bgfx::isValid(m_handle)) bgfx::destroy(m_handle);
		}
	};

	std::unique_ptr<PlatformNS::IIndexBuffer> PlatformNS::IIndexBuffer::Create() {
		return std::make_unique<BgfxIndexBuffer>();
	}
}