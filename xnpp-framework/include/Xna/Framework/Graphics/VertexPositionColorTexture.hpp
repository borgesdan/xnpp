#ifndef XNA_FRAMEWORK_GRAPHICS_VERTEXPOSITIONCOLORTEXTURE_HPP
#define XNA_FRAMEWORK_GRAPHICS_VERTEXPOSITIONCOLORTEXTURE_HPP

#include "IVertexType.hpp"
#include "Xna/Framework/Vector3.hpp"
#include "Xna/Framework/Color.hpp"

namespace Xna {
	//Describes a custom vertex format structure that contains position, color, and one set of texture coordinates.
	struct VertexPositionColorTexture final {
		//The vertex position.
		Vector3 Position{};
		//The vertex color.
		Xna::Color Color{};
		//The texture coordinates.
		Vector2 TextureCoordinate{};

		//An array of three vertex elements describing the position, texture coordinate, and color of this vertex.
		static const inline Xna::VertexDeclaration VertexDeclaration = Xna::VertexDeclaration({
			VertexElement(0, VertexElementFormat::Vector3, VertexElementUsage::Position, 0),
			VertexElement(12, VertexElementFormat::Color, VertexElementUsage::Color, 0),
			VertexElement(16, VertexElementFormat::Vector2, VertexElementUsage::TextureCoordinate, 0)
			});

		//Initializes a new instance of the VertexPositionColorTexture class.
		constexpr VertexPositionColorTexture() = default;

		//Initializes a new instance of the VertexPositionColorTexture class.
		constexpr VertexPositionColorTexture(Vector3 const& position, Xna::Color const& color, Vector2 const& textureCoordinate)
			: Position(position), Color(color), TextureCoordinate(textureCoordinate) {
		}

		constexpr bool operator==(const VertexPositionColorTexture& other) const noexcept
		{
			return Position == other.Position && Color == other.Color && TextureCoordinate == other.TextureCoordinate;
		}
	};
}

#endif