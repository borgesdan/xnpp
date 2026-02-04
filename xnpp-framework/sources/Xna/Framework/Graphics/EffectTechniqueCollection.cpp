#include "Xna/Framework/Graphics/Effect/EffectTechniqueCollection.hpp"
#include "Xna/Framework/Graphics/Effect/EffectTechnique.hpp"
#include <algorithm>

namespace Xna {
	EffectTechniqueCollection::EffectTechniqueCollection(Effect const& effect, size_t count) {
		impl = std::make_shared<Implementation>();
		impl->parent = effect;
		impl->techniques.reserve(count);
	}

	EffectTechnique EffectTechniqueCollection::operator[](size_t index) {
		if (index < Count())
			return impl->techniques[index];

		return EffectTechnique(nullptr);
	}

	EffectTechnique EffectTechniqueCollection::operator[](std::string const& name) {
		for (const auto& tech : impl->techniques) {
			if (tech.Name() == name)
				return tech;
		}

		return EffectTechnique(nullptr);
	}
}