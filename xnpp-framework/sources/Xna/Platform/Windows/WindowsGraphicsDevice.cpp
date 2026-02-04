#include "Xna/Platform/Platform.hpp"
#ifdef PLATFORM_WINDOWS
#include "Xna/Framework/Graphics/GraphicsDevice.hpp"

using Microsoft::WRL::ComPtr;

namespace Xna {
	void Platform::GraphicsDevice_CreateDevice(GraphicsDevice& device, GraphicsAdapter const& adapter, Xna::PresentationParameters const& presentationParameters) {
		// See ref
		//
		// D3D_DRIVER_TYPE
		// https://learn.microsoft.com/en-us/windows/win32/api/d3dcommon/ne-d3dcommon-d3d_driver_type
		// D3D11CreateDevice function 
		// https://learn.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-d3d11createdevice
		//		

		auto createDeviceFlags = 0;
#if _DEBUG
		createDeviceFlags = D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG;
#endif  
		const auto& pAdapter = GraphicsAdapter::UseNullDevice() ? NULL : adapter.impl->platformImpl.adapter.Get();

		//
		// if pAdapter is not NULL driverType must be D3D_DRIVER_TYPE_UNKNOWN
		//
		auto driverType = D3D_DRIVER_TYPE_UNKNOWN;

		if (GraphicsAdapter::UseReferenceDevice())
			driverType = D3D_DRIVER_TYPE_WARP;
		else if (GraphicsAdapter::UseNullDevice())
			driverType = D3D_DRIVER_TYPE_HARDWARE;

		auto hr = D3D11CreateDevice(
			//_In_opt_ IDXGIAdapter* pAdapter,
			pAdapter,
			//D3D_DRIVER_TYPE DriverType,
			driverType,
			//HMODULE Software,
			NULL,
			//UINT Flags,
			createDeviceFlags,
			//_In_reads_opt_( FeatureLevels ) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
			device.implGraphicsDevice->platformImpl.featureLevels,
			//UINT FeatureLevels,
			static_cast<UINT>(device.implGraphicsDevice->platformImpl.featureLevelCount),
			//UINT SDKVersion,
			D3D11_SDK_VERSION,
			//_COM_Outptr_opt_ ID3D11Device** ppDevice
			device.implGraphicsDevice->platformImpl.device.ReleaseAndGetAddressOf(),
			//_Out_opt_ D3D_FEATURE_LEVEL* pFeatureLevel,
			&device.implGraphicsDevice->platformImpl.currentFeatureLevel,
			//_COM_Outptr_opt_ ID3D11DeviceContext** ppImmediateContext
			device.implGraphicsDevice->platformImpl.context.ReleaseAndGetAddressOf());

		if FAILED(hr)
			throw CSharp::InvalidOperationException("D3D11CreateDevice failed.");

		auto swapDesc = WindowsPlatform::GetDefaultSwapChainDesc1();
		swapDesc.Format = DxHelpers::SurfaceFormatToDx(presentationParameters.BackBufferFormat);
		swapDesc.SampleDesc.Count = presentationParameters.MultiSampleCount == 0 ? 1 : static_cast<UINT>(presentationParameters.MultiSampleCount);

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc{};
		fullScreenDesc.Windowed = static_cast<BOOL>(!presentationParameters.IsFullScreen);

		if (presentationParameters.DeviceWindowHandle != 0)
		{
			auto hwnd = reinterpret_cast<HWND>(presentationParameters.DeviceWindowHandle);
			const auto swapChain = WindowsPlatform::CreateSwapChain1(swapDesc, fullScreenDesc, device.implGraphicsDevice->platformImpl.device, hwnd);

			device.implGraphicsDevice->platformImpl.swapChain = swapChain;
		}
		else {
			device.implGraphicsDevice->platformImpl.lazySwapChainDesc = swapDesc;
			device.implGraphicsDevice->platformImpl.lazySwapChainFullScreenDesc = fullScreenDesc;
		}

		device.implGraphicsDevice->presentationParameters = presentationParameters;
	}

