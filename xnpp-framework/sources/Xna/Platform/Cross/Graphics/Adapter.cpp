#include <stdexcept>
#include "SDL3/SDL.h"
#include "Xna/Platform/_Platform.hpp"
#include "Xna/Framework/Graphics/GraphicsAdapter.hpp"

#ifdef PLATFORM_WINDOWS
#include <wrl\client.h>
#include "dxgi.h"
#include "Xna/Internal/Misc.hpp"
#endif

namespace Xna {
#ifdef PLATFORM_WINDOWS
	using Microsoft::WRL::ComPtr;
	struct SdlGraphicsAdapter;

	struct WindowsGraphicsAdapter {
		static void SetDesc(PlatformNS::GraphicsAdapterDesc& desc, size_t index) {
			Microsoft::WRL::ComPtr<IDXGIFactory1> pFactory;

			if FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)pFactory.ReleaseAndGetAddressOf()))
				throw std::runtime_error("WindowsGraphicsAdapter: CreateDXGIFactory1 failed.");

			ComPtr<IDXGIAdapter1> pAdapter = nullptr;
			
			if (pFactory->EnumAdapters1(static_cast<UINT>(index), pAdapter.GetAddressOf()) == DXGI_ERROR_NOT_FOUND)
				return;

			DXGI_ADAPTER_DESC1 dxDesc{};
			pAdapter->GetDesc1(&dxDesc);

			
			desc.description = Xna::Misc::ToString(dxDesc.Description);
			desc.deviceId = static_cast<uint32_t>(dxDesc.DeviceId);
			desc.revision = static_cast<uint32_t>(dxDesc.Revision);
			desc.subSystemId = static_cast<uint32_t>(dxDesc.SubSysId);
			desc.vendorId = static_cast<uint32_t>(dxDesc.VendorId);

			ComPtr<IDXGIOutput> pOutput = nullptr;

			if (pAdapter->EnumOutputs(0, pOutput.GetAddressOf()) != DXGI_ERROR_NOT_FOUND) {
				DXGI_OUTPUT_DESC outputDesc;
				pOutput->GetDesc(&outputDesc);

				desc.monitorHandle = reinterpret_cast<intptr_t>(outputDesc.Monitor);
				desc.deviceName = Xna::Misc::ToString(outputDesc.DeviceName);
			}
		}		
	};
#endif

	struct SdlGraphicsAdapter final : public PlatformNS::IGraphicsAdapter {
		PlatformNS::GraphicsAdapterDesc _desc{};
		SDL_DisplayID _id{0};		

		constexpr SdlGraphicsAdapter() = default;
		constexpr SdlGraphicsAdapter(PlatformNS::GraphicsAdapterDesc const& desc, SDL_DisplayID id) : _desc(desc), _id(id){}

		const PlatformNS::GraphicsAdapterDesc& GetDesc() const override {
			return _desc;
		}

		std::vector<std::unique_ptr<PlatformNS::IGraphicsAdapter>> GetAll() override {
			int displayCount = 0;
			SDL_DisplayID* displays = SDL_GetDisplays(&displayCount);

			if (displayCount == 0)
				return {};

			std::vector<std::unique_ptr<PlatformNS::IGraphicsAdapter>> adapters;
			adapters.resize(displayCount);

			for (int i = 0; i < displayCount; ++i) {
				PlatformNS::GraphicsAdapterDesc desc{};
				desc.deviceName = SDL_GetDisplayName(displays[i]);

#ifdef PLATFORM_WINDOWS
				WindowsGraphicsAdapter::SetDesc(desc, i);
#endif				

				desc.isDefaultAdapter = i == 0;
				adapters[i] = std::make_unique<SdlGraphicsAdapter>(desc, displays[i]);
			}			

			SDL_free(displays);

			return adapters;			
		}		

		static SurfaceFormat MapSDLFormatToSurface(SDL_PixelFormat sdlFormat) {
			switch (sdlFormat) {
			case SDL_PIXELFORMAT_RGBA8888: return SurfaceFormat::Color;
			case SDL_PIXELFORMAT_XRGB8888: return SurfaceFormat::Color;
			case SDL_PIXELFORMAT_BGR565: return SurfaceFormat::Bgr565;
			case SDL_PIXELFORMAT_BGRA5551: return SurfaceFormat::Bgra5551;
			case SDL_PIXELFORMAT_BGRA4444: return SurfaceFormat::Bgra4444;
			default: return SurfaceFormat::Unknown;
			}
		}

		std::vector<DisplayMode> SupportedDisplayModes() override {
			int modeCount = 0;			
			const auto modes = SDL_GetFullscreenDisplayModes(_id, &modeCount);

			std::vector<DisplayMode> displayModes;

			if (modes) {
				for (int j = 0; j < modeCount; ++j) {
					const SDL_DisplayMode* mode = modes[j];

					const auto format = MapSDLFormatToSurface(mode->format);

					if (format == SurfaceFormat::Unknown)
						continue;

					const auto display = DisplayMode(mode->w, mode->h, format);
					displayModes.push_back(display);
				}

				SDL_free((void*)modes);
			}

			return displayModes;
		}

		DisplayMode CurrentDisplayMode() override {
			auto mode = SDL_GetCurrentDisplayMode(_id);

			const auto display = DisplayMode(mode->w, mode->h, MapSDLFormatToSurface(mode->format));
			return display;
		}

		bool IsProfileSupported(GraphicsProfile graphicsProfile) override {
			return true;
		}

		bool QueryBackBufferFormat(GraphicsProfile graphicsProfile, SurfaceFormat format,
			DepthFormat depthFormat, int32_t multiSampleCount, SurfaceFormat& selectedFormat, DepthFormat& selectedDepthFormat, int32_t& selectedMultiSampleCount) override {
			return true;
		}
		bool QueryRenderTargetFormat(GraphicsProfile graphicsProfile, SurfaceFormat format,
			DepthFormat depthFormat, int32_t multiSampleCount, SurfaceFormat& selectedFormat, DepthFormat& selectedDepthFormat, int32_t& selectedMultiSampleCount) override {
			return true;
		}

		~SdlGraphicsAdapter() override = default;
	};

	std::unique_ptr<PlatformNS::IGraphicsAdapter> PlatformNS::IGraphicsAdapter::Create() {
		return std::make_unique<SdlGraphicsAdapter>();
	}
}