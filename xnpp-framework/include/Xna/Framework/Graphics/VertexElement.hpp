#ifndef XNA_FRAMEWORK_GRAPHICS_VERTEXELEMENT_HPP
#define XNA_FRAMEWORK_GRAPHICS_VERTEXELEMENT_HPP

#include "Shared.hpp"
#include <optional>

namespace Xna {
	//Defines input vertex data to the pipeline.
	struct VertexElement final {
		//Retrieves or sets the offset (if any) from the beginning of the stream to the beginning of the vertex data.
		size_t Offset;
		//Gets or sets the format of this vertex element.
		Xna::VertexElementFormat VertexElementFormat;
		//Gets or sets a value describing how the vertex element is to be used.
		Xna::VertexElementUsage VertexElementUsage;
		//Modifies the usage data to allow the user to specify multiple usage types.
		size_t UsageIndex;

		//Initializes a new instance of the VertexElement class.
		constexpr VertexElement(
			size_t offset,
			Xna::VertexElementFormat elementFormat,
			Xna::VertexElementUsage elementUsage,
			size_t usageIndex) : Offset(offset), VertexElementFormat(elementFormat),
			VertexElementUsage(elementUsage), UsageIndex(usageIndex) {
		}

		constexpr bool operator==(const VertexElement& other) const noexcept {
			return Offset == other.Offset && VertexElementFormat == other.VertexElementFormat
				&& VertexElementUsage == other.VertexElementUsage && UsageIndex == other.UsageIndex;
		}

		constexpr operator std::optional<VertexElement>() const {
			return std::make_optional<VertexElement>(*this);
		}
	};
}

#endif