#ifndef XNA_FRAMEWORK_GRAPHICS_VERTEXPOSITIONNORMALTEXTURE_HPP
#define XNA_FRAMEWORK_GRAPHICS_VERTEXPOSITIONNORMALTEXTURE_HPP

#include "IVertexType.hpp"
#include "Xna/Framework/Vector3.hpp"

namespace Xna {
	//Describes a custom vertex format structure that contains position, normal data, and one set of texture coordinates.
	struct VertexPositionNormalTexture final {
		//The vertex position.
		Vector3 Position{};
		//The vertex normal.
		Vector3 Normal{};
		//The texture coordinates.
		Vector2 TextureCoordinate{};

		//An array of three vertex elements describing the position, normal, and texture coordinate of this vertex.
		static const inline Xna::VertexDeclaration VertexDeclaration = Xna::VertexDeclaration({
			VertexElement(0, VertexElementFormat::Vector3, VertexElementUsage::Position, 0),
			VertexElement(12, VertexElementFormat::Vector3, VertexElementUsage::Normal, 0),
			VertexElement(24, VertexElementFormat::Vector2, VertexElementUsage::TextureCoordinate, 0)
			});

		//Initializes a new instance of the VertexPositionNormalTexture class.
		constexpr VertexPositionNormalTexture() = default;

		//Initializes a new instance of the VertexPositionNormalTexture class.
		constexpr VertexPositionNormalTexture(Vector3 const& position, Vector3 const& normal, Vector2 const& textureCoordinate)
			: Position(position), Normal(normal), TextureCoordinate(textureCoordinate) {
		}

		constexpr bool operator==(const VertexPositionNormalTexture& other) const noexcept
		{
			return Position == other.Position && Normal == other.Normal && TextureCoordinate == other.TextureCoordinate;
		}
	};
}

#endif