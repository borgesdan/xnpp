#include "Xna/Platform/Platform.hpp"
#ifdef PLATFORM_WINDOWS
#include "Xna/Framework/Graphics/VertexBuffer.hpp"
#include "Xna/Framework/Graphics/GraphicsDevice.hpp"
#include "Xna/CSharp/Exception.hpp"

namespace Xna {
	void Platform::VertexBuffer_Create(VertexBuffer const b) {
		const auto& impl = b.impl;

		D3D11_BUFFER_DESC desc{};
		desc.ByteWidth = impl->size;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		if (impl->usage == BufferUsage::WriteOnly) {
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else {
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.CPUAccessFlags = 0;
		}

		auto& device = b.GetGraphicsDevice()->implGraphicsDevice->platformImpl.device;

		const auto hr = device->CreateBuffer(
			&desc,
			nullptr,
			impl->platformImpl.buffer.ReleaseAndGetAddressOf());

		if (FAILED(hr))
			throw CSharp::InvalidOperationException("CreateBuffer() failed.");
	}
	
	void Platform::VertexBuffer_SetData(VertexBuffer const& b, size_t offsetInBytes,
		const void* data, size_t sizeOfData, size_t startIndex, size_t elementCount, size_t vertexStride, std::optional<SetDataOptions> options) {

		auto& impl = b.impl;
		auto& device = b.GetGraphicsDevice()->implGraphicsDevice->platformImpl.device;
		auto& buffer = impl->platformImpl.buffer;
		const auto _data = reinterpret_cast<const uint8_t*>(data);
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
		device->GetImmediateContext(context.ReleaseAndGetAddressOf());

		const auto dataSize = elementCount * sizeOfData;

		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;

		if(options && options.value() == SetDataOptions::NoOverwrite)
			mapType = D3D11_MAP_WRITE_NO_OVERWRITE;
		

		if (impl->usage == BufferUsage::WriteOnly) {
			D3D11_MAPPED_SUBRESOURCE mapped{};

			const auto hr = context->Map(
				//_In_  ID3D11Resource *pResource,
				buffer.Get(),
				//_In_  UINT Subresource,
				0,
				//_In_  D3D11_MAP MapType,
				mapType,
				//_In_  UINT MapFlags,
				0,
				//_Out_opt_  D3D11_MAPPED_SUBRESOURCE *pMappedResource
				&mapped);

			if (FAILED(hr))
				throw std::runtime_error("Map failed");

			std::memcpy(
				static_cast<uint8_t*>(mapped.pData) + offsetInBytes,
				_data + startIndex,
				dataSize);

			context->Unmap(buffer.Get(), 0);
		}
		else {
			context->UpdateSubresource(
				//_In_  ID3D11Resource *pDstResource,
				buffer.Get(),
				//_In_  UINT DstSubresource,
				0,
				//_In_opt_  const D3D11_BOX *pDstBox,
				nullptr,
				//_In_  const void *pSrcData,
				_data + startIndex,
				// _In_  UINT SrcRowPitch,
				0,
				//_In_  UINT SrcDepthPitch)
				0);
		}
	}

	void Platform::VertexBuffer_GetData(VertexBuffer const& b,
		size_t offsetInBytes,
		void* data,
		size_t sizeOfData,
		size_t startIndex,
		size_t elementCount,
		size_t vertexStride) {

		auto& impl = b.impl;
		auto& device = b.GetGraphicsDevice()->implGraphicsDevice->platformImpl.device;
		auto& buffer = impl->platformImpl.buffer;
		D3D11_BUFFER_DESC bufferDesc;
		buffer->GetDesc(&bufferDesc);
		const auto _data = reinterpret_cast<const uint8_t*>(data);
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
		device->GetImmediateContext(context.ReleaseAndGetAddressOf());

		const auto dataSize = elementCount * vertexStride;

		//
		// 1. Criar buffer staging
		//
		D3D11_BUFFER_DESC stagingDesc = bufferDesc;
		stagingDesc.Usage = D3D11_USAGE_STAGING;
		stagingDesc.BindFlags = 0;
		stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		stagingDesc.MiscFlags = 0;

		Microsoft::WRL::ComPtr<ID3D11Buffer> stagingBuffer;

		HRESULT hr = device->CreateBuffer(&stagingDesc, nullptr, &stagingBuffer);
		if (FAILED(hr))
			throw CSharp::InvalidOperationException("Failed to create staging buffer");

		//
		// 2. Copiar buffer da GPU para o staging
		//
		context->CopyResource(stagingBuffer.Get(), buffer.Get());

		//
		// 3. Mapear staging buffer para leitura
		//
		D3D11_MAPPED_SUBRESOURCE mapped{};
		hr = context->Map(
			stagingBuffer.Get(),
			0,
			D3D11_MAP_READ,
			0,
			&mapped);

		if (FAILED(hr))
			throw CSharp::InvalidOperationException("Failed to map staging buffer");

		//
		// 4. Copiar dados para o array do usuário
		//
		std::memcpy(
			reinterpret_cast<uint8_t*>(data) + startIndex,
			static_cast<uint8_t*>(mapped.pData) + offsetInBytes,
			dataSize);

		//
		// 5. Desmapear
		//
		context->Unmap(stagingBuffer.Get(), 0);
	}		
}

#endif