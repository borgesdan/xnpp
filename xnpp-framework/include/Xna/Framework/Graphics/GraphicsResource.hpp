#ifndef XNA_FRAMEWORK_GRAPHICS_GRAPHICSRESOURCE_HPP
#define XNA_FRAMEWORK_GRAPHICS_GRAPHICSRESOURCE_HPP

#include <optional>
#include "_GraphicsDevice.hpp"
#include "Xna/Internal/Export.hpp"

namespace Xna {
	//Queries and prepares resources. 
	class GraphicsResource {
	public:
		virtual ~GraphicsResource() = default;

	protected:
		constexpr GraphicsResource() = default;
		XNPP_API std::optional<GraphicsDevice> GetGraphicsDevice() const;
		XNPP_API void Bind(std::optional<GraphicsDevice> const& device);

	private:
		std::optional<GraphicsDevice> baseGraphicsDevice{};
	};
}

#endif