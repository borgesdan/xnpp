#include "Xna/Framework/Graphics/GraphicsResource.hpp"

namespace Xna {
	std::optional<GraphicsDevice> GraphicsResource::GetGraphicsDevice() const {
		return baseGraphicsDevice;
	}

	void GraphicsResource::Bind(std::optional<GraphicsDevice> const& device) {
		baseGraphicsDevice = device;
	}
}