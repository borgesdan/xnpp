#include "Xna/Framework/Graphics/Effect/Effect.hpp"
#include "Xna/Framework/Graphics/GraphicsDevice.hpp"

namespace Xna {
	Effect::Effect(GraphicsDevice const& graphicsDevice, std::vector<uint8_t> const& effectCode) {
		Bind(graphicsDevice);
		impl = std::make_shared<Implementation>();
		//TODO: [!] Somente 1?
		impl->techniqueCollection = EffectTechniqueCollection(*this, 1);
		impl->currentTechnique = EffectTechnique(*this);
		impl->cachedEffectData = effectCode;

		impl->techniqueCollection.impl->techniques.push_back(impl->currentTechnique);
	}
}