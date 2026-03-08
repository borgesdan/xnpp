#ifndef XNA_GRAPHICS_GRAPHICSADAPTER_HPP
#define XNA_GRAPHICS_GRAPHICSADAPTER_HPP

#include <cstdint>
#include <string>
#include <memory>
#include "DisplayMode.hpp"
#include "DisplayModeCollection.hpp"
#include "Xna/Platform/Platform.hpp"
#include "Xna/Internal/Macros.hpp"

namespace Xna {
	//Provides methods to retrieve and manipulate graphics adapters.
	class GraphicsAdapter final {
	public:
		//Collection of available adapters on the system.
		XNPP_API static std::vector<std::optional<GraphicsAdapter>> Adapters();
		//Gets the current display mode.
		XNPP_API DisplayMode CurrentDisplayMode() const;
		//Gets the default adapter. 
		XNPP_API static GraphicsAdapter DefaultAdapter();
		//Retrieves a string used for presentation to the user.
		inline std::string Description() const { return backend->GetDesc().description; }
		//Retrieves a value that is used to help identify a particular chip set. 
		inline uint32_t DeviceId() const { return backend->GetDesc().deviceId; }
		//Retrieves a string that contains the device name.
		inline std::string DeviceName() const { return backend->GetDesc().deviceName; }
		//Determines if this instance of GraphicsAdapter is the default adapter.
		inline bool IsDefaultAdapter() const {
			return backend->GetDesc().isDefaultAdapter;
		}
		//Determines if the graphics adapter is in widescreen mode.
		inline bool IsWideScreen() const {
			return CurrentDisplayMode().AspectRatio() > 1.6000000238418579;
		}
		//Retrieves the handle of the monitor
		inline intptr_t MonitorHandle() const { return backend->GetDesc().monitorHandle; }
		//Retrieves a value used to help identify the revision level of a particular chip set.
		inline uint32_t Revision() const { return backend->GetDesc().revision; }
		//Retrieves a value used to identify the subsystem.
		inline uint32_t SubSystemId() const { return backend->GetDesc().subSystemId; }
		//Returns a collection of supported display modes for the current adapter.
		XNPP_API DisplayModeCollection SupportedDisplayModes() const;
		//Retrieves a value used to identify the manufacturer.
		inline uint32_t VendorId() const { return backend->GetDesc().vendorId; }

		//Gets or sets a NULL device. 
		static constexpr bool UseNullDevice() {
			return _UseNullDevice;
		}

		//Gets or sets a NULL device. 
		static constexpr void UseNullDevice(bool value) {
			_UseNullDevice = value;
		}

		//Gets or sets a reference device.
		static constexpr bool UseReferenceDevice() {
			return _UseReferenceDevice;
		}

		//Gets or sets a reference device. 
		static constexpr void UseReferenceDevice(bool value) {
			_UseReferenceDevice = value;
		}

		//Tests to see if the adapter supports the requested profile.
		XNPP_API bool IsProfileSupported(GraphicsProfile graphicsProfile);

		//Queries the adapter for support for the requested back buffer format.
		XNPP_API bool QueryBackBufferFormat(GraphicsProfile graphicsProfile,
			SurfaceFormat format,
			DepthFormat depthFormat,
			int32_t multiSampleCount,
			SurfaceFormat& selectedFormat,
			DepthFormat& selectedDepthFormat,
			int32_t& selectedMultiSampleCount) const;

		//Queries the adapter for support for the requested render target format.
		XNPP_API bool QueryRenderTargetFormat(GraphicsProfile graphicsProfile,
			SurfaceFormat format,
			DepthFormat depthFormat,
			int32_t multiSampleCount,
			SurfaceFormat& selectedFormat,
			DepthFormat& selectedDepthFormat,
			int32_t& selectedMultiSampleCount) const;

		XNPP_DECLARE_IMPL_WRAPPER(GraphicsAdapter, backend);

	private:
		inline static bool _UseNullDevice = false;
		inline static bool _UseReferenceDevice = false;

		std::shared_ptr<PlatformNS::IGraphicsAdapter> backend;

		inline GraphicsAdapter() {			
			backend = PlatformNS::IGraphicsAdapter::Create();
		}
	};
}

#endif