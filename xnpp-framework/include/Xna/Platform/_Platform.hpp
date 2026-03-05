#ifndef XNA_PLATFORM__PLATFORM_HPP
#define XNA_PLATFORM__PLATFORM_HPP

#include <string>
#include <vector>
#include <cstdint>
#include <optional>
#include <exception>
#include <filesystem>
#include <tuple>
#include <functional>
#include "Xna/CSharp/IO/Stream.hpp"

//OS
#if defined(_WIN32)
#define PLATFORM_WINDOWS 1
#elif defined(__APPLE__) && defined(__MACH__)
#define PLATFORM_MACOS 1
#elif defined(__ANDROID__)
#define PLATFORM_ANDROID 1
#elif defined(__linux__)
#define PLATFORM_LINUX 1
#elif defined(__unix__)
#define PLATFORM_UNIX 1
#else
#error "Sistema operacional não suportado"
#endif
#ifndef PLATFORM_WINDOWS
#define PLATFORM_WINDOWS 0
#endif
#ifndef PLATFORM_MACOS
#define PLATFORM_MACOS 0
#endif
#ifndef PLATFORM_LINUX
#define PLATFORM_LINUX 0
#endif
#ifndef PLATFORM_ANDROID
#define PLATFORM_ANDROID 0
#endif
#ifndef PLATFORM_UNIX
#define PLATFORM_UNIX 0
#endif

//Architecture
#if defined(_M_X64) || defined(__x86_64__)
#define ARCH_X64 1
#elif defined(_M_IX86) || defined(__i386__)
#define ARCH_X86 1
#elif defined(_M_ARM64) || defined(__aarch64__)
#define ARCH_ARM64 1
#elif defined(_M_ARM) || defined(__arm__)
#define ARCH_ARM 1
#else
#error "Arquitetura não suportada"
#endif

#ifndef ARCH_X64
#define ARCH_X64 0
#endif
#ifndef ARCH_X86
#define ARCH_X86 0
#endif
#ifndef ARCH_ARM64
#define ARCH_ARM64 0
#endif
#ifndef ARCH_ARM
#define ARCH_ARM 0
#endif

//Compiler

#if defined(_MSC_VER)
#define COMPILER_MSVC 1
#elif defined(__clang__)
#define COMPILER_CLANG 1
#elif defined(__GNUC__)
#define COMPILER_GCC 1
#else
#error "Compilador não suportado"
#endif

#ifndef COMPILER_MSVC
#define COMPILER_MSVC 0
#endif
#ifndef COMPILER_CLANG
#define COMPILER_CLANG 0
#endif
#ifndef COMPILER_GCC
#define COMPILER_GCC 0
#endif

//Endianess
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define PLATFORM_BIG_ENDIAN 1
#define PLATFORM_LITTLE_ENDIAN 0
#else
#define PLATFORM_BIG_ENDIAN 0
#define PLATFORM_LITTLE_ENDIAN 1
#endif

//Util
#if PLATFORM_WINDOWS
#define FORCE_INLINE __forceinline
#elif COMPILER_GCC || COMPILER_CLANG
#define FORCE_INLINE inline __attribute__((always_inline))
#else
#define FORCE_INLINE inline
#endif

//MAIN
#if defined(PLATFORM_WINDOWS)
#define MAIN_FUNC APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
#endif

namespace Xna {
	namespace CSharp {
		class Screen;
	}

	class BlendState;
	class DepthStencilState;
	class RasterizerState;
	class SamplerState;
	class GraphicsAdapter;
	class GraphicsDevice;
	class RenderTarget2D;
	class Texture2D;
	class Texture3D;
	class SpriteBatch;
	class SpriteFont;
	class Effect;
	class GameWindow;
	class GameHost;
	class SoundEffect;
	class SoundEffectInstance;
	class AudioListener;
	class AudioEmitter;
	class DynamicSoundEffectInstance;
	class Song;
	class Effect;
	class EffectPass;
	class EffectTechnique;
	class EffectAnnotation;
	class BasicEffect;
	class VertexBuffer;
	class DynamicVertexBuffer;

