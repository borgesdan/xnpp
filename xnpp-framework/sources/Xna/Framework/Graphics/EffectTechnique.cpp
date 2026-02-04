#include "Xna/Framework/Graphics/Effect/EffectTechnique.hpp"

namespace Xna {
	EffectTechnique::EffectTechnique(Effect const& effect) {
		impl = std::make_shared<Implementation>();
		impl->parent = effect;
		impl->passes = EffectPassCollection();
		impl->annotations = EffectAnnotationCollection(1);
	}
}