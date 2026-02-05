#include "Xna/Platform/Platform.hpp"
#ifdef PLATFORM_WINDOWS
#include "Xna/Internal/Misc.hpp"
#include "Xna/Framework/Graphics/GraphicsDevice.hpp"
#include "Xna/Framework/Game/GraphicsDeviceManager.hpp"

using Microsoft::WRL::ComPtr;

namespace Xna {
	std::vector<std::optional<GraphicsAdapter>> Platform::GraphicsAdapter_GetAllGraphicsAdapters() {
		auto pFactory = WindowsPlatform::GetDXGIFactory();

		ComPtr<IDXGIAdapter1> pAdapter = nullptr;
		std::vector<std::optional<GraphicsAdapter>> adapters;
		size_t adaptersCount = 0;

		for (UINT count = 0; pFactory->EnumAdapters1(count, pAdapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++count) {
			DXGI_ADAPTER_DESC1 desc{};
			pAdapter->GetDesc1(&desc);

			auto adp = GraphicsAdapter();
			adp.impl->platformImpl.adapter = pAdapter;
			adp.impl->platformImpl.factory = pFactory;
			adp.impl->isDefaultAdapter = count == 0;

			adp.impl->description = Xna::Misc::ToString(desc.Description);
			adp.impl->deviceId = static_cast<uint32_t>(desc.DeviceId);
			adp.impl->revision = static_cast<uint32_t>(desc.Revision);
			adp.impl->subSystemId = static_cast<uint32_t>(desc.SubSysId);
			adp.impl->vendorId = static_cast<uint32_t>(desc.VendorId);

			ComPtr<IDXGIOutput> pOutput = nullptr;

			if (pAdapter->EnumOutputs(0, pOutput.GetAddressOf()) != DXGI_ERROR_NOT_FOUND) {
				DXGI_OUTPUT_DESC outputDesc;
				pOutput->GetDesc(&outputDesc);

				adp.impl->monitorHandle = reinterpret_cast<intptr_t>(outputDesc.Monitor);
				adp.impl->deviceName = Xna::Misc::ToString(outputDesc.DeviceName);
			}

			adapters.push_back(adp);
			++adaptersCount;
		}

		adapters.resize(adaptersCount);
		return adapters;
	}

	void Platform::GraphicsAdapter_SupportedDisplayModes(GraphicsAdapter const& adapter) {
		auto& pAdapter = adapter.impl->platformImpl.adapter;

		ComPtr<IDXGIOutput> pOutput = nullptr;
		ComPtr<IDXGIOutput1> pOutput1 = nullptr;
		size_t numModes = 0;
		const SurfaceFormat validTextureFormats[] = {
				SurfaceFormat::Color,
				SurfaceFormat::Bgr565,
				SurfaceFormat::Bgra5551,
				SurfaceFormat::Bgra4444,
				SurfaceFormat::Dxt1,
				SurfaceFormat::Dxt3,
				SurfaceFormat::Dxt5,
				SurfaceFormat::NormalizedByte2,
				SurfaceFormat::NormalizedByte4
		};
		const size_t validTextureFormatsCount = 9;

		//Count display Modes
		if (pAdapter->EnumOutputs(0, pOutput.GetAddressOf()) != DXGI_ERROR_NOT_FOUND) {


			for (size_t f = 0; f < validTextureFormatsCount; ++f) {
				const auto currentSurface = validTextureFormats[f];
				DXGI_FORMAT format = DxHelpers::SurfaceFormatToDx(currentSurface);

				UINT num = 0;

				if (!pOutput1) {
					pOutput->QueryInterface(IID_IDXGIOutput1, (void**)pOutput1.GetAddressOf());
				}

				pOutput1->GetDisplayModeList1(format, 0, &num, nullptr);
				numModes += num;
			}
		}

		std::vector<DXGI_MODE_DESC1> buffer(numModes);
		UINT bufferOffset = 0;

		//Set display modes
		if (pAdapter->EnumOutputs(0, pOutput.GetAddressOf()) != DXGI_ERROR_NOT_FOUND) {
			for (size_t f = 0; f < validTextureFormatsCount; ++f) {
				const auto currentSurface = validTextureFormats[f];
				DXGI_FORMAT format = DxHelpers::SurfaceFormatToDx(currentSurface);

				UINT numModes = 0;

				if (!pOutput1) {
					pOutput->QueryInterface(IID_IDXGIOutput1, (void**)pOutput1.GetAddressOf());
				}

				pOutput1->GetDisplayModeList1(format, 0, &numModes, nullptr);

				if (numModes == 0)
					continue;


				pOutput1->GetDisplayModeList1(format, 0, &numModes, buffer.data() + bufferOffset);

				bufferOffset += numModes;
			}
		}

		if (!pOutput)
			return;

		//Create DisplayModeCollection
		auto collection = DisplayModeCollection();
		std::vector<std::optional<DisplayMode>> displayList{ buffer.size() };

		for (size_t i = 0; i < buffer.size(); ++i) {
			auto& modedesc = buffer[i];

			const auto display = DisplayMode(
				modedesc.Width,
				modedesc.Height,
				DxHelpers::SurfaceFormatToXna(modedesc.Format));

			displayList[i] = display;
		}

		adapter.impl->supportedDisplayModes = displayList;
	}

	void Platform::GraphicsAdapter_CurrentDisplayMode(GraphicsAdapter const& adapter) {
		auto& pAdapter = adapter.impl->platformImpl.adapter;

		ComPtr<IDXGIOutput> output;
		auto hr = pAdapter->EnumOutputs(0, &output);

		if FAILED(hr)
			return;

		ComPtr<IDXGIOutput1> output1;
		output.As(&output1);

		//TODO: verificar
		DXGI_MODE_DESC1 requestMode = {};
		requestMode.Width = GraphicsDeviceManager::DefaultBackBufferWidth;
		requestMode.Height = GraphicsDeviceManager::DefaultBackBufferHeight;
		requestMode.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		DXGI_MODE_DESC1 current = {};
		output1->FindClosestMatchingMode1(&requestMode, &current, nullptr);

		const auto display = DisplayMode(
			current.Width,
			current.Height,
			SurfaceFormat::Color);

		adapter.impl->currentDisplayMode = display;
	}

	bool Platform::GraphicsAdapter_IsProfileSupported(GraphicsAdapter const& adapter, GraphicsProfile graphicsProfile) {
		return true;
	}

	bool Platform::GraphicsAdapter_QueryBackBufferFormat(
		GraphicsAdapter const& adapter,
		GraphicsProfile graphicsProfile,
		SurfaceFormat format,
		DepthFormat depthFormat,
		int32_t multiSampleCount,
		SurfaceFormat& selectedFormat,
		DepthFormat& selectedDepthFormat,
		int32_t& selectedMultiSampleCount) {

		selectedFormat = DxHelpers::SurfaceFormatToXna(DXGI_FORMAT_B8G8R8A8_UNORM);
		selectedDepthFormat = depthFormat;
		selectedMultiSampleCount = multiSampleCount;

		return true;
	}

	bool Platform::GraphicsAdapter_QueryRenderTargetFormat(
		GraphicsAdapter const& adapter,
		GraphicsProfile graphicsProfile,
		SurfaceFormat format,
		DepthFormat depthFormat,
		int32_t multiSampleCount,
		SurfaceFormat& selectedFormat,
		DepthFormat& selectedDepthFormat,
		int32_t& selectedMultiSampleCount) {

		selectedFormat = DxHelpers::SurfaceFormatToXna(DXGI_FORMAT_B8G8R8A8_UNORM);
		selectedDepthFormat = depthFormat;
		selectedMultiSampleCount = multiSampleCount;

		return true;
	}
}

#endif