	void Platform::GraphicsDevice_Present(GraphicsDevice const& graphicsDevice, std::optional<Rectangle> const& sourceRect, std::optional<Rectangle> const& destination, intptr_t overrideWindowHandle) {
		auto& impl = graphicsDevice.implGraphicsDevice->platformImpl;
		const auto& pp = graphicsDevice.implGraphicsDevice->presentationParameters;
		UINT syncInterval;

		switch (pp.PresentationInterval)
		{
		case PresentInterval::Default:
		case PresentInterval::One:
			syncInterval = 1;
			break;
		case PresentInterval::Two:
			syncInterval = 2;
			break;
		case PresentInterval::Immediate:
			syncInterval = 0;
			break;
		default:
			syncInterval = 1;
			break;
		}

		//TODO: implementar posteriormente

		if (sourceRect.has_value()) {
			D3D11_BOX srcBox{};
			srcBox.left = sourceRect.value().Left();
			srcBox.top = sourceRect.value().Top();
			srcBox.front = 0;
			srcBox.right = sourceRect.value().Right();
			srcBox.bottom = sourceRect.value().Bottom();
			srcBox.back = 1;

			/*graphicsDevice.implGraphicsDevice->context->CopySubresourceRegion(
				destTexture, 0,
				0, 0, 0,
				backbuffer, 0,
				&srcBox
			);*/
		}

		if (destination) {
			D3D11_VIEWPORT vp{};
			vp.TopLeftX = static_cast<FLOAT>(destination->Left());
			vp.TopLeftY = static_cast<FLOAT>(destination->Top());
			vp.Width = static_cast<FLOAT>(destination->Width);
			vp.Height = static_cast<FLOAT>(destination->Height);
			vp.MinDepth = 0.f;
			vp.MaxDepth = 1.f;

			graphicsDevice.implGraphicsDevice->platformImpl.context->RSSetViewports(1, &vp);
		}

		if (overrideWindowHandle != 0) {
			//TODO:
		}

		graphicsDevice.implGraphicsDevice->platformImpl.swapChain->Present(syncInterval, NULL);
	}

	void Platform::GraphicsDevice_SetViewport(GraphicsDevice const& graphicsDevice, Viewport const& viewport) {
		D3D11_VIEWPORT view = DxHelpers::ViewportToDx(viewport);
		graphicsDevice.implGraphicsDevice->platformImpl.context->RSSetViewports(1, &view);
	}

	void Platform::GraphicsDevice_MakeWindowAssociation(GraphicsDevice const& graphicsDevice, PresentationParameters const& pp) {
		auto factory = WindowsPlatform::GetDXGIFactory();

		auto hwnd = reinterpret_cast<HWND>(pp.DeviceWindowHandle);
		const auto hr = factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);

