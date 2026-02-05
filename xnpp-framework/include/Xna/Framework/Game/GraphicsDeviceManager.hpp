#ifndef XNA_FRAMEWORK_GAME_GRAPHICSDEVICEMANAGER_HPP
#define XNA_FRAMEWORK_GAME_GRAPHICSDEVICEMANAGER_HPP

#include "Xna/Framework/Graphics/GraphicsDevice.hpp"
#include "Xna/Framework/Graphics/Shared.hpp"
#include "IGraphicsDeviceManager.hpp"
#include "Xna/Framework/Graphics/IGraphicsDeviceService.hpp"
#include "Xna/Framework/Shared.hpp"
#include <cstdint>
#include <memory>
#include <optional>
#include "Xna/CSharp/TimeSpan.hpp"
#include "Xna/CSharp/Exception.hpp"
#include "GraphicsDeviceInformation.hpp"
#include "Xna/Framework/Game/Game.hpp"
#include "Xna/Internal/Export.hpp"

namespace Xna {
	class Game;

	struct PreparingDeviceSettingsEventArgs : public CSharp::EventArgs {
		PreparingDeviceSettingsEventArgs(GraphicsDeviceInformation const& graphicsDeviceInformation)
			: graphicsDeviceInformation(graphicsDeviceInformation)
		{
		}

		GraphicsDeviceInformation graphicsDeviceInformation;
	};

	//Handles the configuration and management of the graphics device.
	class GraphicsDeviceManager : public IGraphicsDeviceManager, public IGraphicsDeviceService {
	public:

		~GraphicsDeviceManager() override = default;

		//Creates a new GraphicsDeviceManager and registers it to handle the configuration and management of the graphics device for the specified Game.
		XNPP_API GraphicsDeviceManager(Game const& game);

		//Gets the GraphicsDevice associated with the GraphicsDeviceManager.
		inline std::optional<Xna::GraphicsDevice> GraphicsDevice() const override { return impl->device; }

		//Gets the graphics profile, which determines the graphics feature set.
		inline Xna::GraphicsProfile GraphicsProfile() const { return impl->graphicsProfile; }

		//Gets the graphics profile, which determines the graphics feature set.
		inline void GraphicsProfile(Xna::GraphicsProfile const& value) {
			impl->graphicsProfile = value;
			impl->isDeviceDirty = true;
		}

		//Gets or sets a value that indicates whether the device should start in full-screen mode.
		inline bool IsFullScreen() const { return impl->isFullScreen; }

		//Gets or sets a value that indicates whether the device should start in full-screen mode.
		inline void IsFullScreen(bool value) {
			impl->isFullScreen = value;
			impl->isDeviceDirty = true;
		}

		//Gets or sets a value that indicates whether to enable a multisampled back buffer.
		inline bool PreferMultiSampling() const { return impl->allowMultiSampling; }

		//Gets or sets a value that indicates whether to enable a multisampled back buffer.
		inline void PreferMultiSampling(bool value) {
			impl->allowMultiSampling = value;
			impl->isDeviceDirty = true;
		}

		//Gets or sets the format of the back buffer.
		inline SurfaceFormat PreferredBackBufferFormat() const { return impl->backBufferFormat; }

		//Gets or sets the format of the back buffer.
		inline void PreferredBackBufferFormat(SurfaceFormat value) {
			impl->backBufferFormat = value;
			impl->isDeviceDirty = true;
		}

		//Gets or sets the preferred back-buffer height.
		inline int32_t PreferredBackBufferHeight() const { return impl->backBufferHeight; }

		//Gets or sets the preferred back-buffer height.
		inline void PreferredBackBufferHeight(int32_t value) {
			if (value <= 0) throw CSharp::ArgumentOutOfRangeException("value");
			impl->backBufferHeight = value;
			impl->useResizedBackBuffer = false;
			impl->isDeviceDirty = true;
		}

		//Gets or sets the preferred back-buffer width.
		inline int32_t PreferredBackBufferWidth() const { return impl->backBufferWidth; }

