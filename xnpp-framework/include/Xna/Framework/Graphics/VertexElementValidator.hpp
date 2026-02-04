#ifndef XNA_FRAMEWORK_GRAPHICS_VERTEXELEMENTVALIDATOR_HPP
#define XNA_FRAMEWORK_GRAPHICS_VERTEXELEMENTVALIDATOR_HPP

#include "Shared.hpp"
#include "VertexElement.hpp"
#include <memory>

namespace Xna {

    //
    //Internal class
    //

	struct VertexElementValidator final {
		static constexpr size_t GetTypeSize(VertexElementFormat format) noexcept {
            switch (format)
            {
            case VertexElementFormat::Single:
            case VertexElementFormat::Color:
            case VertexElementFormat::Byte4:
            case VertexElementFormat::Short2:
            case VertexElementFormat::NormalizedShort2:
            case VertexElementFormat::HalfVector2:
                return 4;                            
            case VertexElementFormat::Vector2:
            case VertexElementFormat::Short4:
            case VertexElementFormat::NormalizedShort4:
            case VertexElementFormat::HalfVector4:
                return 8;                
            case VertexElementFormat::Vector3:
                return 12;
            case VertexElementFormat::Vector4:
                return 16;                
            default:
                return 0;
            }
		}

        constexpr static size_t GetVertexStride(std::vector<VertexElement> const& elements) {
            size_t vertexStride = 0;
            const auto size = elements.size();

            for (size_t index = 0; index < size; ++index) {
                const auto num = elements[index].Offset + GetTypeSize(elements[index].VertexElementFormat);
                
                if (vertexStride < num)
                    vertexStride = num;
            }
            return vertexStride;
        }

		VertexElementValidator() = delete;
		VertexElementValidator(VertexElementValidator&) = delete;
		VertexElementValidator(VertexElementValidator&&) = delete;
	};

	
}

#endif