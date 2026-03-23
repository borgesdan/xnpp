#include "Xna/Platform/Platform.hpp"
#include "Xna/Framework/Graphics/GraphicsDevice.hpp"
#include "Xna/Framework/Graphics/Buffer.hpp"
#include <bgfx/bgfx.h>
#include <algorithm>


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
			assert(startIndex + elementCount <= elementCount && "startIndex + elementCount > data.size()");

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
			assert(data != nullptr && "data is null.");

			const size_t copySize = elementSize * elementCount;

			assert(offsetInBytes + copySize <= m_sizeOfIndexType * m_indexCount && "offsetInBytes + copySize > m_sizeOfIndexType * m_indexCount");

			const auto data1 = reinterpret_cast<const uint8_t*>(data);

			// Atualiza shadow (se existir)
			if (m_enableShadow) {
				std::memcpy(
					m_shadowData.data() + offsetInBytes,
					data1 + startIndex,
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
				data1 + startIndex,
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

			const size_t copySize = elementSize * elementCount;

			assert(offsetInBytes + copySize <= m_shadowData.size() && "offsetInBytes + copySize > m_shadowData.size()");
			assert(startIndex + elementCount <= elementCount && "startIndex + elementCount > data.size()");

			auto data1 = reinterpret_cast<uint8_t*>(data);

			std::memcpy(
				data1 + startIndex,
				m_shadowData.data() + offsetInBytes,
				copySize
			);
		}

		PlatformNS::BufferStats GetStats() override {
			PlatformNS::BufferStats stats{};
			stats.Usage = (m_flags & static_cast<int>(BGFX_BUFFER_INDEX32)) == static_cast<int>(BGFX_BUFFER_INDEX32) ? BufferUsage::WriteOnly : BufferUsage::None;
			stats.IndexCount = m_indexCount;
			stats.IndexElementSize = m_sizeOfIndexType;

			return stats;
		}

		~BgfxDynamicIndexBuffer() override {
			if (bgfx::isValid(m_handle)) bgfx::destroy(m_handle);
		}
	};

	static constexpr bgfx::Attrib::Enum ConvertVertexElementUsage(VertexElementUsage usage, uint32_t usageIndex = 0) {
			assert((usage != VertexElementUsage::Depth
				&& usage != VertexElementUsage::Fog
				&& usage != VertexElementUsage::PointSize
				&& usage != VertexElementUsage::Sample
				&& usage != VertexElementUsage::TessellateFactor)
				&& "VertexElementUsage not supported.");

		if(usage == VertexElementUsage::Color)
			assert(usageIndex <= 3 && "Invalid color usageIndex");

		if (usage == VertexElementUsage::TextureCoordinate)
			assert(usageIndex <= 7 && "Invalid TexCoord usageIndex");

		using namespace bgfx;

		switch (usage)
		{
		case VertexElementUsage::Position:
			return Attrib::Position;

		case VertexElementUsage::Normal:
			return Attrib::Normal;

		case VertexElementUsage::Tangent:
			return Attrib::Tangent;

		case VertexElementUsage::Binormal:
			return Attrib::Bitangent;

		case VertexElementUsage::Color:
			switch (usageIndex)
			{
			case 0: return Attrib::Color0;
			case 1: return Attrib::Color1;
			case 2: return Attrib::Color2;
			case 3: return Attrib::Color3;
			default: return Attrib::Color0;
			}

		case VertexElementUsage::TextureCoordinate:
			switch (usageIndex)
			{
			case 0: return Attrib::TexCoord0;
			case 1: return Attrib::TexCoord1;
			case 2: return Attrib::TexCoord2;
			case 3: return Attrib::TexCoord3;
			case 4: return Attrib::TexCoord4;
			case 5: return Attrib::TexCoord5;
			case 6: return Attrib::TexCoord6;
			case 7: return Attrib::TexCoord7;
			default: return Attrib::TexCoord0;
			}

		case VertexElementUsage::BlendIndices:
			return Attrib::Indices;

		case VertexElementUsage::BlendWeight:
			return Attrib::Weight;
		default:
			return Attrib::Position;
		}
	}

	struct AttribTypeInfo
	{
		bgfx::AttribType::Enum type;
		uint8_t numComponents;
		bool normalized;
	};

	static constexpr AttribTypeInfo ConvertVertexElementFormat(VertexElementFormat format)
	{
		using namespace bgfx;

		switch (format)
		{
		case VertexElementFormat::Single:
			return { AttribType::Float, 1, false };

		case VertexElementFormat::Vector2:
			return { AttribType::Float, 2, false };

		case VertexElementFormat::Vector3:
			return { AttribType::Float, 3, false };

		case VertexElementFormat::Vector4:
			return { AttribType::Float, 4, false };

		case VertexElementFormat::Color:
			// Uint8 normalizado (0–255 → 0–1)
			return { AttribType::Uint8, 4, true };

		case VertexElementFormat::Byte4:
			return { AttribType::Uint8, 4, false };

		case VertexElementFormat::Short2:
			return { AttribType::Int16, 2, false };

		case VertexElementFormat::Short4:
			return { AttribType::Int16, 4, false };

		case VertexElementFormat::NormalizedShort2:
			return { AttribType::Int16, 2, true };

		case VertexElementFormat::NormalizedShort4:
			return { AttribType::Int16, 4, true };

		case VertexElementFormat::HalfVector2:
			return { AttribType::Half, 2, false };

		case VertexElementFormat::HalfVector4:
			return { AttribType::Half, 4, false };

		default:
			return { AttribType::Float, 1, false };
		}
	}

	//XNA usa Offset manual, bgfx assume layout sequencial
	//A ordem dos elementos precisa estar correta
	//Não há suporte direto para gaps (padding manual)
	struct BgfxVertexBuffer final : public PlatformNS::IVertexBuffer {
		bgfx::VertexBufferHandle m_handle{ BGFX_INVALID_HANDLE };
		bgfx::VertexLayout m_layout{};

		void Init(Xna::GraphicsDevice const& graphicsDevice, Xna::VertexDeclaration const& vertexDeclaration, size_t vertexCount, Xna::BufferUsage usage) override {
			//Copiar e ordenar elementos por Offset
			auto elements = vertexDeclaration.GetVertexElements();
			std::sort(elements.begin(), elements.end(),
				[](const Xna::VertexElement& a, const Xna::VertexElement& b)
				{
					return a.Offset < b.Offset;
				});

			//Construir VertexLayout
			m_layout.begin();

			uint32_t currentOffset = 0;

			for (const auto& element : elements)
			{
				//Validar alinhamento
				assert(!(element.Offset < currentOffset) && "Invalid offset");

				//Converter usage → attrib
				bgfx::Attrib::Enum attrib =
					ConvertVertexElementUsage(
						element.VertexElementUsage,
						static_cast<uint8_t>(element.UsageIndex));

				//Converter format → tipo bgfx
				AttribTypeInfo info =
					ConvertVertexElementFormat(element.VertexElementFormat);

				//Adicionar ao layout
				m_layout.add(
					attrib,
					info.numComponents,
					info.type,
					info.normalized
				);

				//Atualizar offset lógico
				currentOffset = element.Offset; // bgfx não usa offset explícito
			}

			m_layout.end();			
		}

		void SetData(size_t offsetInBytes, const void* data, size_t startIndex, size_t elementCount, size_t vertexStride, size_t elementSize) override {
			assert(data == nullptr && "data is null.");
			assert(!(startIndex + elementCount > elementCount) && "Data out of bounds.");			

			const uint8_t* srcPtr =
				reinterpret_cast<const uint8_t*>(data) +
				(startIndex * elementSize);

			const size_t sizeInBytes = elementCount * elementSize;

			const bgfx::Memory* mem = bgfx::copy(srcPtr, sizeInBytes);

			if(bgfx::isValid(m_handle))
				bgfx::destroy(m_handle);

			m_handle = bgfx::createVertexBuffer(mem, m_layout);

			assert(bgfx::isValid(m_handle) && "bgfx::createVertexBuffer failed.");
		}

		void GetData(size_t offsetInBytes, void* data, size_t startIndex, size_t elementCount, size_t vertexStride, size_t elementSize) override {
			throw std::runtime_error("not supported");
		}

		~BgfxVertexBuffer() override {
			if (bgfx::isValid(m_handle)) bgfx::destroy(m_handle);
		}
	};

	std::unique_ptr<PlatformNS::IIndexBuffer> PlatformNS::IIndexBuffer::Create() {
		return std::make_unique<BgfxIndexBuffer>();
	}

	std::unique_ptr<PlatformNS::IIndexBuffer>PlatformNS::IIndexBuffer::CreateDynamic() {
		return std::make_unique<BgfxDynamicIndexBuffer>();
	}

	std::unique_ptr<PlatformNS::IVertexBuffer> PlatformNS::IVertexBuffer::Create() {
		return std::make_unique<BgfxVertexBuffer>();
	}

	std::unique_ptr<PlatformNS::IVertexBuffer> PlatformNS::IVertexBuffer::CreateDynamic() {
		return nullptr;
	}
}