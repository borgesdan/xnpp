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

		void Init(GraphicsDevice& device, size_t sizeOfIndexType, size_t indexCount, Xna::BufferUsage usage) override {
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


		void SetData(size_t offsetInBytes, const void* data, size_t startIndex, size_t elementCount, size_t elementSize, SetDataOptions setDataOptions) override {
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

	struct BgfxDynamicIndexBuffer final : PlatformNS::IIndexBuffer {
		bgfx::DynamicIndexBufferHandle m_handle{ BGFX_INVALID_HANDLE };

		size_t m_sizeOfIndexType{ 0 };
		size_t m_indexCount{ 0 };
		Xna::BufferUsage m_usage{ BufferUsage::None };

		bool m_enableShadow{ false };
		std::vector<uint8_t> m_shadowData;
		uint16_t m_flags{ BGFX_BUFFER_NONE };

		void Init(GraphicsDevice& device, size_t sizeOfIndexType, size_t indexCount, Xna::BufferUsage usage) override {
			assert((sizeOfIndexType == 2 || sizeOfIndexType == 4) && "Invalid size of type.");

			if (usage == Xna::BufferUsage::WriteOnly)
				m_flags |= BGFX_BUFFER_COMPUTE_WRITE;

			if (sizeOfIndexType == 4)
				m_flags |= BGFX_BUFFER_INDEX32;

			m_handle = bgfx::createDynamicIndexBuffer(
				static_cast<uint32_t>(indexCount),
				m_flags
			);

			if (m_enableShadow)
				m_shadowData.resize(sizeOfIndexType * indexCount);

			m_sizeOfIndexType = sizeOfIndexType;
			m_indexCount = indexCount;
			m_usage = usage;
		}

		void SetData(size_t offsetInBytes, const void* data, size_t startIndex, size_t elementCount, size_t elementSize, SetDataOptions setDataOptions) override {
			const size_t elementSize = sizeof(T);
			const size_t copySize = elementSize * elementCount;

			assert(offsetInBytes + copySize <= m_sizeOfIndexType * m_indexCount && "offsetInBytes + copySize > m_sizeOfIndexType * m_indexCount");

			// Atualiza shadow (se existir)
			if (m_enableShadow) {
				std::memcpy(
					m_shadowData.data() + offsetInBytes,
					data.data() + startIndex,
					copySize
				);
			}

			// DISCARD = recria buffer (evita stall)
			if (setDataOptions == SetDataOptions::Discard) {
				bgfx::destroy(m_handle);

				m_handle = bgfx::createDynamicIndexBuffer(
					static_cast<uint32_t>(m_indexCount),
					m_flags
				);
			}

			const bgfx::Memory* mem = bgfx::copy(
				data.data() + startIndex,
				static_cast<uint32_t>(copySize)
			);

			bgfx::update(
				m_handle,
				static_cast<uint32_t>(offsetInBytes / m_sizeOfIndexType),
				mem
			);
		}

		void GetData(size_t offsetInBytes, void* data, size_t startIndex, size_t elementCount, size_t elementSize) override {
			assert(m_enableShadow && "GetData requer shadow buffer");

			const size_t elementSize = sizeof(T);
			const size_t copySize = elementSize * elementCount;

			assert(offsetInBytes + copySize <= m_shadowData.size() && "offsetInBytes + copySize > m_shadowData.size()");
			assert(startIndex + elementCount <= data.size() && "startIndex + elementCount > data.size()");

			std::memcpy(
				data.data() + startIndex,
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

		~BgfxDynamicIndexBuffer() override {
			if (bgfx::isValid(m_handle)) bgfx::destroy(m_handle);
		}
	};

	std::unique_ptr<PlatformNS::IIndexBuffer> PlatformNS::IIndexBuffer::Create() {
		return std::make_unique<BgfxIndexBuffer>();
	}

	std::unique_ptr<PlatformNS::IIndexBuffer>PlatformNS::IIndexBuffer::CreateDynamic() {
		return std::make_unique<BgfxDynamicIndexBuffer>();
	}
}