		if FAILED(hr)
			throw CSharp::InvalidOperationException("IDXGI11Factory.MakeWindowAssociation() failed.");
	}

	void Platform::GraphicsDevice_GetScissorRectangles(GraphicsDevice const& graphicsDevice, std::vector<Xna::Rectangle>& scissors) {
		auto& impl = graphicsDevice.implGraphicsDevice->platformImpl;

		UINT count{ 0 };

		impl.context->RSGetScissorRects(&count, NULL);
		auto rects = std::vector<D3D11_RECT>(static_cast<size_t>(count));
		impl.context->RSGetScissorRects(&count, rects.data());

		if (scissors.size() != static_cast<size_t>(count))
			scissors.resize(static_cast<size_t>(count));

		size_t index{ 0 };
		for (auto& r : rects) {
			Xna::Rectangle rect{ r.left, r.top, r.right - r.left, r.bottom - r.top };
			scissors[index] = rect;
			++index;
		}
	}

	void Platform::GraphicsDevice_SetScissorRectangles(GraphicsDevice const& graphicsDevice, std::vector<Xna::Rectangle>& scissors) {
		auto& impl = graphicsDevice.implGraphicsDevice->platformImpl;
		std::vector<D3D11_RECT> rects{ scissors.size() };
		size_t index{ 0 };

		for (auto& r : scissors) {
			D3D11_RECT rect{};
			rect.left = static_cast<LONG>(r.X);
			rect.top = static_cast<LONG>(r.Y);
			rect.left = static_cast<LONG>(r.Left());
			rect.bottom = static_cast<LONG>(r.Bottom());

			rects[index] = rect;
			++index;
		}

		const auto count = static_cast<LONG>(scissors.size());

		impl.context->RSSetScissorRects(count, rects.data());
	}

	void Platform::GraphicsDevice_ClearRenderTarget(GraphicsDevice const& graphicsDevice, Color const& color) {
		auto& impl = graphicsDevice.implGraphicsDevice->platformImpl;
		auto colorV4 = color.ToVector4();
		float colorRGBA[4] = { colorV4.X,colorV4.Y,colorV4.Z ,colorV4.W };

		auto& targets = graphicsDevice.implGraphicsDevice->current2DRenderTargets;

		for (auto& target : targets)
		{
			auto& implTarget = target.impl->platformImpl;
			impl.context->ClearRenderTargetView(implTarget.renderTargetView.Get(), colorRGBA);
		}
	}

	RenderTarget2D Platform::GraphicsDevice_CreateBackBufferRenderTarget(GraphicsDevice const& device) {
		auto& implDevice = device.implGraphicsDevice->platformImpl;

		ComPtr<ID3D11Texture2D> backBuffer;
		auto hr = implDevice.swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

		if FAILED(hr)
			throw CSharp::InvalidOperationException("IDXGISwapChain.GetBuffer() failed.");

		D3D11_TEXTURE2D_DESC textureDesc{};
		auto renderTarget = RenderTarget2D();
		auto& impl = renderTarget.impl->texture2D.impl;		

		impl->platformImpl.texture2D = backBuffer;
		impl->platformImpl.texture2D->GetDesc(&textureDesc);
		impl->format = DxHelpers::SurfaceFormatToXna(textureDesc.Format);
		impl->hasMipmaps = textureDesc.MipLevels > 0;
		impl->height = static_cast<size_t>(textureDesc.Height);
		impl->width = static_cast<size_t>(textureDesc.Width);

		auto& implRender = renderTarget.impl;

		hr = implDevice.device->CreateRenderTargetView(
			impl->platformImpl.texture2D.Get(),
			NULL,
			implRender->platformImpl.renderTargetView.ReleaseAndGetAddressOf());

		if FAILED(hr)
			throw CSharp::InvalidOperationException("ID3D11Device.CreateRenderTargetView() failed.");

		D3D11_RENDER_TARGET_VIEW_DESC viewDesc{};

		implRender->platformImpl.renderTargetView->GetDesc(&viewDesc);
		implRender->depthStencilFormat = DxHelpers::DepthFormatToXna(viewDesc.Format);
		implRender->multiSampleCount = static_cast<size_t>(textureDesc.SampleDesc.Count);

		return renderTarget;
	}

	void Platform::GraphicsDevice_SetRenderTargets(GraphicsDevice const& graphicsDevice) {
		auto& impl = graphicsDevice.implGraphicsDevice;
		auto& plat = impl->platformImpl;

		if (impl->current2DRenderTargets.size() == 1) {
			auto& renderTarget = impl->current2DRenderTargets[0];
			auto& renderPlatform = renderTarget.impl->platformImpl;

			plat.context->OMSetRenderTargets(1, renderPlatform.renderTargetView.GetAddressOf(), renderPlatform.depthStencilView.Get());
			return;
		}

		//TODO: implementar validações
		//Máx.Render Targets simultâneos: D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT(8)
		//Formato: Todos devem ser compatíveis
		//Dimensão: Mesma largura, altura e sample count
		//Multisampling: Deve ser idêntico em todos
		//DepthStencil:	Opcional, mas se usado deve ser compatível

		std::vector<ID3D11RenderTargetView*> nativeRenders{ impl->current2DRenderTargets.size() };
		size_t index = 0;

		for (auto& render : impl->current2DRenderTargets) {
			nativeRenders[index] = render.impl->platformImpl.renderTargetView.Get();
			++index;
		}

		auto& renderPlatform = impl->current2DRenderTargets[0].impl->platformImpl;
		plat.context->OMSetRenderTargets(static_cast<UINT>(nativeRenders.size()), nativeRenders.data(), renderPlatform.depthStencilView.Get());
	}

	void Platform::GraphicsDevice_Reset(GraphicsDevice& device, Xna::PresentationParameters const& presentationParameters, GraphicsAdapter const& graphicsAdapter) {
		auto& impl = device.implGraphicsDevice;
		auto& platform = impl->platformImpl;

		// =====================================================
		// 1) Atualizar parâmetros internos
		// =====================================================
		impl->presentationParameters = presentationParameters;
		impl->adapter = graphicsAdapter;

		// =====================================================
		// 2) Desvincular render targets atuais
		// =====================================================
		ID3D11RenderTargetView* nullRTV = nullptr;
		platform.context->OMSetRenderTargets(1, &nullRTV, nullptr);
		platform.context->Flush();

		// =====================================================
		// 3) Liberar backbuffer atual
		// =====================================================


		// =====================================================
		// 4) Redimensionar SwapChain
		// =====================================================
		if (platform.swapChain != nullptr)
		{
			DXGI_SWAP_CHAIN_DESC scDesc{};
			platform.swapChain->GetDesc(&scDesc);

			auto hr = platform.swapChain->ResizeBuffers(
				scDesc.BufferCount,
				presentationParameters.BackBufferWidth,
				presentationParameters.BackBufferHeight,
				DxHelpers::SurfaceFormatToDx(presentationParameters.BackBufferFormat),
				scDesc.Flags
			);

			if (FAILED(hr))
				throw std::runtime_error("IDXGISwapChain.ResizeBuffers failed.");

			//Trocar para fullscreen se necessário
			DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc{};
			platform.swapChain->GetFullscreenDesc(&fullScreenDesc);

			if (fullScreenDesc.Windowed != static_cast<BOOL>(!presentationParameters.IsFullScreen)) {
				const auto hr = platform.swapChain->SetFullscreenState(static_cast<BOOL>(presentationParameters.IsFullScreen), nullptr);

				if (FAILED(hr) && hr != DXGI_ERROR_NOT_CURRENTLY_AVAILABLE && hr != DXGI_STATUS_MODE_CHANGE_IN_PROGRESS)
					throw CSharp::InvalidOperationException("IDXGISwapChain1.SetFullscreenState() failed.");
			}

			//Criar novo renderTarget
			impl->current2DRenderTargets.clear();
			impl->current2DRenderTargets.resize(1);
			impl->current2DRenderTargets[0] = Platform::GraphicsDevice_CreateBackBufferRenderTarget(device);

			//Setar render targets
			Platform::GraphicsDevice_SetRenderTargets(device);
		}
		else {
			//platform.lazySwapChainDesc.Width = presentationParameters.BackBufferWidth;
			//platform.lazySwapChainDesc.Height = presentationParameters.BackBufferHeight;
			platform.lazySwapChainDesc.Format = DxHelpers::SurfaceFormatToDx(presentationParameters.BackBufferFormat);

			platform.lazySwapChainFullScreenDesc.Windowed = static_cast<BOOL>(!presentationParameters.IsFullScreen);
		}

		//Setar viewport
		impl->viewports.clear();
		impl->viewports.resize(1);
		impl->viewports[0] = Viewport(0, 0, presentationParameters.BackBufferWidth, presentationParameters.BackBufferHeight, 0, 1);
		Platform::GraphicsDevice_SetViewport(device, impl->viewports[0]);
	}

	void Platform::GraphicsDevice_LazyInitialization(GraphicsDevice& device, intptr_t windowHandle) {
		auto& impl = device.implGraphicsDevice->platformImpl;
		auto& pp = device.implGraphicsDevice->presentationParameters;

		if (impl.swapChain == nullptr) {
			if (windowHandle == 0)
				throw CSharp::InvalidOperationException("Window handle == 0.");

			const auto swapDesc = impl.lazySwapChainDesc;
			const auto swalFullScreenDesc = impl.lazySwapChainFullScreenDesc;

			auto hwnd = reinterpret_cast<HWND>(windowHandle);
			const auto swapChain = WindowsPlatform::CreateSwapChain1(swapDesc, swalFullScreenDesc, device.implGraphicsDevice->platformImpl.device, hwnd);

			impl.swapChain = swapChain;

			Platform::GraphicsDevice_MakeWindowAssociation(device, device.PresentationParameters());
			auto backBuffer = GraphicsDevice_CreateBackBufferRenderTarget(device);

			device.implGraphicsDevice->current2DRenderTargets.clear();
			device.implGraphicsDevice->current2DRenderTargets.resize(1);
			device.implGraphicsDevice->current2DRenderTargets.at(0) = backBuffer;

			pp.DeviceWindowHandle = windowHandle;
		}
	}

}

#endif