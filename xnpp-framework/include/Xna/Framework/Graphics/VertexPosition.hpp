#ifndef XNA_FRAMEWORK_GRAPHICS_VERTEXPOSITIONCOLOR_HPP
#define XNA_FRAMEWORK_GRAPHICS_VERTEXPOSITIONCOLOR_HPP

#include "IVertexType.hpp"
#include "Xna/Framework/Vector3.hpp"
#include "Xna/Framework/Color.hpp"
#include "Texture2D.hpp"

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

		constexpr bool operator==(const VertexPositionColor& other) const noexcept = default;
	};

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

		constexpr bool operator==(const VertexPositionColorTexture& other) const noexcept = default;
	};

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

		constexpr bool operator==(const VertexPositionNormalTexture& other) const noexcept = default;
	};

	//Describes a custom vertex format structure that contains position and one set of texture coordinates.
	struct VertexPositionTexture final {
		//The vertex position.
		Vector3 Position{};
		//The texture coordinates.
		Vector2 TextureCoordinate{};

		// 	An array of two vertex elements describing the position, followed by the texture coordinate, of this vertex.
		static const inline Xna::VertexDeclaration VertexDeclaration = Xna::VertexDeclaration({
			VertexElement(0, VertexElementFormat::Vector3, VertexElementUsage::Position, 0),
			VertexElement(12, VertexElementFormat::Vector2, VertexElementUsage::TextureCoordinate, 0)
			});

		//Initializes a new instance of the VertexPositionTexture class.
		constexpr VertexPositionTexture() = default;

		//Initializes a new instance of the VertexPositionTexture class.
		constexpr VertexPositionTexture(Vector3 const& position, Vector2 const& textureCoordinate)
			: Position(position), TextureCoordinate(textureCoordinate) {
		}

		constexpr bool operator==(const VertexPositionTexture& other) const noexcept = default;
	};
}

#endif