#ifndef XNA_PLATFORM_WINDOWS_WINDOWSPLATFORM_HPP
#define XNA_PLATFORM_WINDOWS_WINDOWSPLATFORM_HPP

#include "Xna/Platform/_Platform.hpp"

#ifdef PLATFORM_WINDOWS

#include "WindowsHeaders.hpp"
#include "WindowsPlatformHelpers.hpp"
#include <vector>
#include <memory>

namespace Xna {
	//
	// IMPLS
	//
	struct KeyboardImpl{};
	struct MouseImpl{};
	struct GamePadImpl{};

	struct PlatformImpl::BlendStateImpl {
		Microsoft::WRL::ComPtr<ID3D11BlendState> blendState{ nullptr };
		static constexpr int MAX_RENDER_TARGETS = 8;
	};

	struct PlatformImpl::DepthStencilStateImpl {
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencil;
	};

	struct PlatformImpl::RasterizerStateImpl {
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	};

	struct PlatformImpl::SamplerStateImpl {
		Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	};

	struct PlatformImpl::GraphicsAdapterImpl {
		Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter;
		Microsoft::WRL::ComPtr<IDXGIFactory1> factory;
	};

	struct PlatformImpl::GraphicsDeviceImpl {
		Microsoft::WRL::ComPtr<ID3D11Device> device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
		Microsoft::WRL::ComPtr<IDXGIFactory1> factory;
		Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;
		DXGI_SWAP_CHAIN_DESC1 lazySwapChainDesc{};
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC lazySwapChainFullScreenDesc{};

		static constexpr int featureLevelCount = 7;

		inline static constexpr D3D_FEATURE_LEVEL featureLevels[featureLevelCount] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
		};

