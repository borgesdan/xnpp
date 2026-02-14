#include "Xna/Platform/Platform.hpp"
#ifdef PLATFORM_WINDOWS

#include "Xna/Framework/Rectangle.hpp"
#include "Xna/CSharp/IO/Stream.hpp"
#include "Xna/CSharp/Exception.hpp"
#include "Xna/CSharp/Windows/Forms/Screen.hpp"
#include "Xna/Internal/Misc.hpp"
#include <cstdint>
#include <algorithm>

#include "Xna/Platform/Windows/WindowsPlatform.hpp"
#include "Xna/Platform/Windows/WindowsPlatformHelpers.hpp"

#include "Xna/Framework/Game/GameHost.hpp"
#include "Xna/Framework/Media/MediaPlayer.hpp"

#include "Xna/Framework/Graphics/Effect/EffectPass.hpp"
#include "Xna/Framework/Graphics/Effect/BasicEffect.hpp"
#include "Xna/Framework/Graphics/GraphicsDevice.hpp"
#include "SDL3/SDL.h"

using Microsoft::WRL::ComPtr;

namespace Xna {
	static PlatformRectangle RECTtoPRectangle(RECT const& r) {
		return { r.left, r.top, r.left, r.right };
	}

	std::string WindowsHelpers::LPWSTRToString(LPWSTR lpwstr) {
		if (!lpwstr)
			return std::string();

		// 1. Calcular o tamanho necessário para o buffer da string de destino
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, lpwstr, -1, NULL, 0, NULL, NULL);

		// 2. Criar uma string com o tamanho adequado
		std::string strTo(size_needed - 1, 0); // -1 para ignorar o terminador nulo

		// 3. Fazer a conversão real
		WideCharToMultiByte(CP_UTF8, 0, lpwstr, -1, &strTo[0], size_needed, NULL, NULL);

