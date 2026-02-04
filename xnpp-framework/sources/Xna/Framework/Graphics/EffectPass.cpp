#include "Xna/Framework/Graphics/Effect/EffectPass.hpp"

namespace Xna {
	void EffectPass::Apply() {
		Platform::Effect_EffectPass_Apply(*this);
	}
}