		D3D_FEATURE_LEVEL currentFeatureLevel{ D3D_FEATURE_LEVEL_11_1 };
	};

	struct PlatformImpl::SpriteBatchImpl {
		std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	};

	struct PlatformImpl::SpriteFontImpl {
		std::unique_ptr<DirectX::SpriteFont> spriteFont;
	};

	struct PlatformImpl::Texture2DImpl {
		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> stagingTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResource;
	};

	struct PlatformImpl::Texture3DImpl {
		Microsoft::WRL::ComPtr<ID3D11Texture3D> texture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResource;
		Microsoft::WRL::ComPtr<ID3D11Texture3D> stagingTexture;
	};

	struct PlatformImpl::RenderTarget2DImpl {
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilTexture;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	};

	struct PlatformImpl::GameWindowImpl {
		enum class GameWindowMode : UINT {
			Fullscreen = WS_POPUP | WS_VISIBLE,
			Windowed = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE,
			Borderless = WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE,
		};

		HINSTANCE instance{ nullptr };
		HWND handle{ nullptr };
		int width{ 800 };
		int	height{ 480 };
		HICON icon{ LoadIcon(NULL, IDI_APPLICATION) };
		HCURSOR cursor{ LoadCursor(NULL, IDC_ARROW) };
		COLORREF color{ RGB(0,0,0) };
		DWORD style{ static_cast<DWORD>(GameWindowMode::Windowed) };
		int positionX{ 0 };
		int	positionY{ 0 };
		float centerX{ 0 };
		float centerY{ 0 };
	};

	struct PlatformImpl::SoundEffectImpl {
		std::unique_ptr<DirectX::SoundEffect> soundEffect = nullptr;
		WAVEFORMATEX currentWaveFormatex{};
	};

	struct PlatformImpl::SoundEffectInstanceImpl {
		std::unique_ptr<DirectX::SoundEffectInstance> soundEffectInstance = nullptr;
	};

	struct PlatformImpl::DynamicSoundEffectInstanceImpl {
		std::unique_ptr<DirectX::DynamicSoundEffectInstance> dynamicSoundEffectInstance = nullptr;
	};

	struct PlatformImpl::SongImpl {
		//TODO: Nada a fazer
	};

	struct PlatformImpl::EffectPassImpl {
		//TODO: nada a fazer?
	};

	struct PlatformImpl::EffectTechniqueImpl {
		//TODO: nada a fazer?
	};

	struct PlatformImpl::EffectAnnotationImpl {
		//TODO: nada a fazer?
	};

	struct PlatformImpl::EffectImpl {
		//TODO: nada a fazer?
	};

	struct PlatformImpl::BasicEffectImpl {
		std::unique_ptr<DirectX::BasicEffect> basicEffect;
	};

	struct PlatformImpl::VertexBufferImpl {
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	};

	struct PlatformImpl::DynamicVertexBufferImpl {
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	};

	struct Platform::InputProcessMessage {
		UINT msg;
		WPARAM wParam;
		LPARAM lParam;
	};

	//
	// WINDOWS PLATFORM
	//

	class MediaEngineNotify final : public Microsoft::WRL::RuntimeClass<Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>, IMFMediaEngineNotify>
	{
	public:
		STDMETHODIMP EventNotify(
			DWORD event,
			DWORD_PTR ptr,
			DWORD e) override;
	};

	struct WindowsPlatform {
		static inline Microsoft::WRL::ComPtr<IWICImagingFactory> GetWICFactory() {
			InitWIC();
			return WICFactory;
		}

		static inline Microsoft::WRL::ComPtr<IDXGIFactory1> GetDXGIFactory() {
			InitFactory();
			return DXGIFactory;
		}

		static inline DirectX::AudioEngine& GetAudioEngine() {
			InitAudioEngine();
			return *AudioEngine;
		}

		static inline  Microsoft::WRL::ComPtr<IMFMediaEngine> GetMediaEngine() {
			InitMediaEngine();
			return MediaEngine;
		}

		static inline DirectX::GamePad& GetGamePad() { return *GamePad; }
		static inline DirectX::Keyboard& GetKeyboard() { return *Keyboard; }
		static inline DirectX::Mouse& GetMouse() { return *Mouse; }

		static void Initialize();

		static inline void Update() {
			GetAudioEngine().Update();
		}

		static inline void Resume() {
			GetAudioEngine().Resume();
		}

		static inline void Suspend() {
			GetAudioEngine().Suspend();
		}

		static constexpr DXGI_SWAP_CHAIN_DESC1 GetDefaultSwapChainDesc1() {
			DXGI_SWAP_CHAIN_DESC1 sd = {};
			sd.Width = 0;
			sd.Height = 0;
			sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
			sd.BufferCount = 2;
			sd.Scaling = DXGI_SCALING_NONE;
			sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			sd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

			return sd;
		}

		static void Dispose();

		static Microsoft::WRL::ComPtr<IDXGISwapChain1> CreateSwapChain1(DXGI_SWAP_CHAIN_DESC1 desc, DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc, Microsoft::WRL::ComPtr<ID3D11Device> const& device, HWND windowHandle);

	private:
		static inline Microsoft::WRL::ComPtr<IWICImagingFactory> WICFactory = nullptr;
		static inline Microsoft::WRL::ComPtr<IDXGIFactory1> DXGIFactory = nullptr;
		//Audio
		static inline std::unique_ptr<DirectX::AudioEngine> AudioEngine = nullptr;
		//Media
		static inline Microsoft::WRL::ComPtr<IMFMediaEngine> MediaEngine = nullptr;
		//static inline Microsoft::WRL::ComPtr<MediaEngineNotify> MediaEngineNotify = nullptr;
		//Input
		static inline std::unique_ptr<DirectX::GamePad> GamePad = nullptr;
		static inline std::unique_ptr<DirectX::Keyboard> Keyboard = nullptr;
		static inline std::unique_ptr<DirectX::Mouse> Mouse = nullptr;

		static void InitWIC();
		static void InitFactory();
		static void InitAudioEngine();
		static void InitMediaEngine();
	};
}

#endif
#endif
