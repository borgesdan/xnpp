#include "Xna/Framework/Graphics/GraphicsAdapter.hpp"
#include "Xna/Platform/Platform.hpp"

namespace Xna {

	static GraphicsAdapter _DefaultAdapter = nullptr;

	std::vector<std::optional<GraphicsAdapter>> GraphicsAdapter::Adapters() {
		static std::vector<std::optional<GraphicsAdapter>> all;

		if (!all.empty())
			return all;

		auto backend = PlatformNS::IGraphicsAdapter::Create();
		auto adapters = backend->GetAll();

		if (adapters.empty())
			return {};			

		all.resize(adapters.size());

		for (size_t i = 0; i < adapters.size(); ++i) {
			auto adp = GraphicsAdapter(nullptr);
			adp.backend = std::move(adapters[i]);

			if (adapters.size() == 1 || adapters[i]->GetDesc().isDefaultAdapter)
				_DefaultAdapter = adp;

			all[i] = adp;
		}

		

		return all;
	}

	GraphicsAdapter GraphicsAdapter::DefaultAdapter() {
		if (!_DefaultAdapter) {
			auto all = Adapters();

			if (!all.empty())
				_DefaultAdapter = all[0].value();
		}			

		return _DefaultAdapter;
	}
	
	DisplayModeCollection GraphicsAdapter::SupportedDisplayModes() const {
		auto modes = backend->SupportedDisplayModes();
		auto collection = DisplayModeCollection(modes);
		return collection;
	}

	DisplayMode GraphicsAdapter::CurrentDisplayMode() const {
		return backend->CurrentDisplayMode();
	}	

	bool GraphicsAdapter::QueryRenderTargetFormat(GraphicsProfile graphicsProfile,
		SurfaceFormat format,
		DepthFormat depthFormat,
		int32_t multiSampleCount,
		SurfaceFormat& selectedFormat,
		DepthFormat& selectedDepthFormat,
		int32_t& selectedMultiSampleCount) const {
		return backend->QueryRenderTargetFormat(graphicsProfile, format, depthFormat, multiSampleCount, selectedFormat, selectedDepthFormat, selectedMultiSampleCount);
	}

	bool GraphicsAdapter::QueryBackBufferFormat(GraphicsProfile graphicsProfile,
		SurfaceFormat format,
		DepthFormat depthFormat,
		int32_t multiSampleCount,
		SurfaceFormat& selectedFormat,
		DepthFormat& selectedDepthFormat,
		int32_t& selectedMultiSampleCount) const {
		return backend->QueryBackBufferFormat(graphicsProfile, format, depthFormat, multiSampleCount, selectedFormat, selectedDepthFormat, selectedMultiSampleCount);
	}

	bool GraphicsAdapter::IsProfileSupported(GraphicsProfile graphicsProfile) {
		return backend->IsProfileSupported(graphicsProfile);
	}
}