		//Gets or sets the preferred back-buffer width.
		inline void PreferredBackBufferWidth(int32_t value) const {
			if (value <= 0) throw CSharp::ArgumentOutOfRangeException("value");
			impl->backBufferWidth = value;
			impl->useResizedBackBuffer = false;
			impl->isDeviceDirty = true;
		}

		//Gets or sets the format of the depth stencil.
		inline DepthFormat PreferredDepthStencilFormat() const { return impl->depthStencilFormat; }

		//Gets or sets the format of the depth stencil.
		inline void PreferredDepthStencilFormat(DepthFormat value) const {
			impl->depthStencilFormat = value;
			impl->isDeviceDirty = true;
		}

		//Gets or sets the display orientations that are available if automatic rotation and scaling is enabled.
		inline DisplayOrientation SupportedOrientations() const { return impl->supportedOrientations; }

		//Gets or sets the display orientations that are available if automatic rotation and scaling is enabled.
		inline void SupportedOrientations(DisplayOrientation value) {
			impl->supportedOrientations = value;
			impl->isDeviceDirty = true;
		}

		//Gets or sets a value that indicates whether to sync to the vertical trace (vsync) when presenting the back buffer.		
		inline bool SynchronizeWithVerticalRetrace() const { { return impl->synchronizeWithVerticalRetrace; } }
		//Gets or sets a value that indicates whether to sync to the vertical trace (vsync) when presenting the back buffer.
		inline void SynchronizeWithVerticalRetrace(bool value) {
			impl->synchronizeWithVerticalRetrace = value;
			impl->isDeviceDirty = true;
		}

		//Applies any changes to device-related properties, changing the graphics device as necessary.
		XNPP_API void ApplyChanges();
		//Toggles between full screen and windowed mode.
		XNPP_API void ToggleFullScreen();

		inline CSharp::Event<CSharp::EventArgs> DeviceCreated() const override {
			return impl->deviceCreated;
		}

		inline CSharp::Event<CSharp::EventArgs> DeviceResetting() const override {
			return impl->deviceResetting;
		}

		inline CSharp::Event<CSharp::EventArgs> DeviceReset() const override {
			return impl->deviceReset;
		}

		inline CSharp::Event<CSharp::EventArgs> DeviceDisposing() const override {
			return impl->deviceDisposing;
		}

		inline CSharp::Event<PreparingDeviceSettingsEventArgs> PreparingDeviceSettings() const {
			return impl->preparingDeviceSettings;
		}

		//Specifies the default minimum back-buffer height.
		static constexpr int DefaultBackBufferHeight = 480;
		//Specifies the default minimum back-buffer width.
		static constexpr int DefaultBackBufferWidth = 800;

