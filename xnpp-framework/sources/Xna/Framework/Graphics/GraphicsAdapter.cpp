#include "Xna/Framework/Graphics/GraphicsAdapter.hpp"
#include "Xna/Platform/Platform.hpp"

namespace Xna {
	std::optional<GraphicsAdapter> GraphicsAdapter::Implementation::DefaultAdapter = std::nullopt;

	std::vector<std::optional<GraphicsAdapter>> GraphicsAdapter::Adapters() {
		auto all = Platform::GraphicsAdapter_GetAllGraphicsAdapters();

		if (!all.empty())
			Implementation::DefaultAdapter = all[0].value();

		return all;
	}

	GraphicsAdapter GraphicsAdapter::DefaultAdapter() {
		if (!Implementation::DefaultAdapter.has_value()) {
			auto all = Platform::GraphicsAdapter_GetAllGraphicsAdapters();

			if (!all.empty())
				Implementation::DefaultAdapter = all[0].value();
			else
				throw CSharp::InvalidOperationException("Default GraphicsAdapter not found.");
		}			

		return Implementation::DefaultAdapter.value();
	}

	DisplayModeCollection GraphicsAdapter::SupportedDisplayModes() const {
		if (impl->supportedDisplayModes.Count() == 0)
			Platform::GraphicsAdapter_SupportedDisplayModes(*this);

		return impl->supportedDisplayModes;
	}

	DisplayMode GraphicsAdapter::CurrentDisplayMode() const {
		if (!impl->currentDisplayMode.has_value())
			Platform::GraphicsAdapter_CurrentDisplayMode(*this);

		if (!impl->currentDisplayMode.has_value())
			return DisplayMode();

		return impl->currentDisplayMode.value();
	}	
}