	struct Color;
	struct DisplayMode;
	struct DisplayModeCollection;
	struct Rectangle;
	struct PresentationParameters;
	struct Viewport;
	struct Matrix;
	struct Vector2;
	struct Vector3;
	struct KeyboardState;
	struct MouseState;
	struct GamePadState;
	struct GamePadCapabilities;

	enum class DepthFormat;
	enum class GraphicsProfile;
	enum class SurfaceFormat;
	enum class RenderTargetUsage;
	enum class SamplerStateApplyType;
	enum class SpriteSortMode;
	enum class SpriteEffects;
	enum class PlayerIndex;
	enum class GamePadDeadZone;
	enum class SoundState;
	enum class SetDataOptions;
	enum class ClearOptions;

	struct PlatformImpl {
		struct BlendStateImpl;
		struct DepthStencilStateImpl;
		struct GraphicsAdapterImpl;
		struct GraphicsDeviceImpl;
		struct Texture2DImpl;
		struct Texture3DImpl;
		struct RenderTarget2DImpl;
		struct RasterizerStateImpl;
		struct SamplerStateImpl;
		struct SpriteBatchImpl;
		struct SpriteFontImpl;
		struct GameWindowImpl;
		struct SoundEffectImpl;
		struct SoundEffectInstanceImpl;
		struct DynamicSoundEffectInstanceImpl;
		struct VertexBufferImpl;
		struct DynamicVertexBufferImpl;
		struct BasicEffectImpl;
	};

	struct PlatformRectangle {
		int Left{ 0 };
		int Top{ 0 };
		int Right{ 0 };
		int Bottom{ 0 };

		constexpr PlatformRectangle() = default;
		constexpr PlatformRectangle(int x, int y, int right, int bottom)
			:Left(x), Top(y), Right(right), Bottom(bottom) {
		}

		constexpr int X() const noexcept { return Left; }
		constexpr int Y() const noexcept { return Top; }
		constexpr int W() const noexcept { return Right - Left; }
		constexpr int H() const noexcept { return Bottom - Top; }
	};

	struct PlatformSize {
		int Width{ 0 };
		int Height{ 0 };

		constexpr PlatformSize() = default;
		constexpr PlatformSize(int w, int h) :Width(w), Height(h) {}
	};

	struct Value2 {
		intptr_t One{ 0 };
		intptr_t Two{ 0 };

		constexpr Value2() = default;
		constexpr Value2(intptr_t one, intptr_t two)
			: One(one), Two(two) {
		}
	};

	struct Platform {

		struct InputProcessMessage;
		static size_t MaxSamplerStates;

		//Platform

		XNPP_API static void Initialize();
		XNPP_API static void Dispose();
		XNPP_API static void Update();
		XNPP_API static void Suspend();
		XNPP_API static void Resume();

		//System		

		XNPP_API static PlatformRectangle System_ClientRect(intptr_t hwnd);
		XNPP_API static PlatformRectangle System_WindowRect(intptr_t hwnd);
		XNPP_API static size_t System_GetClockCounter();
		XNPP_API static size_t System_GetClockFrequency();
		XNPP_API static bool System_MultiMonitorSupport();
		XNPP_API static PlatformRectangle System_VirtualScreen();
		XNPP_API static PlatformSize System_PrimaryMonitorSize();
		XNPP_API static PlatformRectangle System_WorkingArea();
		XNPP_API static PlatformRectangle System_MonitorWorkingArea(intptr_t hMonitor);
		XNPP_API static intptr_t System_MonitorFromHandle(intptr_t hwnd);
		XNPP_API static intptr_t System_MonitorFromRect(int32_t left, int32_t top, int32_t right, int32_t bottom);
		XNPP_API static intptr_t System_MonitorFromPoint(int32_t x, int32_t y);
		XNPP_API static std::string System_MonitorDeviceName(intptr_t monitor);
		XNPP_API static PlatformRectangle System_MonitorArea(intptr_t monitor);
		XNPP_API static bool System_MonitorIsPrimary(intptr_t monitor);
		XNPP_API static int32_t System_MonitorBitDepth(intptr_t monitor, intptr_t hdc);
		XNPP_API static std::vector<Value2> System_GetAllMonitorHandlers();

