#ifndef XNA_PLATFORM__PLATFORM_HPP
#define XNA_PLATFORM__PLATFORM_HPP

#include <string>
#include <vector>
#include <cstdint>
#include <optional>
#include <exception>
#include <filesystem>
#include <tuple>
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
		struct KeyboardImpl;
		struct MouseImpl;
		struct GamePadImpl;
		struct SoundEffectImpl;
		struct SoundEffectInstanceImpl;
		struct DynamicSoundEffectInstanceImpl;
		struct SongImpl;
		struct EffectImpl;
		struct EffectPassImpl;
		struct EffectTechniqueImpl;
		struct EffectAnnotationImpl;
		struct BasicEffectImpl;
		struct VertexBufferImpl;
		struct DynamicVertexBufferImpl;
	};		

	struct Platform {
		struct InputProcessMessage;
		static size_t MaxSamplerStates;	
		
		//Platform

		static void Initialize();
		static void Dispose();	
		static void Update();
		static void Suspend();
		static void Resume();

		//System

		static Rectangle System_ClientRect(intptr_t hwnd);
		static Rectangle System_WindowRect(intptr_t hwnd);
		static size_t System_GetClockCounter();
		static size_t System_GetClockFrequency();
		static void System_ProcessException(std::exception& ex);
		static void System_GetExecutablePath(std::filesystem::path& path);
		static bool System_MultiMonitorSupport();
		static std::tuple<int32_t, int32_t, int32_t, int32_t> System_VirtualScreen();
		static std::tuple<int32_t, int32_t> System_PrimaryMonitorSize();
		static std::tuple<int32_t, int32_t, int32_t, int32_t> System_WorkingArea();
		static std::tuple<int32_t, int32_t, int32_t, int32_t> System_MonitorWorkingArea(intptr_t hMonitor);
		static intptr_t System_MonitorFromHandle(intptr_t hwnd);
		static intptr_t System_MonitorFromRect(int32_t left, int32_t top, int32_t right, int32_t bottom);
		static intptr_t System_MonitorFromPoint(int32_t x, int32_t y);
		static std::string System_MonitorDeviceName(intptr_t monitor);
		static std::tuple<int32_t, int32_t, int32_t, int32_t> System_MonitorArea(intptr_t monitor);
		static bool System_MonitorIsPrimary(intptr_t monitor);
		static int32_t System_MonitorBitDepth(intptr_t monitor, intptr_t hdc);
		//static void System_GetAllScreens(std::vector<CSharp::Screen>& scren);
		static std::vector<std::tuple<intptr_t, intptr_t>> System_GetAllMonitorHandlers();

		//GameWindow

		static void GameWindow_Create(GameWindow const& gw);
		static void GameWindow_Update(GameWindow const& gw);
		static void GameWindow_Close(GameWindow const& gw);
		static bool GameWindow_WindowIsMinimized(GameWindow const& gw);
		static void GameWindow_MinimizeWindow(GameWindow const& gw, bool value);
		static void GameWindow_SetMouseVisible(GameWindow const& gw, bool value);
		static void GameWindow_AllowUserResizing(GameWindow const& gw, bool value);
		static Rectangle GameWindow_ClientBounds(GameWindow const& gw);
		static void GameWindow_SetTitle(GameWindow const& gw, std::string const& title);
		static void GameWindow_BeginScreenDeviceChange(GameWindow const& gw, bool willBeFullScreen);
		static void GameWindow_EndScreenDeviceChange(GameWindow const& gw, std::string const& screenDeviceName, int32_t clientWidth, int32_t clientHeight);

		//GameHost

		static void GameHost_Tick(GameHost& gh);						

		//Input
		static KeyboardState Keyboard_GetState();
		static void Keyboard_ProcessMessage(InputProcessMessage const& msg);

		static MouseState Mouse_GetState();
		static void Mouse_ProcessMessage(InputProcessMessage const& msg);
		static intptr_t Mouse_GetWindowHandle();
		static void Mouse_SetWindowHandle(intptr_t value);
		static void Mouse_SetPosition(int32_t x, int32_t y);

		static GamePadState GamePad_GetState(PlayerIndex index, GamePadDeadZone deadZone);
		static GamePadCapabilities GamePad_GetCapabilities(PlayerIndex index);
		static bool GamePad_SetVibration(PlayerIndex index, float leftMotor, float rightMotor, float leftTrigger, float rightTrigger);
		static void GamePad_Suspend();
		static void GamePad_Resume();		

		//States

		static void BlendState_Apply(BlendState const& blendState, GraphicsDevice& device);
		static void DepthStencilState_Apply(DepthStencilState const& depthStencil, GraphicsDevice& device);
		static void RasterizerState_Apply(RasterizerState const& state, GraphicsDevice& device);
		static void SamplerState_Apply(SamplerState const& state, GraphicsDevice& device, size_t samplerIndex, SamplerStateApplyType type);				
		
		//GraphicsAdapter

		static std::vector<std::optional<GraphicsAdapter>> GraphicsAdapter_GetAllGraphicsAdapters();
		static void GraphicsAdapter_SupportedDisplayModes(GraphicsAdapter const& adapter);
		static void GraphicsAdapter_CurrentDisplayMode(GraphicsAdapter const& adapter);
		static bool GraphicsAdapter_IsProfileSupported(GraphicsAdapter const& adapter, GraphicsProfile graphicsProfile);
		static bool GraphicsAdapter_QueryBackBufferFormat(GraphicsAdapter const& adapter, GraphicsProfile graphicsProfile, SurfaceFormat format, 
			DepthFormat depthFormat, int32_t multiSampleCount, SurfaceFormat& selectedFormat, DepthFormat& selectedDepthFormat, int32_t& selectedMultiSampleCount);
		static bool GraphicsAdapter_QueryRenderTargetFormat(GraphicsAdapter const& adapter, GraphicsProfile graphicsProfile, SurfaceFormat format,
			DepthFormat depthFormat, int32_t multiSampleCount, SurfaceFormat& selectedFormat, DepthFormat& selectedDepthFormat, int32_t& selectedMultiSampleCount);

		// GraphicsDevice

		static void GraphicsDevice_CreateDevice(GraphicsDevice& graphicsDevice, GraphicsAdapter const& adapter, Xna::PresentationParameters const& presentationParameters);
		static void GraphicsDevice_Present(GraphicsDevice const& graphicsDevice, std::optional<Rectangle> const& rec, std::optional<Rectangle> const& destination, intptr_t overrideWindowHandle);
		static void GraphicsDevice_SetViewport(GraphicsDevice const& graphicsDevice, Viewport const& viewport);
		static void GraphicsDevice_MakeWindowAssociation(GraphicsDevice const& graphicsDevice, PresentationParameters const& pp);
		static void GraphicsDevice_GetScissorRectangles(GraphicsDevice const& graphicsDevice, std::vector<Xna::Rectangle>& scissors);
		static void GraphicsDevice_SetScissorRectangles(GraphicsDevice const& graphicsDevice, std::vector<Xna::Rectangle>& scissors);
		static void GraphicsDevice_ClearRenderTarget(GraphicsDevice const& graphicsDevice, Color const& color);
		static RenderTarget2D GraphicsDevice_CreateBackBufferRenderTarget(GraphicsDevice const& graphicsDevice);
		static void GraphicsDevice_SetRenderTargets(GraphicsDevice const& graphicsDevice);
		static void GraphicsDevice_Reset(GraphicsDevice& device, Xna::PresentationParameters const& presentationParameters, GraphicsAdapter const& graphicsAdapter);
		static void GraphicsDevice_LazyInitialization(GraphicsDevice& device, intptr_t windowHandle);
		

		//Texture2D

		enum class Texture2D_SaveFileType {
			Png,
			Jpeg
		};

		static void Texture2D_Init(Texture2D& texture, GraphicsDevice const& device, size_t width, size_t height, bool mipMap, SurfaceFormat format);
		static Texture2D Texture2D_FromStream(GraphicsDevice const& device, CSharp::Stream& stream, size_t width, size_t height, bool zoom);
		static void Texture2D_SaveFile(Texture2D& texture, CSharp::Stream& stream, size_t width, size_t height, Texture2D_SaveFileType type);
		static int32_t Texture2D_LevelCount(Texture2D const& texture);
		static void Texture2D_SetData(Texture2D& texture, size_t level, std::optional<Rectangle> const& rect, const void* data, size_t startIndex, size_t elementCount, bool hasMipMap, size_t sizeOfData);
		static void Texture2D_GetData(Texture2D& texture, size_t level, std::optional<Rectangle> const& rect, void* data, size_t startIndex, size_t elementCount, size_t sizeOfData);		

		//Texture3D

		static void Texture3D_Init(Texture3D& texture, GraphicsDevice& graphicsDevice, size_t width, size_t height, size_t depth, bool mipMap, SurfaceFormat format);
		void Texture3D_SetData(Texture3D& texture, size_t level, size_t left, size_t top, size_t right, size_t bottom, size_t front, size_t back,
			const void* data, size_t startIndex, size_t elementCount);
		static void Texture3D_GetData(Texture3D& texture, size_t level, size_t left, size_t top, size_t right, size_t bottom, size_t front, size_t back,
			void* data, size_t startIndex, size_t elementCount, size_t sizeOfData);

		//RenderTarget2D

		static void RenderTarget2D_Init(RenderTarget2D& renderTarget, GraphicsDevice& device, DepthFormat preferredDepthFormat,
			size_t preferredMultiSampleCount, RenderTargetUsage usage);

		//SpriteBatch

		static void SpriteBatch_InitializeSpriteBatch(SpriteBatch const& sb, GraphicsDevice const& device);
		static void SpriteBatch_Begin(SpriteBatch const& sb, SpriteSortMode sortMode, std::optional<BlendState> const& blendState, std::optional<SamplerState> const& samplerState,
			std::optional<DepthStencilState> const& depthStencilState, std::optional<RasterizerState> const& rasterizerState, std::optional<Effect> const& effect, std::optional<Matrix> transformMatrix);
		static void SpriteBatch_End(SpriteBatch const& sb);
		static void SpriteBatch_Draw(SpriteBatch const& sb, Texture2D const& texture, Vector2 const& position, std::optional<Rectangle> const& sourceRectangle, Color const& color,
			float rotation, Vector2 const& origin, Vector2 const& scale, SpriteEffects effects, float layerDepth);
		static void SpriteBatch_Draw(SpriteBatch const& sb, Texture2D const& texture, Rectangle const& destinationRectangle, std::optional<Rectangle> const& sourceRectangle, Color const& color,
			float rotation, Vector2 const& origin, SpriteEffects effects, float layerDepth);
		static void SpriteBatch_DrawString(SpriteBatch const& sb, SpriteFont const& sf, std::string const& text, Vector2 const& position, Color const& color, float rotation, Vector2 const& origin,
			Vector2 const& scale, SpriteEffects effects, float layerDepth);

		//SpriteFont

		static void SpriteFont_Create(SpriteFont const& sf, Texture2D const& texture,
			std::vector<Rectangle> const& glyphs, std::vector<Rectangle> const& cropping,
			std::vector<char32_t> const& charMap, int32_t lineSpacing, float spacing,
			std::vector<Vector3> const& kerning, std::optional<char32_t> const& defaultCharacter);
		static Vector2 SpriteFont_MeasureString(SpriteFont const& sf, std::string const& text, bool ignoreWhiteSpace);
		static Vector2 SpriteFont_MeasureString(SpriteFont const& sf, std::wstring const& text, bool ignoreWhiteSpace);

		//SoundEffect

		enum class SoundEffect_State {
			Play,
			Pause,
			Stop,
			Resume
		};

		static void SoundEffect_Create(SoundEffect const&, std::vector<uint8_t> const& format, std::vector<uint8_t> const& data, size_t offset, size_t count, size_t loopStart, size_t loopLength);
		static void SoundEffect_SetMasterSoundProperties(std::optional<float> volume, std::optional<float> speedOfSound, std::optional<float> dopplerScale, std::optional<float> distanceScale);
		static void SoundEffect_CreateInstance(SoundEffectInstance const& se);
		static void SoundEffect_CreateInstance(DynamicSoundEffectInstance const& se);
		static void SoundEffect_DeleteInstance(SoundEffectInstance const& se);
		static void SoundEffect_SetState(SoundEffectInstance const& se, SoundEffect_State state, bool immediateIfStop = true);
		static void SoundEffect_SetState(DynamicSoundEffectInstance const& se, SoundEffect_State state, bool immediateIfStop = true);
		static void SoundEffect_SetAttributes(SoundEffectInstance const& se, std::optional<float> volume, std::optional<float> pan, std::optional<float> pitch);
		static void SoundEffect_Apply3D(SoundEffectInstance const& se, std::vector<AudioListener> const& listener, AudioEmitter emitter);
		static SoundState SoundEffect_GetState(SoundEffectInstance const& se);
		static void SoundEffect_SubmitBuffer(DynamicSoundEffectInstance const& se, std::vector<uint8_t> const& buffer, size_t offset, size_t count);
		static int32_t SoundEffect_GetPendingBufferCount(DynamicSoundEffectInstance const& se);

		//Song

		static void Song_FromFile(Song const& song, std::filesystem::path const& filename);
		static void MediaPlayer_Play(Song const& song);
		static void MediaPlayer_Pause();
		static void MediaPlayer_Resume();
		static void MediaPlayer_Stop();
		static void MediaPlayer_SetVolume(float volume);
		static void MediaPlayer_SetMuted(bool value);
		static void MediaPlayer_SetIsRepeating(bool value);
		static double MediaPlayer_GetPlayPosition();
		static double MediaPlayer_GetDuration();
		
		enum class MediaPlayer_MediaProperty {
			Album,
			Genre,
			Artist,
			Duration,
			TrackNumber,
			IsProtected,
			PlayCount,
			Rating
		};

		static void MediaPlayer_SetProperty(Song const& song, MediaPlayer_MediaProperty prop);
		
		//Effects

		static void Effect_EffectPass_Apply(EffectPass const& p);
		static void Effect_BasicEffect_Create(BasicEffect const& e, GraphicsDevice const& d);
		static void Effect_BasicEffect_SetValues(BasicEffect const& e);
		static void Effect_BasicEffect_Apply(BasicEffect const& e);

		//Buffer

		static void VertexBuffer_Create(VertexBuffer const b);
		static void VertexBuffer_SetData(VertexBuffer const& b,
			size_t offsetInBytes,
			const void* data,
			size_t sizeOfData,
			size_t startIndex,
			size_t elementCount,
			size_t vertexStride,
			std::optional<SetDataOptions> options);
		static void VertexBuffer_GetData(VertexBuffer const& b,
			size_t offsetInBytes,
			void* data,
			size_t sizeOfData,
			size_t startIndex,
			size_t elementCount,
			size_t vertexStride
			);
		
	};	
}

#endif