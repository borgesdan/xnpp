#ifndef XNA_GRAPHICS_GRAPHICSADAPTER_HPP
#define XNA_GRAPHICS_GRAPHICSADAPTER_HPP

#include <cstdint>
#include <string>
#include <memory>
#include "DisplayMode.hpp"
#include "DisplayModeCollection.hpp"
#include "Xna/Platform/Platform.hpp"

namespace Xna {
	//Provides methods to retrieve and manipulate graphics adapters.
	class GraphicsAdapter final {
	public:				
		//Collection of available adapters on the system.
		static std::vector<std::optional<GraphicsAdapter>> Adapters();
		//Gets the current display mode.
		DisplayMode CurrentDisplayMode() const;
		//Gets the default adapter. 
		static GraphicsAdapter DefaultAdapter();
		//Retrieves a string used for presentation to the user.
		inline std::string Description() const { return impl->description; }
		//Retrieves a value that is used to help identify a particular chip set. 
		inline uint32_t DeviceId() const { return impl->deviceId; }
		//Retrieves a string that contains the device name.
		std::string DeviceName() const { return impl->deviceName; }
		//Determines if this instance of GraphicsAdapter is the default adapter.
		inline bool IsDefaultAdapter() const {
			return impl->isDefaultAdapter;
		}
		//Determines if the graphics adapter is in widescreen mode.
		inline bool IsWideScreen() const {
			return CurrentDisplayMode().AspectRatio() > 1.6000000238418579;
		}
		//Retrieves the handle of the monitor
		inline intptr_t MonitorHandle() const { return impl->monitorHandle; }
		//Retrieves a value used to help identify the revision level of a particular chip set.
		inline uint32_t Revision() const { return impl->revision; }
		//Retrieves a value used to identify the subsystem.
		inline uint32_t SubSystemId() const { return impl->subSystemId; }
		//Returns a collection of supported display modes for the current adapter.
		DisplayModeCollection SupportedDisplayModes() const;
		//Retrieves a value used to identify the manufacturer.
		inline uint32_t VendorId() const { return impl->vendorId; }

		//Gets or sets a NULL device. 
		static constexpr bool UseNullDevice() {
			return Implementation::useNullDevice;
		}
		
		//Gets or sets a NULL device. 
		static constexpr void UseNullDevice(bool value) {
			Implementation::useNullDevice = value;
		}
		
		//Gets or sets a reference device.
		static constexpr bool UseReferenceDevice() {
			return Implementation::UseReferenceDevice;
		}
		
		//Gets or sets a reference device. 
		static constexpr void UseReferenceDevice(bool value) {
			Implementation::UseReferenceDevice = value;
		}
		
		//Tests to see if the adapter supports the requested profile.
		inline bool IsProfileSupported(GraphicsProfile graphicsProfile) {
			return Platform::GraphicsAdapter_IsProfileSupported(*this, graphicsProfile);
		}

		//Queries the adapter for support for the requested back buffer format.
		inline bool QueryBackBufferFormat(GraphicsProfile graphicsProfile,
			SurfaceFormat format,
			DepthFormat depthFormat,
			int32_t multiSampleCount,
			SurfaceFormat& selectedFormat,
			DepthFormat& selectedDepthFormat,
			int32_t& selectedMultiSampleCount) const {
			return Platform::GraphicsAdapter_QueryBackBufferFormat(*this, graphicsProfile, format, depthFormat, multiSampleCount, selectedFormat, selectedDepthFormat, selectedMultiSampleCount);
		}

		//Queries the adapter for support for the requested render target format.
		inline bool QueryRenderTargetFormat(GraphicsProfile graphicsProfile,
			SurfaceFormat format,
			DepthFormat depthFormat,
			int32_t multiSampleCount,
			SurfaceFormat& selectedFormat,
			DepthFormat& selectedDepthFormat,
			int32_t& selectedMultiSampleCount) const {
			return Platform::GraphicsAdapter_QueryRenderTargetFormat(*this, graphicsProfile, format, depthFormat, multiSampleCount, selectedFormat, selectedDepthFormat, selectedMultiSampleCount);
		}
		
		inline GraphicsAdapter(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(GraphicsAdapter const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }
		
	private:
		struct Implementation {
			std::string description;
			std::string deviceName;
			intptr_t monitorHandle{ 0 };
			uint32_t deviceId{ 0 };
			uint32_t revision{ 0 };
			uint32_t subSystemId{ 0 };
			uint32_t vendorId{ 0 };
			bool isDefaultAdapter{ false };

			std::optional<DisplayMode> currentDisplayMode;
			DisplayModeCollection supportedDisplayModes;
			static std::optional<GraphicsAdapter> DefaultAdapter;

			inline static bool useNullDevice = false;
			inline static bool UseReferenceDevice = false;

			PlatformImpl::GraphicsAdapterImpl platformImpl;
		};		
		
		inline GraphicsAdapter() { impl = std::make_shared<Implementation>(); }
		std::shared_ptr<Implementation> impl;		
	
		friend struct Platform;
	};	
}

#endif