#include <stdexcept>
#include <cstddef>
#include "SDL3/SDL.h"
#include "Xna/Platform/Platform.hpp"
#include "Xna/Framework/Graphics/GraphicsAdapter.hpp"

#if PLATFORM_WINDOWS
#include <wrl\client.h>
#include "dxgi.h"
#endif

namespace Xna {
#if PLATFORM_WINDOWS
	using Microsoft::WRL::ComPtr;
	struct SdlGraphicsAdapter;

	std::string WCharToString(const WCHAR* wstr) {
		if (!wstr || wstr[0] == L'\0') {
			return std::string();
		}

		// 1. Descobre o tamanho necessário para a string UTF-8 (incluindo o caractere nulo)
		int size_needed = WideCharToMultiByte(
			CP_UTF8,            // Página de código de destino (UTF-8)
			0,                  // Flags de conversão
			wstr,               // String de origem (WCHAR*)
			-1,                 // -1 indica que a string termina com '\0'
			nullptr,            // Buffer de destino nulo para apenas calcular o tamanho
			0,                  // Tamanho do buffer (0 para calcular)
			nullptr,            // Caractere padrão (não usado com CP_UTF8)
			nullptr             // Flag de uso de caractere padrão (não usado com CP_UTF8)
		);

		if (size_needed <= 0) {
			return std::string();
		}

		// 2. Aloca a std::string (descontando o '\0' final, pois std::string já gerencia isso)
		std::string result(size_needed - 1, '\0');

		// 3. Executa a conversão real
		WideCharToMultiByte(
			CP_UTF8,
			0,
			wstr,
			-1,
			&result[0],         // Em C++17+, pode-se usar result.data()
			size_needed,
			nullptr,
			nullptr
		);

		return result;
	}

	struct WindowsGraphicsAdapter {
		static void SetDesc(Platform::GraphicsAdapterDesc& desc, size_t index) {
			Microsoft::WRL::ComPtr<IDXGIFactory1> pFactory;

			if FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)pFactory.ReleaseAndGetAddressOf()))
				throw std::runtime_error("WindowsGraphicsAdapter: CreateDXGIFactory1 failed.");

			ComPtr<IDXGIAdapter1> pAdapter = nullptr;
			
			if (pFactory->EnumAdapters1(static_cast<UINT>(index), pAdapter.GetAddressOf()) == DXGI_ERROR_NOT_FOUND)
				return;

			DXGI_ADAPTER_DESC1 dxDesc{};
			pAdapter->GetDesc1(&dxDesc);

			
			desc.description = WCharToString(dxDesc.Description);
			desc.deviceId = static_cast<uint32_t>(dxDesc.DeviceId);
			desc.revision = static_cast<uint32_t>(dxDesc.Revision);
			desc.subSystemId = static_cast<uint32_t>(dxDesc.SubSysId);
			desc.vendorId = static_cast<uint32_t>(dxDesc.VendorId);

			ComPtr<IDXGIOutput> pOutput = nullptr;

			if (pAdapter->EnumOutputs(0, pOutput.GetAddressOf()) != DXGI_ERROR_NOT_FOUND) {
				DXGI_OUTPUT_DESC outputDesc;
				pOutput->GetDesc(&outputDesc);

				desc.monitorHandle = reinterpret_cast<intptr_t>(outputDesc.Monitor);
				desc.deviceName = WCharToString(outputDesc.DeviceName);
			}
		}		
	};
#endif

	struct SdlGraphicsAdapter final : public Platform::IGraphicsAdapter {
		Platform::GraphicsAdapterDesc _desc{};
		SDL_DisplayID _id{0};		

		constexpr SdlGraphicsAdapter() = default;
		constexpr SdlGraphicsAdapter(Platform::GraphicsAdapterDesc const& desc, SDL_DisplayID id) : _desc(desc), _id(id){}

		const Platform::GraphicsAdapterDesc& GetDesc() const override {
			return _desc;
		}

		std::vector<std::unique_ptr<Platform::IGraphicsAdapter>> GetAll() override {
			int displayCount = 0;
			SDL_DisplayID* displays = SDL_GetDisplays(&displayCount);

			if (displayCount == 0)
				return {};

			std::vector<std::unique_ptr<Platform::IGraphicsAdapter>> adapters;
			adapters.resize(displayCount);

			for (int i = 0; i < displayCount; ++i) {
				Platform::GraphicsAdapterDesc desc{};
				desc.deviceName = SDL_GetDisplayName(displays[i]);

#if PLATFORM_WINDOWS
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

	std::unique_ptr<Platform::IGraphicsAdapter> Platform::IGraphicsAdapter::Create() {
		return std::make_unique<SdlGraphicsAdapter>();
	}
}