		//platform dependent
		XNPP_API static void System_ProcessException(std::string const& exception);
		XNPP_API static void System_GetExecutablePath(std::filesystem::path& path);

		//GameWindow

		XNPP_API static void GameWindow_Create(GameWindow const& gw);
		XNPP_API static void GameWindow_Update(GameWindow const& gw);
		XNPP_API static void GameWindow_Close(GameWindow const& gw);
		XNPP_API static bool GameWindow_WindowIsMinimized(GameWindow const& gw);
		XNPP_API static void GameWindow_MinimizeWindow(GameWindow const& gw, bool value);
		XNPP_API static void GameWindow_SetMouseVisible(GameWindow const& gw, bool value);
		XNPP_API static void GameWindow_AllowUserResizing(GameWindow const& gw, bool value);
		XNPP_API static Rectangle GameWindow_ClientBounds(GameWindow const& gw);
		XNPP_API static void GameWindow_SetTitle(GameWindow const& gw, std::string const& title);

		//GameHost

		XNPP_API static void GameHost_Tick(GameHost& gh);

		//Input
		XNPP_API static KeyboardState Keyboard_GetState();

		XNPP_API static MouseState Mouse_GetState();
		XNPP_API static intptr_t Mouse_GetWindowHandle();
		XNPP_API static void Mouse_SetWindowHandle(intptr_t value);
		XNPP_API static void Mouse_SetPosition(int32_t x, int32_t y);

		XNPP_API static GamePadState GamePad_GetState(PlayerIndex index, GamePadDeadZone deadZone);
		XNPP_API static GamePadCapabilities GamePad_GetCapabilities(PlayerIndex index);
		XNPP_API static bool GamePad_SetVibration(PlayerIndex index, float leftMotor, float rightMotor, float leftTrigger, float rightTrigger);
		XNPP_API static void GamePad_Suspend();
		XNPP_API static void GamePad_Resume();

		//GraphicsAdapter

		XNPP_API static std::vector<std::optional<GraphicsAdapter>> GraphicsAdapter_GetAllGraphicsAdapters();
		XNPP_API static void GraphicsAdapter_SupportedDisplayModes(GraphicsAdapter const& adapter);
		XNPP_API static void GraphicsAdapter_CurrentDisplayMode(GraphicsAdapter const& adapter);
		XNPP_API static bool GraphicsAdapter_IsProfileSupported(GraphicsAdapter const& adapter, GraphicsProfile graphicsProfile);
		XNPP_API static bool GraphicsAdapter_QueryBackBufferFormat(GraphicsAdapter const& adapter, GraphicsProfile graphicsProfile, SurfaceFormat format,
			DepthFormat depthFormat, int32_t multiSampleCount, SurfaceFormat& selectedFormat, DepthFormat& selectedDepthFormat, int32_t& selectedMultiSampleCount);
		XNPP_API static bool GraphicsAdapter_QueryRenderTargetFormat(GraphicsAdapter const& adapter, GraphicsProfile graphicsProfile, SurfaceFormat format,
			DepthFormat depthFormat, int32_t multiSampleCount, SurfaceFormat& selectedFormat, DepthFormat& selectedDepthFormat, int32_t& selectedMultiSampleCount);

		//SpriteBatch

