#include "Xna/Platform/Platform.hpp"
#ifdef PLATFORM_WINDOWS
#include "Xna/Framework/Graphics/GraphicsDevice.hpp"
#include "Xna/Framework/Graphics/Texture2D.hpp"
#include "Xna/Framework/Graphics/Texture3D.hpp"
#include "Xna/Framework/Graphics/RenderTarget2D.hpp"

using Microsoft::WRL::ComPtr;

namespace Xna {
	//
	//Texture2D
	//

	void Platform::Texture2D_Init(Texture2D& texture, GraphicsDevice const& device, size_t width, size_t height, bool mipMap, SurfaceFormat format) {
		auto& adpImpl = texture.impl->platformImpl;
		auto& grfImpl = device.implGraphicsDevice->platformImpl;

		D3D11_TEXTURE2D_DESC desc{};
		desc.Width = width;
		desc.Height = height;
		desc.ArraySize = 1;
		desc.Format = DxHelpers::SurfaceFormatToDx(format);
		desc.MipLevels = mipMap ? 0 : 1;
		desc.SampleDesc.Count = 1;
		// XNA usa textura dinâmica se pretende chamar SetData muitas vezes
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		if (mipMap)
			desc.BindFlags |= D3D11_BIND_RENDER_TARGET;

		desc.CPUAccessFlags = 0;
		desc.MiscFlags = mipMap ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

		HRESULT hr = grfImpl.device->CreateTexture2D(&desc, nullptr, adpImpl.texture2D.ReleaseAndGetAddressOf());

		if (FAILED(hr))
			return; //throw

		// Criar ShaderResourceView (igual ao XNA)
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = DxHelpers::SurfaceFormatToDx(format);

		if (mipMap)
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = -1;   // usar todos os mips
			srvDesc.Texture2D.MostDetailedMip = 0;
		}
		else
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
			srvDesc.Texture2D.MostDetailedMip = 0;
		}

		Microsoft::WRL::ComPtr<ID3D11Resource> resource = nullptr;
		if (adpImpl.texture2D->QueryInterface(IID_ID3D11Resource, (void**)resource.GetAddressOf()))
			return; //throw

		hr = grfImpl.device->CreateShaderResourceView(resource.Get(), &srvDesc, adpImpl.shaderResource.ReleaseAndGetAddressOf());

		if (FAILED(hr))
			throw std::runtime_error("Failed to create ShaderResourceView");

		// Se mipmaps são habilitados, gerar mips inicializados (por enquanto zerados)
		if (mipMap)
			grfImpl.context->GenerateMips(adpImpl.shaderResource.Get());
	}

	int32_t Platform::Texture2D_LevelCount(Texture2D const& texture) {
		D3D11_TEXTURE2D_DESC desc{};
		texture.impl->platformImpl.texture2D->GetDesc(&desc);

		return static_cast<int32_t>(desc.MipLevels);
	}

	Texture2D Platform::Texture2D_FromStream(GraphicsDevice const& device, CSharp::Stream& stream, size_t width, size_t height, bool zoom) {
		auto wicFactory = WindowsPlatform::GetWICFactory();

		// 1) Ler bytes do Stream para um buffer
		const auto imgSize = stream.Length();
		std::vector<BYTE> buffer(imgSize);
		stream.Read(reinterpret_cast<uint8_t*>(buffer.data()), imgSize);

		//// 2) Criar WIC stream + decodificador
		Microsoft::WRL::ComPtr<IWICStream> wicStream;
		wicFactory->CreateStream(&wicStream);
		wicStream->InitializeFromMemory(buffer.data(), (DWORD)imgSize);

		ComPtr<IWICBitmapDecoder> decoder;
		wicFactory->CreateDecoderFromStream(
			wicStream.Get(),
			nullptr,
			WICDecodeMetadataCacheOnLoad,
			&decoder
		);

		ComPtr<IWICBitmapFrameDecode> frame;
		decoder->GetFrame(0, &frame);

		// Tamanho original da imagem
		UINT srcW, srcH;
		frame->GetSize(&srcW, &srcH);

		//3) Ajustar o tamanho final (zoom ON/OFF)
		UINT finalW = width == 0 ? srcW : static_cast<UINT>(width);
		UINT finalH = height == 0 ? srcH : static_cast<UINT>(height);

		float scaleX = (float)finalW / srcW;
		float scaleY = (float)finalH / srcH;

		if (!zoom) {
			const auto scale = std::min(scaleX, scaleY);
			finalW = UINT(finalW * scale);
			finalH = UINT(finalH * scale);
		}

		// 4) Usar WIC para redimensionamento
		Microsoft::WRL::ComPtr<IWICBitmapScaler> scaler;
		wicFactory->CreateBitmapScaler(&scaler);

		scaler->Initialize(
			frame.Get(),
			finalW,
			finalH,
			WICBitmapInterpolationModeFant
		);

		// 5) Converter para formato DXGI-friendly (RGBA)
		Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
		wicFactory->CreateFormatConverter(&converter);

		converter->Initialize(
			scaler.Get(),
			GUID_WICPixelFormat32bppRGBA, // compatível com DXGI_FORMAT_R8G8B8A8_UNORM
			WICBitmapDitherTypeNone,
			nullptr,
			0.0f,
			WICBitmapPaletteTypeCustom
		);


		// 6) Criar o Texture2D final
		auto tex = Texture2D(
			device,
			finalW,
			finalH,
			true,     // gera mipmap
			SurfaceFormat::Color
		);

		// 7) Ler pixels convertidos para um buffer temporário
		const UINT bytesPerPixel = 4;
		const auto pixelsSize = finalW * finalH * bytesPerPixel;
		std::vector<BYTE> pixels(pixelsSize);

		converter->CopyPixels(
			nullptr,
		finalW * bytesPerPixel,		
			(UINT)pixels.size(),
			pixels.data()
		);

		// 8) Enviar para o GPU usando seu SetData
		tex.SetData(pixels);
		return tex;		
	}

	void Platform::Texture2D_SaveFile(Texture2D& texture, CSharp::Stream& stream, size_t width, size_t height, Platform::Texture2D_SaveFileType type) {
		auto wicFactory = WindowsPlatform::GetWICFactory();
		auto impl = texture.impl->platformImpl;

		// 1) Criar staging texture para copiar da GPU para CPU

		D3D11_TEXTURE2D_DESC desc{};
		impl.texture2D->GetDesc(&desc);

		desc.Width = width;
		desc.Height = height;
		desc.Usage = D3D11_USAGE_STAGING;
		desc.BindFlags = 0;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.MipLevels = 1;   // salvará somente o mip 0
		desc.MiscFlags = 0;

		//Obter o context
		Microsoft::WRL::ComPtr<ID3D11Device> device = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = nullptr;
		impl.texture2D->GetDevice(device.GetAddressOf());
		device->GetImmediateContext(context.GetAddressOf());

		HRESULT hr = device->CreateTexture2D(&desc, nullptr, impl.stagingTexture.ReleaseAndGetAddressOf());

		if (FAILED(hr))
			throw std::runtime_error("Erro ao criar staging texture para SaveAsPng");

		// 2) Copiar da texture original (mip 0) para a staging
		context->CopySubresourceRegion(
			impl.stagingTexture.Get(),     // destino
			0,
			0, 0, 0,
			impl.texture2D.Get(),     // origem
			0,
			nullptr
		);

		// 3) Mapear staging texture para leitura CPU
		D3D11_MAPPED_SUBRESOURCE mapped{};
		hr = context->Map(
			impl.stagingTexture.Get(),
			0,
			D3D11_MAP_READ,
			0,
			&mapped
		);

		if (FAILED(hr))
			throw std::runtime_error("Erro ao mapear staging texture para SaveAsPng");

		const UINT bytesPerPixel = 4;

		// 4) Criar WIC Bitmap para receber os pixels
		ComPtr<IWICBitmap> wicBitmap;
		hr = wicFactory->CreateBitmap(
			width,
			height,
			GUID_WICPixelFormat32bppRGBA,
			WICBitmapCacheOnLoad,
			&wicBitmap
		);

		if (FAILED(hr))
			throw std::runtime_error("Erro ao criar WIC Bitmap");

		// 5) Copiar linha a linha da staging texture para o bitmap WIC

		WICRect rect = { 0, 0, static_cast<INT>(width), static_cast<INT>(height) };
		ComPtr<IWICBitmapLock> lock;
		wicBitmap->Lock(&rect, WICBitmapLockWrite, &lock);

		UINT wicBufferSize = 0;
		BYTE* wicBuffer = nullptr;

		lock->GetDataPointer(&wicBufferSize, &wicBuffer);

		BYTE* src = reinterpret_cast<BYTE*>(mapped.pData);

		for (int y = 0; y < height; y++)
		{
			BYTE* srcRow = src + y * mapped.RowPitch;
			BYTE* dstRow = wicBuffer + y * width * bytesPerPixel;

			memcpy(dstRow, srcRow, width * bytesPerPixel);
		}

		context->Unmap(impl.stagingTexture.Get(), 0);

		// 6) Criar o codificador PNG ou JPEG
		Microsoft::WRL::ComPtr<IWICBitmapEncoder> encoder;
		GUID guid = type == Platform::Texture2D_SaveFileType::Png ? GUID_ContainerFormatPng : GUID_ContainerFormatJpeg;
		hr = wicFactory->CreateEncoder(guid, nullptr, &encoder);

		if (FAILED(hr))
			throw std::runtime_error("Erro ao criar encoder PNG");

		// 7) SHCreateMemStream(nullptr, 0) cria um stream vazio (capacidade interna cresce automaticamente)
		IStream* outStream = SHCreateMemStream(nullptr, 0);
		Microsoft::WRL::ComPtr<IStream> comIStream;
		comIStream.Attach(outStream);

		encoder->Initialize(comIStream.Get(), WICBitmapEncoderNoCache);

		// 8) Criar frame PNG
		ComPtr<IWICBitmapFrameEncode> frame;
		ComPtr<IPropertyBag2> props;

		encoder->CreateNewFrame(&frame, &props);
		frame->Initialize(props.Get());
		frame->SetSize(width, height);

		WICPixelFormatGUID format = GUID_WICPixelFormat32bppRGBA;
		frame->SetPixelFormat(&format);

		// 9) Escrever pixels no frame
		hr = frame->WriteSource(wicBitmap.Get(), nullptr);
		if (FAILED(hr))
			throw std::runtime_error("Erro ao escrever pixels no frame PNG");

		frame->Commit();
		encoder->Commit();

		// 5) Copiar o PNG final para o Stream customizado!
		DxHelpers::CopyIStreamToCustomStream(comIStream.Get(), stream);
	}

	void Platform::Texture2D_SetData(Texture2D& texture, size_t level, std::optional<Rectangle> const& rect, const void* data, size_t startIndex, size_t elementCount, bool hasMipMap, size_t sizeOfData) {
		auto& impl = texture.impl->platformImpl;

		const auto* srcBytes = reinterpret_cast<const BYTE*>(data) + startIndex;
		UINT srcBytesCount = static_cast<UINT>(elementCount * sizeOfData);

		// Obter descrição da textura
		D3D11_TEXTURE2D_DESC desc{};
		impl.texture2D->GetDesc(&desc);

		//Obter o context
		Microsoft::WRL::ComPtr<ID3D11Device> device = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = nullptr;
		impl.texture2D->GetDevice(device.GetAddressOf());
		device->GetImmediateContext(context.GetAddressOf());

		UINT width = std::max(1u, desc.Width >> level);
		UINT height = std::max(1u, desc.Height >> level);

		// ---------------------------
		// Caso textura não seja dinâmica → UpdateSubresource
		// ---------------------------

		if (desc.Usage != D3D11_USAGE_DYNAMIC)
		{
			D3D11_BOX box{};
			D3D11_BOX* pBox = nullptr;

			if (rect)
			{
				box.left = rect->Left();
				box.top = rect->Top();
				box.front = 0;
				box.right = rect->Right();
				box.bottom = rect->Bottom();
				box.back = 1;
				pBox = &box;
			}

			UINT rowPitch =
				rect ? static_cast<UINT>((rect->Right() - rect->Left()) * sizeOfData)
				: static_cast<UINT>(width * sizeOfData);

			context->UpdateSubresource(
				impl.texture2D.Get(),
				level,
				pBox,
				srcBytes,
				rowPitch,
				0
			);
		}
		else {
			D3D11_MAPPED_SUBRESOURCE mapped{};
			HRESULT hr = context->Map(
				impl.texture2D.Get(),
				level,
				D3D11_MAP_WRITE_DISCARD,
				0,
				&mapped
			);

			if (FAILED(hr))
				throw std::runtime_error("Failed to Map texture");

			BYTE* dst = reinterpret_cast<BYTE*>(mapped.pData);

			if (rect)
			{
				UINT copyWidthBytes = static_cast<UINT>((rect->Right() - rect->Left()) * sizeOfData);
				UINT copyHeight = static_cast<UINT>((rect->Bottom() - rect->Top()));

				const BYTE* src = srcBytes;
				BYTE* dstRow =
					dst + rect->Top() * mapped.RowPitch
					+ rect->Left() * sizeOfData;

				for (UINT y = 0; y < copyHeight; y++)
				{
					memcpy(dstRow, src, copyWidthBytes);
					dstRow += mapped.RowPitch;
					src += copyWidthBytes;
				}
			}
			else
			{
				// Cópia contínua
				memcpy(dst, srcBytes, srcBytesCount);
			}

			context->Unmap(impl.texture2D.Get(), level);
		}

		if (hasMipMap && level == 0)
			context->GenerateMips(impl.shaderResource.Get());
	}

	void Platform::Texture2D_GetData(Texture2D& texture, size_t level, std::optional<Rectangle> const& rect, void* data, size_t startIndex, size_t elementCount, size_t sizeOfData) {
		auto& impl = texture.impl->platformImpl;

		//Obter o device e context
		Microsoft::WRL::ComPtr<ID3D11Device> device = nullptr;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = nullptr;
		impl.texture2D->GetDevice(device.GetAddressOf());
		device->GetImmediateContext(context.GetAddressOf());

		const UINT elementSize = static_cast<UINT>(sizeOfData);

		// Obter descrição da textura
		D3D11_TEXTURE2D_DESC desc{};
		impl.texture2D->GetDesc(&desc);

		// 1. Dimensões do mip level
		// ---------------------------------------------------------
		UINT mipWidth = std::max(1u, desc.Width >> level);
		UINT mipHeight = std::max(1u, desc.Height >> level);

		UINT x = 0;
		UINT y = 0;
		UINT readWidth = mipWidth;
		UINT readHeight = mipHeight;

		if (rect)
		{
			x = rect->Left();
			y = rect->Top();
			readWidth = rect->Right() - rect->Left();
			readHeight = rect->Bottom() - rect->Top();

			assert(x + readWidth <= mipWidth);
			assert(y + readHeight <= mipHeight);
		}

		const UINT bytesPerPixel = DxHelpers::BytesPerPixelFromDXGIFormat(desc.Format);

		// 2. Criar staging texture se necessário
		// ---------------------------------------------------------

		if (!impl.stagingTexture)
		{
			D3D11_TEXTURE2D_DESC desc{};
			impl.texture2D->GetDesc(&desc);

			desc.Usage = D3D11_USAGE_STAGING;
			desc.BindFlags = 0;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			desc.MiscFlags = 0;

			HRESULT hr = device->CreateTexture2D(&desc, nullptr, &impl.stagingTexture);

			if (FAILED(hr))
				throw CSharp::InvalidOperationException("Error creating staging texture.");
		}

		// 3. Copiar do GPU para staging (somente o mip desejado)
		// ---------------------------------------------------------
		context->CopySubresourceRegion(
			impl.stagingTexture.Get(),
			level,
			0, 0, 0,
			impl.texture2D.Get(),
			level,
			nullptr
		);

		// 4. Mapear staging texture para leitura
		// ---------------------------------------------------------
		D3D11_MAPPED_SUBRESOURCE mapped{};
		HRESULT hr = context->Map(
			impl.stagingTexture.Get(),
			level,
			D3D11_MAP_READ,
			0,
			&mapped);

		if (FAILED(hr))
			throw CSharp::InvalidOperationException("Error in Map() staging texture.");

		// Row pitch pode ser maior que width * bytesPerPixel
		BYTE* src = reinterpret_cast<BYTE*>(mapped.pData);

		const UINT expectedBytes = readWidth * readHeight * bytesPerPixel;
		const UINT outputBytes = static_cast<UINT>(elementCount * elementSize);

		assert(outputBytes >= expectedBytes && "buffer do usuário pequeno");

		BYTE* dst = reinterpret_cast<BYTE*>(data) + startIndex;

		// 5. Copiar linha por linha do buffer mapeado
		// ---------------------------------------------------------
		for (UINT row = 0; row < readHeight; row++)
		{
			BYTE* srcRow = src + (y + row) * mapped.RowPitch + x * bytesPerPixel;
			BYTE* dstRow = dst + row * readWidth * bytesPerPixel;

			memcpy(dstRow, srcRow, static_cast<size_t>(readWidth) * bytesPerPixel);
		}

		// 6. Unmap
		// ---------------------------------------------------------
		context->Unmap(impl.stagingTexture.Get(), level);
	}

	//
	// RenderTarget2D
	//

	void Platform::RenderTarget2D_Init(RenderTarget2D& renderTarget, GraphicsDevice& device, DepthFormat preferredDepthFormat,
		size_t preferredMultiSampleCount, RenderTargetUsage usage) {
		auto& impl = renderTarget.impl;
		auto& platform = renderTarget.impl->platformImpl;
		auto& texture2D = impl->texture2D;
		auto& implTex2D = texture2D.impl;
		auto& platformTex2D = implTex2D->platformImpl;
		auto& deviceImpl = device.implGraphicsDevice->platformImpl;				

		// 1) Criar TEXTURE2D base (já criada pela base Texture2D)
		// Mas agora precisamos recriar com bind flags extras.

		// Obter descrição existente
		D3D11_TEXTURE2D_DESC desc{};
		platformTex2D.texture2D->GetDesc(&desc);
		desc.BindFlags |= D3D11_BIND_RENDER_TARGET;   // obrigatório
		desc.MipLevels = implTex2D->hasMipmaps ? desc.MipLevels : 1; // no render target só funciona com MIP autogerado
		desc.SampleDesc.Count = preferredMultiSampleCount; //Multisample
		desc.SampleDesc.Quality = 0;

		// Criar nova textura substituindo a da base
		platformTex2D.texture2D.Reset();
		auto hr = deviceImpl.device->CreateTexture2D(&desc, nullptr, platformTex2D.texture2D.GetAddressOf());

		if (FAILED(hr))
			throw std::runtime_error("RenderTarget2D: falha ao criar textura de render target");

		// 2) Criar Render Target View
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc{};
		rtvDesc.Format = desc.Format;

		if (preferredMultiSampleCount > 1) {
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
		}
		else {
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			rtvDesc.Texture2D.MipSlice = 0;
		}

		hr = deviceImpl.device->CreateRenderTargetView(platformTex2D.texture2D.Get(), &rtvDesc, platform.renderTargetView.ReleaseAndGetAddressOf());

		if (FAILED(hr))
			throw std::runtime_error("RenderTarget2D: falha ao criar RenderTargetView");

		// 3) Criar ShaderResourceView (para uso como textura)

		if (!platformTex2D.shaderResource)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
			srvDesc.Format = desc.Format;

			if (preferredMultiSampleCount > 1) {
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
			}
			else {
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MostDetailedMip = 0;
				srvDesc.Texture2D.MipLevels = desc.MipLevels;
			}

			hr = deviceImpl.device->CreateShaderResourceView(platformTex2D.texture2D.Get(), &srvDesc, platformTex2D.shaderResource.ReleaseAndGetAddressOf());
			if (FAILED(hr))
				throw std::runtime_error("RenderTarget2D: falha ao criar SRV para render target");
		}

		// 4) Criar DepthStencil Buffer (opcional)
		if (preferredDepthFormat != DepthFormat::None)
		{
			D3D11_TEXTURE2D_DESC depthDesc{};
			depthDesc.Width = static_cast<UINT>(implTex2D->width);
			depthDesc.Height = static_cast<UINT>(implTex2D->height);
			depthDesc.MipLevels = 1;
			depthDesc.ArraySize = 1;
			depthDesc.Format = DxHelpers::DepthFormatToDx(preferredDepthFormat);
			depthDesc.SampleDesc.Count = static_cast<UINT>(preferredMultiSampleCount);
			depthDesc.SampleDesc.Quality = 0;
			depthDesc.Usage = D3D11_USAGE_DEFAULT;
			depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

			hr = deviceImpl.device->CreateTexture2D(&depthDesc, nullptr, platform.depthStencilTexture.ReleaseAndGetAddressOf());

			if (FAILED(hr))
				throw std::runtime_error("RenderTarget2D: falha ao criar DepthStencilTexture");

			// Criar DSV
			D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
			dsvDesc.Format = depthDesc.Format;

			if (preferredMultiSampleCount > 1)
				dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
			else
				dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

			hr = deviceImpl.device->CreateDepthStencilView(platform.depthStencilTexture.Get(), &dsvDesc, platform.depthStencilView.ReleaseAndGetAddressOf());
			if (FAILED(hr))
				throw std::runtime_error("RenderTarget2D: falha ao criar DepthStencilView");
		}

		deviceImpl.context->OMSetRenderTargets(0, nullptr, nullptr);
	}

	//
	// Texture3D
	//

	void Platform::Texture3D_Init(Texture3D& texture, GraphicsDevice& graphicsDevice, size_t width, size_t height, size_t depth, bool mipMap, SurfaceFormat format) {
		auto& impl = texture.impl->platformImpl;
		auto& deviceImpl = graphicsDevice.implGraphicsDevice->platformImpl;

		auto& device = deviceImpl.device;
		//auto& context = deviceImpl.context;				

		// Calcular MIP levels
		if (mipMap) {
			auto maxDim = std::max(std::max(width, height), depth);
			auto levels = 1;

			while (maxDim > 1) {
				maxDim /= 2;
				levels++;
			}

			texture.impl->mipLevels = levels;
		}

		const auto mipLevels = texture.impl->mipLevels;

		// Criar TEXTURE3D
		// ==============================================
		D3D11_TEXTURE3D_DESC desc{};
		desc.Width = width;
		desc.Height = height;
		desc.Depth = depth;
		desc.MipLevels = mipLevels;
		desc.Format = DxHelpers::SurfaceFormatToDx(format);

		desc.BindFlags =
			D3D11_BIND_SHADER_RESOURCE;          // sempre precisa de SRV
		if (mipMap)
			desc.BindFlags |= D3D11_BIND_RENDER_TARGET;

		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;                // SetData usa UpdateSubresource
		desc.MiscFlags = mipMap ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

		HRESULT hr = device->CreateTexture3D(&desc, nullptr, impl.texture.ReleaseAndGetAddressOf());

		if (FAILED(hr))
			throw std::runtime_error("Texture3D: falha ao criar ID3D11Texture3D");

		// Criar SRV
		// ==============================================
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
		srvDesc.Texture3D.MostDetailedMip = 0;
		srvDesc.Texture3D.MipLevels = mipMap ? mipLevels : 1;

		hr = device->CreateShaderResourceView(impl.texture.Get(), &srvDesc, impl.shaderResource.ReleaseAndGetAddressOf());

		if (FAILED(hr))
			throw std::runtime_error("Texture3D: falha ao criar Shader Resource View.");
	}

	void Platform::Texture3D_SetData(Texture3D& texture, size_t level, size_t left, size_t top, size_t right, size_t bottom, size_t front, size_t back,
		const void* data, size_t startIndex, size_t elementCount) {

		if (!data)
			throw CSharp::InvalidOperationException("Data is null.");

		if (level >= texture.impl->mipLevels)
			throw CSharp::ArgumentOutOfRangeException("Level is greater then or equals to mipLevels.");

		Microsoft::WRL::ComPtr<ID3D11Device> device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
		texture.impl->platformImpl.texture->GetDevice(device.ReleaseAndGetAddressOf());

		// Calcular o tamanho do mip level
		// ===========================================
		size_t min = 1;

		const auto mipWidth = std::max(min, texture.impl->width >> level);
		const auto mipHeight = std::max(min, texture.impl->height >> level);
		const auto mipDepth = std::max(min, texture.impl->depth >> level);

		// Validar bounds
		if (right > mipWidth || bottom > mipHeight || back > mipDepth) {
			throw CSharp::ArgumentOutOfRangeException("The region is off-limits.");
		}

		// Dimensões da região
		const auto boxWidth = right - left;
		const auto boxHeight = bottom - top;
		const auto boxDepth = back - front;

		if (boxWidth <= 0 || boxHeight <= 0 || boxDepth <= 0)
			return; // nada a fazer

		// Determinar tamanho de pixel do SurfaceFormat
		// ===========================================
		DXGI_FORMAT dxgiFormat = DxHelpers::SurfaceFormatToDx(texture.impl->format);
		UINT bytesPerPixel = DxHelpers::BytesPerPixelFromDXGIFormat(dxgiFormat);

		// Calcular ponteiro inicial dentro do buffer
		// ===========================================
		const uint8_t* srcData = reinterpret_cast<const uint8_t*>(data) + startIndex * bytesPerPixel;

		// Criar D3D11_BOX
		// ===========================================
		D3D11_BOX box{};
		box.left = static_cast<UINT>(left);
		box.top = static_cast<UINT>(top);
		box.front = static_cast<UINT>(front);
		box.right = static_cast<UINT>(right);
		box.bottom = static_cast<UINT>(bottom);
		box.back = static_cast<UINT>(back);

		// Calcular pitch
		// ===========================================
		const auto rowPitch = static_cast<UINT>(boxWidth) * bytesPerPixel;
		const auto depthPitch = static_cast<UINT>(boxWidth) * static_cast<UINT>(boxHeight) * bytesPerPixel;

		// UpdateSubresource (rápido e direto)
		// ===========================================
		context->UpdateSubresource(
			texture.impl->platformImpl.texture.Get(),
			level,
			&box,
			srcData,
			rowPitch,
			depthPitch
		);
	}

	void Platform::Texture3D_GetData(Texture3D& texture, size_t level, size_t left, size_t top, size_t right, size_t bottom, size_t front, size_t back,
		void* data, size_t startIndex, size_t elementCount, size_t sizeOfData) {

		if (!data)
			throw CSharp::ArgumentException("data não pode ser nulo.");

		if (level >= texture.impl->mipLevels)
			throw CSharp::ArgumentOutOfRangeException("Level is greater then or equals to mipLevels.");

		const auto elementSize = sizeOfData;

		// Dimensões do nível solicitado
		size_t min = 1;
		const auto mipWidth = std::max(min, texture.impl->width >> level);
		const auto mipHeight = std::max(min, texture.impl->height >> level);
		const auto mipDepth = std::max(min, texture.impl->depth >> level);

		// Normalização dos limites
		min = 0;
		left = std::max(min, std::min(left, mipWidth));
		right = std::max(min, std::min(right, mipWidth));
		top = std::max(min, std::min(top, mipHeight));
		bottom = std::max(min, std::min(bottom, mipHeight));
		front = std::max(min, std::min(front, mipDepth));
		back = std::max(min, std::min(back, mipDepth));

		const auto widthToRead = right - left;
		const auto heightToRead = bottom - top;
		const auto depthToRead = back - front;

		if (widthToRead <= 0 || heightToRead <= 0 || depthToRead <= 0)
			return;

		const auto totalElementsNeeded = static_cast<UINT>(widthToRead * heightToRead * depthToRead);

		if (static_cast<UINT>(elementCount) < totalElementsNeeded)
			throw CSharp::ArgumentOutOfRangeException("elementCount < totalElementsNeeded.");

		//--------------------------------------------------------------------
		// Criar a staging texture se ela ainda não existir
		//--------------------------------------------------------------------
		auto& impl = texture.impl->platformImpl;

		Microsoft::WRL::ComPtr<ID3D11Device> device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
		impl.texture->GetDevice(device.ReleaseAndGetAddressOf());
		device->GetImmediateContext(context.ReleaseAndGetAddressOf());

		if (!impl.stagingTexture)
		{
			D3D11_TEXTURE3D_DESC desc{};
			impl.texture->GetDesc(&desc);

			desc.Usage = D3D11_USAGE_STAGING;
			desc.BindFlags = 0;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			desc.MiscFlags = 0;

			const auto hr = device->CreateTexture3D(&desc, nullptr, impl.stagingTexture.ReleaseAndGetAddressOf());

			if (FAILED(hr))
				throw CSharp::InvalidOperationException("ID3D11Device.CreateTexture3D() failed.");
		}

		//--------------------------------------------------------------------
		// Copiar o mip level específico
		//--------------------------------------------------------------------
		const auto srcSubresource = D3D11CalcSubresource(level, 0, texture.impl->mipLevels);

		context->CopySubresourceRegion(
			impl.stagingTexture.Get(),
			srcSubresource,
			0, 0, 0, // destino
			impl.texture.Get(),
			srcSubresource,
			nullptr // copia completa
		);

		//--------------------------------------------------------------------
		// Mapear a staging para leitura
		//--------------------------------------------------------------------
		D3D11_MAPPED_SUBRESOURCE mapped{};
		const auto hr = context->Map(
			impl.stagingTexture.Get(),
			srcSubresource,
			D3D11_MAP_READ,
			0,
			&mapped
		);

		if (FAILED(hr))
			throw CSharp::InvalidOperationException("ID3D11DeviceContext.Map() failed.");

		//--------------------------------------------------------------------
		// Cópia manual do subvolume
		//--------------------------------------------------------------------

		auto srcBase = static_cast<BYTE*>(mapped.pData);

		const auto rowPitch = static_cast<size_t>(mapped.RowPitch);     // bytes por linha
		const auto depthPitch = static_cast<size_t>(mapped.DepthPitch);   // bytes por "slice" Z

		auto dst = reinterpret_cast<BYTE*>(data) + (startIndex * elementSize);

		for (int z = 0; z < depthToRead; z++)
		{
			BYTE* srcSlice = srcBase + (front + z) * depthPitch;

			for (int y = 0; y < heightToRead; y++)
			{
				BYTE* srcRow = srcSlice + (top + y) * rowPitch;
				BYTE* srcPixel = srcRow + (left * elementSize);
				memcpy(dst, srcPixel, widthToRead * elementSize);

				dst += widthToRead * elementSize;
			}
		}

		context->Unmap(impl.stagingTexture.Get(), srcSubresource);
	}
}

#endif