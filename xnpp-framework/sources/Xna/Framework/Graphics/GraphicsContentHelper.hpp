#ifndef XNA_FRAMEWORK_GRAPHICS_GRAPHICSCONTENTHELPER_HPP
#define XNA_FRAMEWORK_GRAPHICS_GRAPHICSCONTENTHELPER_HPP

#include "Xna/Framework/Graphics/Shared.hpp"

namespace Xna {
	class GraphicsDevice;
	class ContentReader;

	//Internal class
	struct GraphicsContentHelper final {
	public:
		static GraphicsDevice GraphicsDeviceFromContentReader(ContentReader& contentReader);

		GraphicsContentHelper() = delete;
		GraphicsContentHelper(GraphicsContentHelper&) = delete;
		GraphicsContentHelper(GraphicsContentHelper&&) = delete;

		static bool IsProfileCompatible(GraphicsProfile deviceProfile, GraphicsProfile contentProfile);
	};
}

#endif