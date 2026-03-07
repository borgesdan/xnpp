#include <stdexcept>
#include "SDL3/SDL.h"
#include "Xna/Platform/_Platform.hpp"

#ifdef PLATFORM_WINDOWS
#include <wrl\client.h>
#include "dxgi.h"
#include "Xna/Internal/Misc.hpp"
#endif

#include "Xna/Framework/Graphics/GraphicsAdapter.hpp"

namespace Xna {
#ifdef PLATFORM_WINDOWS
	using Microsoft::WRL::ComPtr;
	struct WindowsGraphicsAdapter {
		static inline std::vector<PlatformNS::GaphicsAdapterDesc> GetAll() {
			Microsoft::WRL::ComPtr<IDXGIFactory1> pFactory;

			if FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)pFactory.ReleaseAndGetAddressOf()))
				throw std::runtime_error("WindowsGraphicsAdapter: CreateDXGIFactory1 failed.");

			ComPtr<IDXGIAdapter1> pAdapter = nullptr;
			std::vector<PlatformNS::GaphicsAdapterDesc> adapters;
			size_t adaptersCount = 0;

			for (UINT count = 0; pFactory->EnumAdapters1(count, pAdapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++count) {
				DXGI_ADAPTER_DESC1 desc{};
				pAdapter->GetDesc1(&desc);

				PlatformNS::GaphicsAdapterDesc adp;
				adp.description = Xna::Misc::ToString(desc.Description);
				adp.deviceId = static_cast<uint32_t>(desc.DeviceId);
				adp.revision = static_cast<uint32_t>(desc.Revision);
				adp.subSystemId = static_cast<uint32_t>(desc.SubSysId);
				adp.vendorId = static_cast<uint32_t>(desc.VendorId);

				ComPtr<IDXGIOutput> pOutput = nullptr;

				if (pAdapter->EnumOutputs(0, pOutput.GetAddressOf()) != DXGI_ERROR_NOT_FOUND) {
					DXGI_OUTPUT_DESC outputDesc;
					pOutput->GetDesc(&outputDesc);

					adp.monitorHandle = reinterpret_cast<intptr_t>(outputDesc.Monitor);
					adp.deviceName = Xna::Misc::ToString(outputDesc.DeviceName);
				}

				adapters.push_back(adp);
			}

			return adapters;
		}
	};
#endif

	struct SdlGraphicsAdapter final : public PlatformNS::IGraphicsAdapter {
		std::vector<PlatformNS::GaphicsAdapterDesc> GetAll() override {
#ifdef PLATFORM_WINDOWS
			return WindowsGraphicsAdapter::GetAll();
#endif
			return getAll();			
		}

		std::vector<PlatformNS::GaphicsAdapterDesc> getAll() {
			int displayCount = 0;
			SDL_DisplayID* displays = SDL_GetDisplays(&displayCount);

			if (displayCount == 0) {
				SDL_free(displays);
				return {};
			}

			std::vector<PlatformNS::GaphicsAdapterDesc> adapters(displayCount);

			for (int i = 0; i < displayCount; ++i)
				adapters[i].deviceName = SDL_GetDisplayName(displays[i]);

			SDL_free(displays);
			return adapters;
		}

		void SupportedDisplayModes(GraphicsAdapter const& adapter) override {}
		void CurrentDisplayMode(GraphicsAdapter const& adapter) override {}
		bool IsProfileSupported(GraphicsAdapter const& adapter, GraphicsProfile graphicsProfile) override { return false; }
		bool QueryBackBufferFormat(GraphicsAdapter const& adapter, GraphicsProfile graphicsProfile, SurfaceFormat format,
			DepthFormat depthFormat, int32_t multiSampleCount, SurfaceFormat& selectedFormat, DepthFormat& selectedDepthFormat, int32_t& selectedMultiSampleCount) override {
			return false;
		}
		bool QueryRenderTargetFormat(GraphicsAdapter const& adapter, GraphicsProfile graphicsProfile, SurfaceFormat format,
			DepthFormat depthFormat, int32_t multiSampleCount, SurfaceFormat& selectedFormat, DepthFormat& selectedDepthFormat, int32_t& selectedMultiSampleCount) override {
			return false;
		}

		~SdlGraphicsAdapter() override = default;
	};

	std::unique_ptr<PlatformNS::IGraphicsAdapter> PlatformNS::IGraphicsAdapter::Create() {
		return std::make_unique<SdlGraphicsAdapter>();
	}
}