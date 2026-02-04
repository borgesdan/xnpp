#ifndef XNA_FRAMEWORK_GRAPHICS_EFFECT_HPP
#define XNA_FRAMEWORK_GRAPHICS_EFFECT_HPP

#include "_Effect.hpp"
#include "EffectTechniqueCollection.hpp"
#include "EffectParameterCollection.hpp"
#include "EffectTechnique.hpp"

namespace Xna {
	struct Effect::Implementation {
		EffectTechniqueCollection techniqueCollection{nullptr};
		EffectParameterCollection paramCollection{nullptr};
		EffectTechnique currentTechnique{ nullptr };
		std::vector<uint8_t> cachedEffectData;
	};

	

	inline EffectTechnique Effect::CurrentTechnique() const {
		return impl->currentTechnique;
	}

	inline void Effect::CurrentTechnique(EffectTechnique const& value) {
		impl->currentTechnique = value;
	}

	inline EffectTechniqueCollection Effect::Techniques() const {
		return impl->techniqueCollection;
	}

	inline EffectParameterCollection Effect::Parameters() const {
		return impl->paramCollection;
	}
}

#endif