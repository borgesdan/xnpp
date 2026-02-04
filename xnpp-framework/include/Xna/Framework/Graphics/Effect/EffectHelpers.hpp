#ifndef XNA_FRAMEWORK_GRAPHICS_EFFECT_EFFECTHELPERS_HPP
#define XNA_FRAMEWORK_GRAPHICS_EFFECT_EFFECTHELPERS_HPP

#include "Xna/Framework/Vector3.hpp"
#include "Xna/Framework/Graphics/Effect/DirectionalLight.hpp"

namespace Xna {
	//
	// Internal class
	//

	struct EffectHelpers final {

		static Vector3 EnableDefaultLighting(DirectionalLight light0, DirectionalLight light1, DirectionalLight light2);

		EffectHelpers() = delete;
		EffectHelpers(EffectHelpers&) = delete;
		EffectHelpers(EffectHelpers&&) = delete;
	};
}

#endif