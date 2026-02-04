#ifndef XNA_FRAMEWORK_GRAPHICS_EFFECTTECHNIQUE_HPP
#define XNA_FRAMEWORK_GRAPHICS_EFFECTTECHNIQUE_HPP

#include "_EffectTechnique.hpp"
#include "EffectPassCollection.hpp"
#include "EffectAnnotationCollection.hpp"
#include "Effect.hpp"
#include "Xna/Platform/Platform.hpp"

namespace Xna {
	struct EffectTechnique::Implementation {
		std::optional<Effect> parent;
		std::optional<EffectPassCollection> passes;
		std::optional<EffectAnnotationCollection> annotations;
		std::string name;
		PlatformImpl::EffectTechniqueImpl platformImpl;
	};

	inline std::string EffectTechnique::Name() const {
		return impl->name;
	}

	inline EffectPassCollection EffectTechnique::Passes() const {
		return impl->passes.value();
	}

	inline EffectAnnotationCollection EffectTechnique::Annotations() const {
		return impl->annotations.value();
	}
}

#endif