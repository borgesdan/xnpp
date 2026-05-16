#ifndef XNA_PLATFORM_PLATFORM_HPP
#define XNA_PLATFORM_PLATFORM_HPP

#include <string>
#include <vector>
#include <cstdint>
#include <optional>
#include <exception>
#include <filesystem>
#include <functional>
#include "Xna/Internal/Macros.hpp"
#include "Xna/Framework/Graphics/Shared.hpp"
#include "Xna/Framework/Graphics/VertexDeclaration.hpp"

namespace Xna {
	enum class PlayerIndex;
	enum class SurfaceFormat;
	enum class DepthFormat;
	enum class GraphicsProfile;
	enum class ClearOptions;
	enum class SpriteEffects;
	enum class BufferUsage;
	enum class SetDataOptions;
	enum class GamePadDeadZone;

	struct Rectangle;
	struct MouseState;
	struct GamePadState;
	struct KeyboardState;
	struct GamePadCapabilities;	
	struct PresentationParameters;
	struct DisplayMode;
	struct Viewport;
	struct BlendState;
	struct DepthStencilState;
	struct RasterizerState;
	struct SamplerState;
	struct Color;
	struct Matrix;
	struct Vector2;
	struct VertexDeclaration;

	class GameWindow;
	class GameHost;
	class VertexBuffer;
	class GraphicsAdapter;
	class GraphicsDevice;

	namespace Platform {
		struct System {
			XNPP_API static void Initialize();
			XNPP_API static void Dispose();
			XNPP_API static void Update();
			XNPP_API static void Suspend();
			XNPP_API static void Resume();
			XNPP_API static size_t GetClockCounter();
			XNPP_API static size_t GetClockFrequency();
			XNPP_API static bool MultiMonitorSupport();
			XNPP_API static void ProcessException(std::string const& exception);
			XNPP_API static void GetExecutablePath(std::filesystem::path& path);
		};		

		struct Window {
			XNPP_API static void Create(GameWindow const& gw);
			XNPP_API static void Update(GameWindow const& gw);
			XNPP_API static void Close(GameWindow const& gw);
			XNPP_API static bool WindowIsMinimized(GameWindow const& gw);
			XNPP_API static void MinimizeWindow(GameWindow const& gw, bool value);
			XNPP_API static void SetMouseVisible(GameWindow const& gw, bool value);
			XNPP_API static void AllowUserResizing(GameWindow const& gw, bool value);
			XNPP_API static Rectangle ClientBounds(GameWindow const& gw);
			XNPP_API static void SetTitle(GameWindow const& gw, std::string const& title);
		};

		struct Host {
			XNPP_API static void Tick(GameHost& gh);
		};

		struct Input {
			XNPP_API static KeyboardState KeyboardGetState();

			XNPP_API static MouseState MouseGetState();
			XNPP_API static intptr_t MouseGetWindowHandle();
			XNPP_API static void MouseSetWindowHandle(intptr_t value);
			XNPP_API static void MouseSetPosition(int32_t x, int32_t y);

			XNPP_API static GamePadState GamePadGetState(PlayerIndex index, GamePadDeadZone deadZone);
			XNPP_API static GamePadCapabilities GamePadGetCapabilities(PlayerIndex index);
			XNPP_API static bool GamePadSetVibration(PlayerIndex index, float leftMotor, float rightMotor, float leftTrigger, float rightTrigger);
			XNPP_API static void GamePadSuspend();
			XNPP_API static void GamePadResume();
		};

		struct Graphics {
			XNPP_API size_t GetMaxSamplerStates();
		};

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
			XNPP_API virtual void Unload() = 0;
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

		struct GraphicsAdapterDesc {
			std::string description;
			std::string deviceName;
			intptr_t monitorHandle{ 0 };
			uint32_t deviceId{ 0 };
			uint32_t revision{ 0 };
			uint32_t subSystemId{ 0 };
			uint32_t vendorId{ 0 };
			bool isDefaultAdapter{ false };
		};

		struct IGraphicsAdapter {
			XNPP_API virtual ~IGraphicsAdapter() = default;

