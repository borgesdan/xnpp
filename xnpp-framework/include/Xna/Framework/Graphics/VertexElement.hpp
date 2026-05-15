#ifndef XNA_FRAMEWORK_GRAPHICS_VERTEXELEMENT_HPP
#define XNA_FRAMEWORK_GRAPHICS_VERTEXELEMENT_HPP

#include "Shared.hpp"
#include <cstdint>

namespace Xna {
	//Defines input vertex data to the pipeline.
	struct VertexElement final {
		//Retrieves or sets the offset (if any) from the beginning of the stream to the beginning of the vertex data.
		uint32_t Offset{ 0 };
		//Gets or sets the format of this vertex element.
		Xna::VertexElementFormat VertexElementFormat{ Xna::VertexElementFormat::Single };
		//Gets or sets a value describing how the vertex element is to be used.
		Xna::VertexElementUsage VertexElementUsage{ Xna::VertexElementUsage::Position };
		//Modifies the usage data to allow the user to specify multiple usage types.
		uint32_t UsageIndex{ 0 };

		//Initializes a new instance of the VertexElement class.
		constexpr VertexElement(
			uint32_t offset,
			Xna::VertexElementFormat elementFormat,
			Xna::VertexElementUsage elementUsage,
			uint32_t usageIndex) : Offset(offset), VertexElementFormat(elementFormat),
			VertexElementUsage(elementUsage), UsageIndex(usageIndex) {
		}

		constexpr VertexElement() = default;

		constexpr bool operator==(const VertexElement& other) const noexcept = default;
	};
}

#endif