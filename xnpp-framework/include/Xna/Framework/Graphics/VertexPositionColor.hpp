#ifndef XNA_FRAMEWORK_GRAPHICS_VERTEXPOSITIONCOLOR_HPP
#define XNA_FRAMEWORK_GRAPHICS_VERTEXPOSITIONCOLOR_HPP

#include "IVertexType.hpp"
#include "Xna/Framework/Vector3.hpp"
#include "Xna/Framework/Color.hpp"

namespace Xna {
	//Describes a custom vertex format structure that contains position and color information.
	struct VertexPositionColor final {
		//The vertex position.
		Vector3 Position{};
		//The vertex color.
		Xna::Color Color{};

		//An array of two vertex elements describing the position, followed by the color, of this vertex.
		static const inline Xna::VertexDeclaration VertexDeclaration = Xna::VertexDeclaration({
			VertexElement(0, VertexElementFormat::Vector3, VertexElementUsage::Position, 0),
			VertexElement(12, VertexElementFormat::Color, VertexElementUsage::Color, 0)
			});

		//Initializes a new instance of the VertexPositionColor class.
		constexpr VertexPositionColor() = default;
		
		//Initializes a new instance of the VertexPositionColor class.
		constexpr VertexPositionColor(Vector3 const& position, Xna::Color const& color)	: Position(position), Color(color) {
		}

		constexpr bool operator==(const VertexPositionColor& other) const noexcept
		{
			return Position == other.Position && Color == other.Color;
		}
	};
}

#endif