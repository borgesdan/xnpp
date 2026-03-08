#ifndef XNA_FRAMEWORK_GRAPHICS_EFFECTPASS_HPP
#define XNA_FRAMEWORK_GRAPHICS_EFFECTPASS_HPP

#include "Xna/Platform/Platform.hpp"
#include "EffectAnnotationCollection.hpp"
#include "EffectTechnique.hpp"
#include "_EffectPass.hpp"

namespace Xna {
	struct EffectPass::Implementation {
		std::string name;
		std::optional<EffectAnnotationCollection> annotations;
	};

	inline std::string EffectPass::Name() const { return impl->name; }
	inline EffectAnnotationCollection EffectPass::Annotations() const 
	{ 
		return impl->annotations.value_or(EffectAnnotationCollection(nullptr)); 
	}
}

#endif