			XNPP_API virtual const GraphicsAdapterDesc& GetDesc() const = 0;
			XNPP_API virtual std::vector<std::unique_ptr<IGraphicsAdapter>> GetAll() = 0;
			XNPP_API virtual std::vector<DisplayMode> SupportedDisplayModes() = 0;
			XNPP_API virtual DisplayMode CurrentDisplayMode() = 0;
			XNPP_API virtual bool IsProfileSupported(GraphicsProfile graphicsProfile) = 0;
			XNPP_API virtual bool QueryBackBufferFormat(GraphicsProfile graphicsProfile, SurfaceFormat format,
				DepthFormat depthFormat, int32_t multiSampleCount, SurfaceFormat& selectedFormat, DepthFormat& selectedDepthFormat, int32_t& selectedMultiSampleCount) = 0;
			XNPP_API virtual bool QueryRenderTargetFormat(GraphicsProfile graphicsProfile, SurfaceFormat format,
				DepthFormat depthFormat, int32_t multiSampleCount, SurfaceFormat& selectedFormat, DepthFormat& selectedDepthFormat, int32_t& selectedMultiSampleCount) = 0;

			XNPP_API static std::unique_ptr<IGraphicsAdapter> Create();
		};

		struct IGraphicsDevice {
			XNPP_API virtual ~IGraphicsDevice() = default;

			XNPP_API virtual void CreateDevice(GraphicsAdapter const& adapter, PresentationParameters const& presentationParameters) = 0;
			XNPP_API virtual void Present(std::optional<Rectangle> const& rec, std::optional<Rectangle> const& destination, intptr_t overrideWindowHandle) = 0;
			XNPP_API virtual Viewport GetViewport() = 0;
			XNPP_API virtual void SetViewport(Viewport const& viewport) = 0;
			XNPP_API virtual void Reset(PresentationParameters const& presentationParameters, GraphicsAdapter const& graphicsAdapter) = 0;

			XNPP_API virtual const BlendState& GetBlendState() const = 0;
			XNPP_API virtual const DepthStencilState& GetDepthStencilState() const = 0;
			XNPP_API virtual const RasterizerState& GetRasterizerState() const = 0;
			XNPP_API virtual const Color GetBlendFactor() const = 0;

			XNPP_API virtual void SetVertexBuffer(VertexBuffer const& vertexBuffer, uint32_t vertexOffset) = 0;

			XNPP_API virtual void ApplyBlendState(BlendState const& blend) = 0;
			XNPP_API virtual void ApplyDepthStencilState(DepthStencilState const& depth) = 0;
			XNPP_API virtual void ApplyRasterizerState(RasterizerState const& rasterizer) = 0;
			XNPP_API virtual void ApplyBlendFactor(Color const& color) = 0;

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
				const RasterizerState* rasterizerState,
				const Matrix* matrix) = 0;

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

			XNPP_API static std::unique_ptr<ISpriteBatch> Create(GraphicsDevice const& device);
		};

		struct IndexBufferStats {
			BufferUsage Usage{ BufferUsage::None };
			int IndexElementSize{ 0 };
			size_t IndexCount{ 0 };
		};

		struct IIndexBuffer {
			XNPP_API virtual ~IIndexBuffer() = default;

			XNPP_API virtual void Init(GraphicsDevice& device, size_t sizeOfIndexType, size_t indexCount, BufferUsage usage) = 0;
			XNPP_API virtual void SetData(size_t offsetInBytes, const void* data, size_t startIndex, size_t elementCount, size_t elementSize, SetDataOptions setDataOptions) = 0;
			XNPP_API virtual void GetData(size_t offsetInBytes, void* data, size_t startIndex, size_t elementCount, size_t elementSize) = 0;
			XNPP_API virtual IndexBufferStats GetStats() = 0;

			XNPP_API static std::unique_ptr<IIndexBuffer> Create();
			XNPP_API static std::unique_ptr<IIndexBuffer> CreateDynamic();
		};

		struct VertexBufferStats {
			BufferUsage Usage{ BufferUsage::None };
			uint32_t VertexCount{ 0 };
			VertexDeclaration VertexDeclaration{};
		};

		struct IVertexBuffer {
			XNPP_API virtual ~IVertexBuffer() = default;

			XNPP_API virtual void Init(GraphicsDevice const& graphicsDevice, VertexDeclaration const& vertexDeclaration, size_t vertexCount, BufferUsage usage) = 0;
			XNPP_API virtual void SetData(size_t offsetInBytes, const void* data, size_t startIndex, size_t elementCount, size_t vertexStride, size_t elementSize, SetDataOptions options) = 0;
			XNPP_API virtual void GetData(size_t offsetInBytes, void* data, size_t startIndex, size_t elementCount, size_t vertexStride, size_t elementSize) = 0;
			XNPP_API virtual VertexBufferStats GetStats() = 0;

			XNPP_API static std::unique_ptr<IVertexBuffer> Create();
			XNPP_API static std::unique_ptr<IVertexBuffer> CreateDynamic();
		};
	}	
}

#endif