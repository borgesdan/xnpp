#ifndef XNA_FRAMEWORK_GRAPHICS_VERTEXDECLARATION_HPP
#define XNA_FRAMEWORK_GRAPHICS_VERTEXDECLARATION_HPP

#include "VertexElement.hpp"
#include <vector>
#include <memory>
#include "Xna/Framework/Graphics/VertexElementValidator.hpp"

namespace Xna {
	//A vertex declaration, which defines per-vertex data.
	struct VertexDeclaration final{
	public:
		//Initializes a new instance of the VertexDeclaration class.
		constexpr VertexDeclaration() = default;

		//Initializes a new instance of the VertexDeclaration class.
		constexpr VertexDeclaration(size_t vertexStride, std::vector<VertexElement>& elements)
			: vertexStride(vertexStride), elements(std::move(elements)) {

			//TODO:[!] implementar
			//VertexElementValidator.Validate(vertexStride, elements1);
		}

		//Initializes a new instance of the VertexDeclaration class.
		constexpr VertexDeclaration(std::vector<VertexElement>& elements)
			: elements(std::move(elements)), vertexStride(VertexElementValidator::GetVertexStride(this->elements)) {

			//TODO:[!] implementar		
			//VertexElementValidator.Validate(vertexStride, elements1);
		}

		//Initializes a new instance of the VertexDeclaration class.		
		constexpr VertexDeclaration(size_t vertexStride, std::vector<VertexElement> const& elements)
			: vertexStride(vertexStride), elements(elements) {

			//TODO:[!] implementar
			//VertexElementValidator.Validate(vertexStride, elements1);
		}

		//Initializes a new instance of the VertexDeclaration class.
		constexpr VertexDeclaration(std::vector<VertexElement> const& elements)
			: elements(elements), vertexStride(VertexElementValidator::GetVertexStride(this->elements)) {

			//TODO:[!] implementar		
			//VertexElementValidator.Validate(vertexStride, elements1);
		}			

		//The number of bytes from one vertex to the next.
		constexpr size_t VertexStride() const noexcept { return vertexStride; }
		
		//Gets the vertex shader declaration.
		const std::vector<VertexElement>& GetVertexElements() const {
			return elements;
		}

	private:
		std::vector<VertexElement> elements;
		size_t vertexStride;
	};
}

#endif