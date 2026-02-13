#ifndef XNA_FRAMEWORK_GRAPHICS_EFFECTANNOTATION_HPP
#define XNA_FRAMEWORK_GRAPHICS_EFFECTANNOTATION_HPP

#include "_EffectAnnotation.hpp"
#include "Xna/Platform/Platform.hpp"

namespace Xna {
	struct EffectAnnotation::Implementation {        
        std::string name;
        std::string semantic;
        int32_t rows;
        int32_t columns;
        EffectParameterClass paramClass;
        EffectParameterType paramType;
        EffectParameterTypeVariant variant;
	};

	inline std::string EffectAnnotation::Name() const { return impl->name; }
	inline std::string EffectAnnotation::Semantic() const { return impl->semantic; }
	inline size_t EffectAnnotation::RowCount() const { return impl->rows; }
	inline size_t EffectAnnotation::ColumnCount() const { return impl->columns; }
	inline EffectParameterClass EffectAnnotation::ParameterClass() const { return impl->paramClass; }
	inline EffectParameterType EffectAnnotation::ParameterType() const { return impl->paramType; }
}

#endif