		inline GraphicsDeviceManager(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(GraphicsDeviceManager const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	protected:
		//Determines whether the given GraphicsDeviceInformation is compatible with the existing graphics device.
		XNPP_API bool CanResetDevice(GraphicsDeviceInformation const& newDeviceInfo);
		//Finds the best device configuration that is compatible with the current device preferences.
		XNPP_API GraphicsDeviceInformation FindBestDevice(bool anySuitableDevice);
		//Ranks the given list of devices that satisfy the given preferences.
		XNPP_API void RankDevices(std::vector<GraphicsDeviceInformation>& foundDevices);

		XNPP_API void OnDeviceCreated(void* sender, CSharp::EventArgs const& e);
		XNPP_API void OnDeviceDisposing(void* sender, CSharp::EventArgs const& e);
		XNPP_API void OnDeviceReset(void* sender, CSharp::EventArgs const& e);
		XNPP_API void OnDeviceResetting(void* sender, CSharp::EventArgs const& e);
		XNPP_API void OnPreparingDeviceSettings(void* sender, PreparingDeviceSettingsEventArgs const& e);

	private:
		void GameWindowScreenDeviceNameChanged(void* sender, CSharp::EventArgs const& e);
		void GameWindowClientSizeChanged(void* sender, CSharp::EventArgs const& e);
		void GameWindowOrientationChanged(void* sender, CSharp::EventArgs const& e);

		bool EnsureDevice();
		void CreateDevice(GraphicsDeviceInformation& newInfo);
		XNPP_API void ChangeDevice(bool forceCreate);
		void MassagePresentParameters(PresentationParameters& pp);
		static void ConfigureTouchInput(PresentationParameters const& pp);

		void HandleDisposing(void* sender, CSharp::EventArgs const& e);
		void HandleDeviceLost(void* sender, CSharp::EventArgs const& e);
		void HandleDeviceReset(void* sender, CSharp::EventArgs const& e);
		void HandleDeviceResetting(void* sender, CSharp::EventArgs const& e);
		Xna::GraphicsProfile ReadDefaultGraphicsProfile();
		void RankDevicesPlatform(std::vector<GraphicsDeviceInformation>& foundDevices);
		GraphicsDeviceInformation FindBestPlatformDevice(bool anySuitableDevice);
		void AddDevices(bool anySuitableDevice, std::vector<GraphicsDeviceInformation>& foundDevices);
		void AddDevices(GraphicsAdapter const& adapter, DisplayMode const& mode, GraphicsDeviceInformation const& baseDeviceInfo, std::vector<GraphicsDeviceInformation>& foundDevices);
		bool IsWindowOnAdapter(intptr_t windowHandle, GraphicsAdapter const& adapter);
		bool EnsureDevicePlatform();
		void ValidateGraphicsDeviceInformation(GraphicsDeviceInformation const& devInfo);

	public:
		inline virtual void CreateDevice() override { ChangeDevice(true); };
		XNPP_API virtual bool BeginDraw() override;
		XNPP_API virtual void EndDraw() override;

	public:
		struct Implementation {
			CSharp::TimeSpan deviceLostSleepTime{ 50 };
			std::optional<Game> game;
			bool isReallyFullScreen{ false };
			bool isDeviceDirty{ false };
			bool inDeviceTransition{ false };
			std::optional<Xna::GraphicsDevice> device;
			bool synchronizeWithVerticalRetrace{ true };
			bool isFullScreen{ false };
			Xna::GraphicsProfile graphicsProfile{};
			SurfaceFormat backBufferFormat{ SurfaceFormat::Color };
			DepthFormat depthStencilFormat{ DepthFormat::Depth24 };
			int32_t backBufferWidth{ DefaultBackBufferWidth };
			int32_t backBufferHeight{ DefaultBackBufferHeight };
			bool allowMultiSampling{ false };
			DisplayOrientation supportedOrientations{ DisplayOrientation::Default };
			DisplayOrientation currentWindowOrientation{ DisplayOrientation::Default };
			int32_t resizedBackBufferWidth{ 0 };
			int32_t resizedBackBufferHeight{ 0 };
			bool useResizedBackBuffer{ false };
			CSharp::Event<CSharp::EventArgs> deviceCreated;
			CSharp::Event<CSharp::EventArgs> deviceResetting;
			CSharp::Event<CSharp::EventArgs> deviceReset;
			CSharp::Event<CSharp::EventArgs> deviceDisposing;
			CSharp::Event<PreparingDeviceSettingsEventArgs> preparingDeviceSettings;
			bool beginDrawOk{ false };
		};

		std::shared_ptr<Implementation> impl;		
	};

	struct GraphicsDeviceInformationComparer {
		std::optional<GraphicsDeviceManager> graphics;

		GraphicsDeviceInformationComparer(GraphicsDeviceManager const& graphicsComponent) : graphics(graphicsComponent){}

		XNPP_API bool operator()(GraphicsDeviceInformation const& d1, GraphicsDeviceInformation const& d2) const;

		inline int RankFormat(SurfaceFormat format) const
		{
			if (format == graphics->PreferredBackBufferFormat())
				return 0;

			return SurfaceFormatBitDepth(format) == SurfaceFormatBitDepth(graphics->PreferredBackBufferFormat()) ? 1 : 99;//int.MaxValue
		}

		static constexpr int SurfaceFormatBitDepth(SurfaceFormat format)
		{
			switch (format)
			{
			case SurfaceFormat::Color:
			case SurfaceFormat::Rgba1010102:
				return 32;
			case SurfaceFormat::Bgr565:
			case SurfaceFormat::Bgra5551:
			case SurfaceFormat::Bgra4444:
				return 16;
			default:
				return 0;
			}
		}
	};
}

#endif