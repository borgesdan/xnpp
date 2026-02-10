#include "Xna/Framework/Graphics/Effect/EffectAnnotationCollection.hpp"
#include "Xna/Framework/Graphics/Effect/EffectAnnotation.hpp"

namespace Xna {
	EffectAnnotationCollection::EffectAnnotationCollection(size_t count) {
		impl = std::make_shared<Implementation>();
		impl->annotations.reserve(count);
	}

	EffectAnnotation EffectAnnotationCollection::operator[](size_t index) {
		if (index < Count())
			return impl->annotations[index];

		return EffectAnnotation(nullptr);
	}

	EffectAnnotation EffectAnnotationCollection::operator[](std::string const& name) {
		for (const auto& tech : impl->annotations) {
			if (tech.Name() == name)
				return tech;
		}

		return EffectAnnotation(nullptr);
	}
}