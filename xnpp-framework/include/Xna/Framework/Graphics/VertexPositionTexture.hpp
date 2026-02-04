#ifndef XNA_FRAMEWORK_GRAPHICS_VERTEXPOSITIONTEXTURE_HPP
#define XNA_FRAMEWORK_GRAPHICS_VERTEXPOSITIONTEXTURE_HPP

#include "IVertexType.hpp"
#include "Xna/Framework/Vector3.hpp"

namespace Xna {
	//Describes a custom vertex format structure that contains position and one set of texture coordinates.
	struct VertexPositionTexture final {
		//The vertex position.
		Vector3 Position{};
		//The texture coordinates.
		Vector2 TextureCoordinate{};

		// 	An array of two vertex elements describing the position, followed by the texture coordinate, of this vertex.
		static const inline Xna::VertexDeclaration VertexDeclaration =Xna::VertexDeclaration({
			VertexElement(0, VertexElementFormat::Vector3, VertexElementUsage::Position, 0),
			VertexElement(12, VertexElementFormat::Vector2, VertexElementUsage::TextureCoordinate, 0)
				});

		//Initializes a new instance of the VertexPositionTexture class.
		constexpr VertexPositionTexture() = default;

		//Initializes a new instance of the VertexPositionTexture class.
		constexpr VertexPositionTexture(Vector3 const& position, Vector2 const& textureCoordinate)
			: Position(position), TextureCoordinate(textureCoordinate) {
		}

		constexpr bool operator==(const VertexPositionTexture& other) const noexcept {
			return Position == other.Position && TextureCoordinate == other.TextureCoordinate;
		}
	};
}

#endif