		XNPP_API static void SpriteBatch_InitializeSpriteBatch(SpriteBatch const& sb, GraphicsDevice const& device);
		XNPP_API static void SpriteBatch_Begin(SpriteBatch const& sb, SpriteSortMode sortMode, std::optional<BlendState> const& blendState, std::optional<SamplerState> const& samplerState,
			std::optional<DepthStencilState> const& depthStencilState, std::optional<RasterizerState> const& rasterizerState, std::optional<Effect> const& effect, std::optional<Matrix> transformMatrix);
		XNPP_API static void SpriteBatch_End(SpriteBatch const& sb);
		XNPP_API static void SpriteBatch_Draw(SpriteBatch const& sb, Texture2D const& texture, Vector2 const& position, std::optional<Rectangle> const& sourceRectangle, Color const& color,
			float rotation, Vector2 const& origin, Vector2 const& scale, SpriteEffects effects, float layerDepth);
		XNPP_API static void SpriteBatch_Draw(SpriteBatch const& sb, Texture2D const& texture, Rectangle const& destinationRectangle, std::optional<Rectangle> const& sourceRectangle, Color const& color,
			float rotation, Vector2 const& origin, SpriteEffects effects, float layerDepth);
		XNPP_API static void SpriteBatch_DrawString(SpriteBatch const& sb, SpriteFont const& sf, std::string const& text, Vector2 const& position, Color const& color, float rotation, Vector2 const& origin,
			Vector2 const& scale, SpriteEffects effects, float layerDepth);

		//SpriteFont

		XNPP_API static void SpriteFont_Create(SpriteFont const& sf, Texture2D const& texture,
			std::vector<Rectangle> const& glyphs, std::vector<Rectangle> const& cropping,
			std::vector<char32_t> const& charMap, int32_t lineSpacing, float spacing,
			std::vector<Vector3> const& kerning, std::optional<char32_t> const& defaultCharacter);
		XNPP_API static Vector2 SpriteFont_MeasureString(SpriteFont const& sf, std::string const& text, bool ignoreWhiteSpace);
		XNPP_API static Vector2 SpriteFont_MeasureString(SpriteFont const& sf, std::wstring const& text, bool ignoreWhiteSpace);

		//Effects

		XNPP_API static void Effect_EffectPass_Apply(EffectPass const& p);
		XNPP_API static void Effect_BasicEffect_Create(BasicEffect const& e, GraphicsDevice const& d);
		XNPP_API static void Effect_BasicEffect_SetValues(BasicEffect const& e);
		XNPP_API static void Effect_BasicEffect_Apply(BasicEffect const& e);

		//Buffer

		XNPP_API static void VertexBuffer_Create(VertexBuffer const b);
		XNPP_API static void VertexBuffer_SetData(VertexBuffer const& b,
			size_t offsetInBytes,
			const void* data,
			size_t sizeOfData,
			size_t startIndex,
			size_t elementCount,
			size_t vertexStride,
			std::optional<SetDataOptions> options);
		XNPP_API static void VertexBuffer_GetData(VertexBuffer const& b,
			size_t offsetInBytes,
			void* data,
			size_t sizeOfData,
			size_t startIndex,
			size_t elementCount,
			size_t vertexStride
		);
	};

	namespace PlatformNS {
		XNPP_API size_t Graphics_GetMaxSamplerStates();

		enum class MediaState {
			Playing,
			Paused,
			Stoped,
		};

		struct MasterAudio {
			XNPP_API static void SetMasterVolume(float value);
		};

		struct ISoundEffect {
			virtual ~ISoundEffect() = default;

			XNPP_API virtual void Load(std::vector<uint8_t> const& format, std::vector<uint8_t> const& data, size_t offset, size_t count, size_t loopStart, size_t loopLength) = 0;

			XNPP_API static std::unique_ptr<ISoundEffect> Create();
		};

		struct ISoundEffectInstance {
			virtual ~ISoundEffectInstance() = default;
			XNPP_API virtual void Load(ISoundEffect* baseSE) = 0;
			XNPP_API virtual void SetVolume(float value) = 0;
			XNPP_API virtual void SetPan(float value) = 0;
			XNPP_API virtual void SetPitch(float value) = 0;
			XNPP_API virtual void IsLooped(bool value) = 0;
			XNPP_API virtual void Play() = 0;
			XNPP_API virtual void Pause() = 0;
			XNPP_API virtual void Stop() = 0;
			XNPP_API virtual MediaState GetState() = 0;

