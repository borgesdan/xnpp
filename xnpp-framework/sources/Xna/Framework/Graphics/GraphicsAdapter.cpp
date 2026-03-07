#include "Xna/Framework/Graphics/GraphicsAdapter.hpp"
#include "Xna/Platform/Platform.hpp"

namespace Xna {
	std::optional<GraphicsAdapter> GraphicsAdapter::Implementation::DefaultAdapter = std::nullopt;

	std::vector<std::optional<GraphicsAdapter>> GraphicsAdapter::Adapters() {
		auto backend = PlatformNS::IGraphicsAdapter::Create();
		auto adapters = backend->GetAll();

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

		return all;
	}

	GraphicsAdapter GraphicsAdapter::DefaultAdapter() {
		if (!Implementation::DefaultAdapter.has_value()) {
			auto all = Adapters();

			if (!all.empty())
				Implementation::DefaultAdapter = all[0].value();
			else
				throw CSharp::InvalidOperationException("Default GraphicsAdapter not found.");
		}			

		return Implementation::DefaultAdapter.value();
	}

	DisplayModeCollection GraphicsAdapter::SupportedDisplayModes() const {
		if (impl->supportedDisplayModes.Count() == 0) {
			//Platform::GraphicsAdapter_SupportedDisplayModes(*this);
			
		}

		return impl->supportedDisplayModes;
	}

	DisplayMode GraphicsAdapter::CurrentDisplayMode() const {
		if (!impl->currentDisplayMode.has_value()) {
			//Platform::GraphicsAdapter_CurrentDisplayMode(*this);

		}

		if (!impl->currentDisplayMode.has_value())
			return DisplayMode();

		return impl->currentDisplayMode.value();
	}	
}