		return strTo;
	}

	//
	//WindowsPlatform
	//

	STDMETHODIMP MediaEngineNotify::EventNotify(DWORD event, DWORD_PTR ptr, DWORD e) {
		switch (event)
		{
		case MF_MEDIA_ENGINE_EVENT_LOADEDMETADATA: {
			MediaPlayer::OnActiveSongChanged(CSharp::EventArgs::Empty);
			break;
		}
		case MF_MEDIA_ENGINE_EVENT_CANPLAY: {
			break;
		}
		case MF_MEDIA_ENGINE_EVENT_PLAY: {
			MediaPlayer::OnMediaStateChanged(CSharp::EventArgs::Empty);
			break;
		}
		case MF_MEDIA_ENGINE_EVENT_PAUSE: {
			MediaPlayer::OnMediaStateChanged(CSharp::EventArgs::Empty);
			break;
		}
		case MF_MEDIA_ENGINE_EVENT_PURGEQUEUEDEVENTS: {
			break;
		}
		case MF_MEDIA_ENGINE_EVENT_NOTIFYSTABLESTATE: {
			break;
		}
		case MF_MEDIA_ENGINE_EVENT_ERROR: {
			break;
		}
		case MF_MEDIA_ENGINE_EVENT_ENDED:
			MediaPlayer::state = MediaState::Stopped;
			MediaPlayer::OnMediaStateChanged(CSharp::EventArgs::Empty);
			break;
		}

		return S_OK;
	}

	void WindowsPlatform::InitWIC() {
		if (WICFactory)
			return;

		const auto hresult = CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(WICFactory.GetAddressOf())
		);

		if FAILED(hresult)
			throw CSharp::InvalidOperationException("CreateDXGIFactory1 failed.");
	}

	void WindowsPlatform::InitFactory() {
		if (DXGIFactory)
			return;

		if FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)DXGIFactory.ReleaseAndGetAddressOf()))
			throw CSharp::InvalidOperationException("CreateDXGIFactory1 failed.");
	}

	Microsoft::WRL::ComPtr<IDXGISwapChain1> WindowsPlatform::CreateSwapChain1(DXGI_SWAP_CHAIN_DESC1 desc,
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc, Microsoft::WRL::ComPtr<ID3D11Device> const& device, HWND windowHandle) {
		if (!IsWindow(windowHandle))
			throw CSharp::InvalidOperationException("Invalid window handle.");

		ComPtr<IDXGIFactory2> factory2;
		auto hr = WindowsPlatform::GetDXGIFactory().As(&factory2);

		if FAILED(hr)
			throw CSharp::InvalidOperationException("DXGIFactory1 to DXGIFactory2 failed.");

		ComPtr<IDXGISwapChain1> swapChain1;

		hr = factory2->CreateSwapChainForHwnd(
			device.Get(),
			windowHandle,
			&desc,
			&fullScreenDesc,
			nullptr,
			swapChain1.ReleaseAndGetAddressOf()
		);

		if FAILED(hr)
			throw CSharp::InvalidOperationException("CreateSwapChainForHwnd() failed.");

		return swapChain1;
	}

	void WindowsPlatform::InitAudioEngine() {
		if (AudioEngine == nullptr) {
			DirectX::AUDIO_ENGINE_FLAGS eflags = DirectX::AudioEngine_Default;
#ifdef _DEBUG
			eflags |= DirectX::AudioEngine_Debug;
#endif
			AudioEngine = std::make_unique<DirectX::AudioEngine>(eflags);
		}
	}

	void WindowsPlatform::InitMediaEngine() {
		if (MediaEngine != nullptr)
			return;

		Microsoft::WRL::ComPtr<IMFAttributes> attributes;
		MFCreateAttributes(&attributes, 2);

		auto notify = Microsoft::WRL::Make<MediaEngineNotify>();

		attributes->SetUnknown(MF_MEDIA_ENGINE_CALLBACK, notify.Get());
		attributes->SetUINT32(MF_MEDIA_ENGINE_AUDIO_CATEGORY, AudioCategory_GameMedia);

		Microsoft::WRL::ComPtr<IMFMediaEngineClassFactory> factory;
		auto hr = CoCreateInstance(
			CLSID_MFMediaEngineClassFactory,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&factory));

		if FAILED(hr)
			throw CSharp::InvalidOperationException("CoCreateInstance->IMFMediaEngineClassFactory failed.");

		hr = factory->CreateInstance(
			MF_MEDIA_ENGINE_AUDIOONLY | MF_MEDIA_ENGINE_REAL_TIME_MODE,
			attributes.Get(),
			MediaEngine.ReleaseAndGetAddressOf());

		if FAILED(hr)
			throw CSharp::InvalidOperationException("CreateInstance->IMFMediaEngine* failed.");

		hr = MediaEngine->SetAutoPlay(TRUE);

		if FAILED(hr)
			throw CSharp::InvalidOperationException("MediaEngine->SetAutoPlay(TRUE) failed.");
	}

	void WindowsPlatform::Initialize() {
		//Rotines

		Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_SINGLETHREADED); //RO_INIT_MULTITHREADED
		if (FAILED(initialize))
			throw CSharp::InvalidOperationException("RoInitializeWrapper failed.");

		HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED); //COINIT_MULTITHREADED
		if (FAILED(hr))
			throw CSharp::InvalidOperationException("CoInitializeEx failed.");

		//Media
		hr = MFStartup(MF_VERSION);
		if FAILED(hr)
			throw CSharp::InvalidOperationException("MFStartup failed.");

		//Init functions
		InitWIC();
		InitFactory();
		InitAudioEngine();
		InitMediaEngine();

		//Input
		Keyboard = std::make_unique<DirectX::Keyboard>();
		Mouse = std::make_unique<DirectX::Mouse>();
		GamePad = std::make_unique<DirectX::GamePad>();
	}

	void WindowsPlatform::Dispose() {
		WICFactory = nullptr;
		DXGIFactory = nullptr;

		if (AudioEngine) {
			AudioEngine->Suspend();
			AudioEngine = nullptr;
		}

		if (MediaEngine) {
			MediaEngine->Pause();
			MediaEngine = nullptr;
		}

		if (GamePad) {
			GamePad->Suspend();
			GamePad = nullptr;
		}		

		Keyboard = nullptr;
		Mouse = nullptr;

		MFShutdown();
		CoUninitialize();
		RoUninitialize();
	}

	//
	//DxHelpers
	//

	Microsoft::WRL::ComPtr<IStream> DxHelpers::CreateIStreamFromCustomStream(CSharp::Stream& stream) {
		// --- 1) Ler conteúdo inteiro da Stream customizada ---
		size_t size = stream.Length();
		BYTE* buffer = new BYTE[size];

		// Garantir ponteiro no início
		stream.Seek(0, CSharp::SeekOrigin::Begin);

		// Ler bytes
		stream.Read(reinterpret_cast<uint8_t*>(buffer), size);


		// --- 2) Criar IStream baseado nesses bytes ---
		IStream* pStream = SHCreateMemStream(buffer, static_cast<UINT>(size));

		delete[] buffer;

		// Se falhou, retornar nullptr
		if (!pStream)
			return nullptr;

		Microsoft::WRL::ComPtr<IStream> comStream;
		comStream.Attach(pStream);

		// --- 3) Reposicionar ponteiro no início ---	
		LARGE_INTEGER li = {};
		comStream->Seek(li, STREAM_SEEK_SET, nullptr);

		return comStream;
	}

	void DxHelpers::CopyIStreamToCustomStream(IStream* iStream, CSharp::Stream& stream) {
		// 1 — Voltar ao início do IStream
		LARGE_INTEGER liZero = {};
		iStream->Seek(liZero, STREAM_SEEK_SET, nullptr);

		// 2 — Obter tamanho
		STATSTG stat = {};
		if (FAILED(iStream->Stat(&stat, STATFLAG_NONAME)))
			return;

		ULONG size = static_cast<ULONG>(stat.cbSize.QuadPart);
		if (size == 0)
			return;

		// 3 — Buffer temporário
		std::unique_ptr<BYTE[]> buffer(new BYTE[size]);

		// 4 — Ler tudo do IStream
		ULONG readBytes = 0;
		HRESULT hr = iStream->Read(buffer.get(), size, &readBytes);
		if (FAILED(hr) || readBytes != size)
			return;

		// 5 — Gravar no Stream customizado
		stream.Write(reinterpret_cast<uint8_t*>(buffer.get()), static_cast<int32_t>(size));

		// (Opcional) voltar posição do Stream customizado ao início
		stream.Seek(0, CSharp::SeekOrigin::Begin);
	}

	//
	//System
	//							

	PlatformRectangle Platform::System_MonitorArea(intptr_t monitor) {
		MONITORINFOEX info{};
		info.cbSize = sizeof(MONITORINFOEX);
		auto hmonitor = reinterpret_cast<HMONITOR>(monitor);
		GetMonitorInfo(hmonitor, &info);
		return RECTtoPRectangle(info.rcMonitor);
	}

	bool Platform::System_MonitorIsPrimary(intptr_t monitor) {
		MONITORINFOEX info{};
		info.cbSize = sizeof(MONITORINFOEX);
		auto hmonitor = reinterpret_cast<HMONITOR>(monitor);
		GetMonitorInfo(hmonitor, &info);
		return ((info.dwFlags & MONITORINFOF_PRIMARY) != 0);
	}	

	int32_t Platform::System_MonitorBitDepth(intptr_t monitor, intptr_t hdc) {
		HDC screenDC = reinterpret_cast<HDC>(hdc);

		if (screenDC == NULL) {
			MONITORINFOEX info{};
			info.cbSize = sizeof(MONITORINFOEX);
			auto hmonitor = reinterpret_cast<HMONITOR>(monitor);
			GetMonitorInfo(hmonitor, &info);

			auto _deviceName = info.szDevice;
			screenDC = CreateDCW(LPCWSTR(_deviceName), NULL, NULL, NULL);
		}
		
		int32_t bitDepth = GetDeviceCaps(screenDC, BITSPIXEL);
		bitDepth *= GetDeviceCaps(screenDC, PLANES);

		DeleteDC(screenDC);

		return bitDepth;
	}

	void Platform::System_ProcessException(std::string const& exception) {
		MessageBox(nullptr, exception.c_str(), "XN++", MB_OK);
	}

	void Platform::System_GetExecutablePath(std::filesystem::path& path) {
		wchar_t buffer[MAX_PATH]{};
		const auto length = GetModuleFileNameW(nullptr, buffer, MAX_PATH);

		if (length > 0)
			path = std::filesystem::path(buffer, buffer + length);
	}

	static BOOL CALLBACK MonitorEnumProcCallback(HMONITOR hmonitor, HDC hdc, LPRECT lprect, LPARAM lparam) {
		auto handlers = (std::vector<std::tuple<intptr_t, intptr_t>>*)lparam;
		handlers->push_back({reinterpret_cast<intptr_t>(hmonitor), reinterpret_cast<intptr_t>(hdc) });

		return true;
	}

	std::vector<std::tuple<intptr_t, intptr_t>> Platform::System_GetAllMonitorHandlers() {
		std::vector<std::tuple<intptr_t, intptr_t>> list;
		EnumDisplayMonitors(NULL, NULL, MonitorEnumProcCallback, (LPARAM)&list);

		return list;
	}

	void Platform::Initialize() {
		WindowsPlatform::Initialize();

		//TODO: [!] remover isso aqui
		if (!(SDL_Init(SDL_INIT_VIDEO))) {
			// Falha ao inicializar: SDL_GetPrimaryDisplay retornará 0
			SDL_Log("Erro: %s", SDL_GetError());
		}
	}

	void Platform::Dispose() {
		WindowsPlatform::Dispose();
	}

	void Platform::Update() {
		WindowsPlatform::Update();
	}

	void Platform::Suspend() {
		WindowsPlatform::Suspend();
	}

	void Platform::Resume() {
		WindowsPlatform::Resume();
	}

	size_t Platform::MaxSamplerStates = static_cast<size_t>(D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT);

	//
	// GameHost
	//

	void Platform::GameHost_Tick(GameHost& gh) {
		MSG msg{};

		do {
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				if (gh.impl->exitRequested)
					gh.impl->gameWindow.Close();
				else
					gh.RunOneFrame();
			}

		} while (msg.message != WM_QUIT);
	}

	//
	// Effect
	//

	void Platform::Effect_EffectPass_Apply(EffectPass const& p) {
		//TODO: p.impl->platformImpl.iEffect->Apply();
	}

	void Platform::Effect_BasicEffect_Create(BasicEffect const& e, GraphicsDevice const& d) {
		auto& device = d.implGraphicsDevice->platformImpl.device;

		e.impl->platformImpl.basicEffect = std::make_unique<DirectX::BasicEffect>(device.Get());
	}



	void Platform::Effect_BasicEffect_SetValues(BasicEffect const& e) {
		auto& effect = e.impl->platformImpl.basicEffect;

		const auto world = DxHelpers::MatrixToDx(e.World());
		effect->SetWorld(world);
		const auto view = DxHelpers::MatrixToDx(e.View());
		effect->SetView(world);
		const auto projection = DxHelpers::MatrixToDx(e.Projection());
		effect->SetProjection(world);

		const auto ldir0 = e.DirectionalLight0().Direction();
		DirectX::XMFLOAT3 dir0{ ldir0.X, ldir0.Y, ldir0.Z };
		effect->SetLightDirection(0, DirectX::XMLoadFloat3(&dir0)); //TODO: começa do 0?

		const auto ldir1 = e.DirectionalLight1().Direction();
		DirectX::XMFLOAT3 dir1{ ldir1.X, ldir1.Y, ldir1.Z };
		effect->SetLightDirection(1, DirectX::XMLoadFloat3(&dir1));

		const auto ldir2 = e.DirectionalLight2().Direction();
		DirectX::XMFLOAT3 dir2{ ldir2.X, ldir2.Y, ldir2.Z };
		effect->SetLightDirection(2, DirectX::XMLoadFloat3(&dir2));

		const auto alColor = e.AmbientLightColor();
		DirectX::XMFLOAT3 ambColor{ alColor.X, alColor.Y, alColor.Z };
		effect->SetAmbientLightColor(DirectX::XMLoadFloat3(&ambColor));

		effect->SetLightEnabled(0, e.LightingEnabled());
		effect->SetLightEnabled(1, e.LightingEnabled());
		effect->SetLightEnabled(2, e.LightingEnabled());

		if (e.LightingEnabled())
			effect->EnableDefaultLighting();

		effect->SetFogEnabled(e.FogEnabled());
		effect->SetFogStart(e.FogStart());
		effect->SetFogEnd(e.FogEnd());

		const auto difColor = e.DiffuseColor();
		DirectX::XMFLOAT3 diffuseColor{ difColor.X, difColor.Y, difColor.Z };
		effect->SetDiffuseColor(DirectX::XMLoadFloat3(&diffuseColor));

		const auto emColor = e.EmissiveColor();
		DirectX::XMFLOAT3 emissiveColor{ emColor.X, emColor.Y, emColor.Z };
		effect->SetEmissiveColor(DirectX::XMLoadFloat3(&emissiveColor));

		const auto espColor = e.SpecularColor();
		DirectX::XMFLOAT3 specularColor{ espColor.X, espColor.Y, espColor.Z };
		effect->SetSpecularColor(DirectX::XMLoadFloat3(&specularColor));

		effect->SetSpecularPower(e.SpecularPower());
		effect->SetAlpha(e.Alpha());
		effect->SetPerPixelLighting(e.PreferPerPixelLighting());

		effect->SetTextureEnabled(e.TextureEnabled());

		if (e.TextureEnabled() && e.Texture().has_value()) {
			auto& shader = e.Texture()->impl->platformImpl.shaderResource;
			effect->SetTexture(shader.Get());
		}

		effect->SetVertexColorEnabled(e.VertexColorEnabled());
	}

	void Platform::Effect_BasicEffect_Apply(BasicEffect const& e) {
		auto device = e.GetGraphicsDevice();
		auto& effect = e.impl->platformImpl.basicEffect;
		effect->Apply(device->implGraphicsDevice->platformImpl.context.Get());
	}
}


#endif