			XNPP_API static std::unique_ptr<ISoundEffectInstance> Create();
		};

		struct IMediaPlayer {
			XNPP_API virtual void Play(std::filesystem::path const& song) = 0;
			XNPP_API virtual void Pause() = 0;
			XNPP_API virtual void Resume() = 0;
			XNPP_API virtual void Stop() = 0;
			XNPP_API virtual void SetVolume(float volume) = 0;
			XNPP_API virtual void SetMuted(bool value) = 0;
			XNPP_API virtual void SetIsRepeating(bool value) = 0;
			XNPP_API virtual double GetPlayPosition() = 0; //Em milisegundos

			XNPP_API virtual void SongChanged(std::function<void()> const& func) = 0;
			XNPP_API virtual void MediaStateChanged(std::function<void()> const& func) = 0;

			XNPP_API virtual ~IMediaPlayer() = default;
			XNPP_API static IMediaPlayer& GetInstance();
		};

		struct IGraphicsDevice {
			XNPP_API virtual ~IGraphicsDevice() = default;

			XNPP_API virtual void CreateDevice(GraphicsAdapter const& adapter, Xna::PresentationParameters const& presentationParameters) = 0;
			XNPP_API virtual void Present(std::optional<Rectangle> const& rec, std::optional<Rectangle> const& destination, intptr_t overrideWindowHandle) = 0;
			XNPP_API virtual void SetViewport(Viewport const& viewport) = 0;
			XNPP_API virtual void MakeWindowAssociation(PresentationParameters const& pp) = 0;
			XNPP_API virtual void Reset(Xna::PresentationParameters const& presentationParameters, GraphicsAdapter const& graphicsAdapter) = 0;
			XNPP_API virtual void LazyInitialization(intptr_t windowHandle) = 0;
			XNPP_API virtual void ApplyBlendState(BlendState const& blend) = 0;
			XNPP_API virtual void ApplyDepthStencilState(DepthStencilState const& depth) = 0;
			XNPP_API virtual void ApplyRasterizerState(RasterizerState const& rasterizer) = 0;
			XNPP_API virtual void Clear(ClearOptions options, Color const& color, float depth, int32_t stencil) = 0;

			XNPP_API static std::unique_ptr<IGraphicsDevice> Create();
		};

		struct ITexture2D {
			XNPP_API virtual ~ITexture2D() = default;
			XNPP_API virtual void Initialize(size_t width, size_t height, bool mipMap, SurfaceFormat format) = 0;
			XNPP_API virtual void SetData(size_t level, std::optional<Rectangle> const& rect, const void* data,
				size_t startIndex, size_t elementCount, bool hasMipMap, size_t sizeOfData) = 0;
			XNPP_API virtual void GetData(size_t level, std::optional<Rectangle> const& rect, void* data,
				size_t startIndex, size_t elementCount, size_t sizeOfData) = 0;

			XNPP_API static std::unique_ptr<ITexture2D> Create();
		};

		struct ISpriteBatch {
			XNPP_API virtual ~ISpriteBatch() = default;
			XNPP_API virtual void Begin(
				SpriteSortMode sortMode,
				const BlendState* blendState,
				const SamplerState* samplerState,
				const DepthStencilState* depthStencilState,
				const RasterizerState* rasterizerState) = 0;

			XNPP_API virtual void Draw(
				ITexture2D const& texture,
				Vector2 const& pos,				
				const Rectangle* sourceRect,
				Vector2 const& origin,
				Vector2 const& scale,
				float rotation,
				Color const& color,
				SpriteEffects effects,
				float layerDepth) = 0;

			XNPP_API virtual void End() = 0;

			XNPP_API static std::unique_ptr<ISpriteBatch> Create();
		};
	}
}

#endif