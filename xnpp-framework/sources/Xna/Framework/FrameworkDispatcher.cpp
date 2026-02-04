#include "Xna/Framework/FrameworkDispatcher.hpp"
#include "Xna/Framework/Audio/SoundEffectInstance.hpp"

namespace Xna {
	void FrameworkDispatcher::Update() {
		SoundEffect::RecycleStoppedFireAndForgetInstances();
	}
}