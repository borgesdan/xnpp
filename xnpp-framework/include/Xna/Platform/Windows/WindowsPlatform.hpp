#ifndef XNA_PLATFORM_WINDOWS_WINDOWSPLATFORM_HPP
#define XNA_PLATFORM_WINDOWS_WINDOWSPLATFORM_HPP

#include "Xna/Platform/_Platform.hpp"

#ifdef PLATFORM_WINDOWS

#include "WindowsHeaders.hpp"
#include "WindowsPlatformHelpers.hpp"
#include <vector>
#include <memory>
#include <SDL3/SDL.h>

namespace Xna {
	//
	// IMPLS
	//

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

	struct PlatformImpl::BasicEffectImpl {
		std::unique_ptr<DirectX::BasicEffect> basicEffect;
	};

	struct PlatformImpl::VertexBufferImpl {
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	};

	struct PlatformImpl::DynamicVertexBufferImpl {
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	};

	static WPARAM ProcessKeyboard(SDL_Event const& event) {
		WPARAM wParam{ 0 };

		switch (event.key.key) {
			// Setas e Navegação
		case SDLK_LEFT:     wParam = VK_LEFT;     break;
		case SDLK_RIGHT:    wParam = VK_RIGHT;    break;
		case SDLK_UP:       wParam = VK_UP;       break;
		case SDLK_DOWN:     wParam = VK_DOWN;     break;
		case SDLK_HOME:     wParam = VK_HOME;     break;
		case SDLK_END:      wParam = VK_END;      break;
		case SDLK_PAGEUP:   wParam = VK_PRIOR;    break;
		case SDLK_PAGEDOWN: wParam = VK_NEXT;     break;

			// Teclas de Controle
		case SDLK_ESCAPE:   wParam = VK_ESCAPE;   break;
		case SDLK_RETURN:   wParam = VK_RETURN;   break;
		case SDLK_SPACE:    wParam = VK_SPACE;    break;
		case SDLK_BACKSPACE:wParam = VK_BACK;     break;
		case SDLK_TAB:      wParam = VK_TAB;      break;
		case SDLK_DELETE:   wParam = VK_DELETE;   break;
		case SDLK_INSERT:   wParam = VK_INSERT;   break;

			// Modificadores
		case SDLK_LSHIFT:
		case SDLK_RSHIFT:   wParam = VK_SHIFT;    break;
		case SDLK_LCTRL:
		case SDLK_RCTRL:    wParam = VK_CONTROL;  break;
		case SDLK_LALT:
		case SDLK_RALT:     wParam = VK_MENU;     break;

			// Teclas de Função
		case SDLK_F1:  wParam = VK_F1;  break;
		case SDLK_F2:  wParam = VK_F2;  break;
		case SDLK_F3:  wParam = VK_F3;  break;
		case SDLK_F4:  wParam = VK_F4;  break;
		case SDLK_F5:  wParam = VK_F5;  break;
		case SDLK_F6:  wParam = VK_F6;  break;
		case SDLK_F7:  wParam = VK_F7;  break;
		case SDLK_F8:  wParam = VK_F8;  break;
		case SDLK_F9:  wParam = VK_F9;  break;
		case SDLK_F10: wParam = VK_F10; break;
		case SDLK_F11: wParam = VK_F11; break;
		case SDLK_F12: wParam = VK_F12; break;

		default:
			// Para caracteres ASCII (A-Z, 0-9)
			if (event.key.key >= 32 && event.key.key <= 126) {
				wParam = static_cast<WPARAM>(event.key.key);
				// No Windows, VK_A até VK_Z são sempre maiúsculos ('A' = 0x41)
				if (wParam >= 'a' && wParam <= 'z') {
					wParam -= 32;
				}
			}
			else {
				// Se cair aqui, a tecla não foi mapeada (ex: Volume, Media keys)
				wParam = 0;
			}
			break;
		}

		return wParam;
	}

	struct Platform::InputProcessMessage {
		UINT msg{0};
		WPARAM wParam{0};
		LPARAM lParam{0};

		InputProcessMessage() = default;
		
		inline InputProcessMessage(SDL_Event const& event) {
			if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
				msg = (event.type == SDL_EVENT_KEY_DOWN) ? WM_KEYDOWN : WM_KEYUP;
				wParam = ProcessKeyboard(event);
				
				// Reconstruindo o lParam bit a bit:
				uint32_t repeatCount = 1;
				uint32_t scanCode = static_cast<uint32_t>(event.key.key) & 0xFF;
				uint32_t extended = 0;
				uint32_t prevState = event.key.repeat ? 1 : 0;
				uint32_t transition = event.type == SDL_EVENT_KEY_UP ? 1 : 0;

				lParam = (repeatCount) // Bits 0-15
					| (scanCode << 16) // Bits 16-23
					| (extended << 24) // Bit 24
					| (prevState << 30) // Bit 30
					| (transition << 31); // Bit 31

				return;
			}

			switch (event.type)
			{
			case SDL_EVENT_MOUSE_MOTION:
				msg = WM_MOUSEMOVE;				
				if (event.motion.state & SDL_BUTTON_LMASK) wParam |= MK_LBUTTON;
				if (event.motion.state & SDL_BUTTON_RMASK) wParam |= MK_RBUTTON;
				if (event.motion.state & SDL_BUTTON_MMASK) wParam |= MK_MBUTTON;				
				lParam = MAKELPARAM((int)event.motion.x, (int)event.motion.y);
				break;
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
			case SDL_EVENT_MOUSE_BUTTON_UP: {
				bool down = (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN);
				lParam = MAKELPARAM((int)event.button.x, (int)event.button.y);

				if (event.button.button == SDL_BUTTON_LEFT) {
					msg = down ? WM_LBUTTONDOWN : WM_LBUTTONUP;
					wParam = MK_LBUTTON;
				}
				else if (event.button.button == SDL_BUTTON_RIGHT) {
					msg = down ? WM_RBUTTONDOWN : WM_RBUTTONUP;
					wParam = MK_RBUTTON;
				}
				else if (event.button.button == SDL_BUTTON_MIDDLE) {
					msg = down ? WM_MBUTTONDOWN : WM_MBUTTONUP;
					wParam = MK_MBUTTON;
				}
				else if (event.button.button == SDL_BUTTON_X1 || event.button.button == SDL_BUTTON_X2) {
					msg = down ? WM_XBUTTONDOWN : WM_XBUTTONUP;
					// XBUTTON usa o HIWORD do wParam para identificar qual botão (1 ou 2)
					WORD xbtn = (event.button.button == SDL_BUTTON_X1) ? XBUTTON1 : XBUTTON2;
					wParam = MAKEWPARAM(0, xbtn);
				}
				break;
			}
			case SDL_EVENT_MOUSE_WHEEL: { //TODO: [!] Mouse wheel retorna 0 no directxtk, mas vamos desconsiderar por enquanto
				msg = WM_MOUSEWHEEL;				
				short wheelDelta = static_cast<short>(event.wheel.y * 120); //Windows 120 (WHEEL_DELTA).
				wParam = MAKEWPARAM(0, wheelDelta);				
				float mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				lParam = MAKELPARAM((int)mouseX, (int)mouseY);
				break;
			}
			default:
				break;
			}
		}
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
