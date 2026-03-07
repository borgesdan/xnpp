#include "Xna/Framework/Graphics/GraphicsAdapter.hpp"
#include "Xna/Platform/Platform.hpp"

namespace Xna {
	std::optional<GraphicsAdapter>  GraphicsAdapter::_DefaultAdapter = std::nullopt;

	std::vector<std::optional<GraphicsAdapter>> GraphicsAdapter::Adapters() {
		auto backend = PlatformNS::IGraphicsAdapter::Create();
		auto adapters = backend->GetAll();

		if (adapters.empty())
			return {};

		std::vector<std::optional<GraphicsAdapter>> all(adapters.size());
		for (size_t i = 0; i < adapters.size(); ++i) {
			auto adp = GraphicsAdapter();
			adp.impl->description = adapters[i].description;
			adp.impl->deviceName = adapters[i].deviceName;
			adp.impl->monitorHandle = adapters[i].monitorHandle;
			adp.impl->deviceId = adapters[i].deviceId;
			adp.impl->revision = adapters[i].revision;
			adp.impl->subSystemId = adapters[i].subSystemId;
			adp.impl->vendorId = adapters[i].vendorId;

			all[i] = adp;
		}
		
		_DefaultAdapter = all[0].value();

		return all;
	}

	GraphicsAdapter GraphicsAdapter::DefaultAdapter() {
		if (!_DefaultAdapter.has_value()) {
			auto all = Adapters();

			if (!all.empty())
				_DefaultAdapter = all[0].value();			
		}			

		return _DefaultAdapter.value_or(nullptr);
	}
	
	DisplayModeCollection GraphicsAdapter::SupportedDisplayModes() const {
		auto modes = impl->backend->SupportedDisplayModes();
		auto collection = DisplayModeCollection(modes);
		return collection;
	}

	DisplayMode GraphicsAdapter::CurrentDisplayMode() const {
		return impl->backend->CurrentDisplayMode();
	}	

	bool GraphicsAdapter::QueryRenderTargetFormat(GraphicsProfile graphicsProfile,
		SurfaceFormat format,
		DepthFormat depthFormat,
		int32_t multiSampleCount,
		SurfaceFormat& selectedFormat,
		DepthFormat& selectedDepthFormat,
		int32_t& selectedMultiSampleCount) const {
		// return Platform::GraphicsAdapter_QueryRenderTargetFormat(*this, graphicsProfile, format, depthFormat, multiSampleCount, selectedFormat, selectedDepthFormat, selectedMultiSampleCount);
		return true;
	}

	bool GraphicsAdapter::QueryBackBufferFormat(GraphicsProfile graphicsProfile,
		SurfaceFormat format,
		DepthFormat depthFormat,
		int32_t multiSampleCount,
		SurfaceFormat& selectedFormat,
		DepthFormat& selectedDepthFormat,
		int32_t& selectedMultiSampleCount) const {
		//return Platform::GraphicsAdapter_QueryBackBufferFormat(*this, graphicsProfile, format, depthFormat, multiSampleCount, selectedFormat, selectedDepthFormat, selectedMultiSampleCount);
		return true;
	}

	bool GraphicsAdapter::IsProfileSupported(GraphicsProfile graphicsProfile) {
		//return Platform::GraphicsAdapter_IsProfileSupported(*this, graphicsProfile);
		return true;
	}
}