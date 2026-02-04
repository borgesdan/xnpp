#include "Xna/Framework/Graphics/Effect/EffectHelpers.hpp"

namespace Xna {
	Vector3 EffectHelpers::EnableDefaultLighting(DirectionalLight light0, DirectionalLight light1, DirectionalLight light2) {
        light0.Direction(Vector3(-0.5265408f, -0.5735765f, -0.6275069f));
        light0.DiffuseColor(Vector3(1.0f, 0.9607844f, 0.8078432f));
        light0.SpecularColor(Vector3(1.0f, 0.9607844f, 0.8078432f));
        light0.Enabled(true);
        light1.Direction(Vector3(0.7198464f, 0.3420201f, 0.6040227f));
        light1.DiffuseColor(Vector3(0.9647059f, 0.7607844f, 0.4078432f));
        light1.SpecularColor(Vector3::Zero());
        light1.Enabled(true);
        light2.Direction(Vector3(0.4545195f, -0.7660444f, 0.4545195f));
        light2.DiffuseColor(Vector3(0.3231373f, 0.3607844f, 0.3937255f));
        light2.SpecularColor(Vector3(0.3231373f, 0.3607844f, 0.3937255f));
        light2.Enabled(true);
        return Vector3(0.05333332f, 0.09882354f, 0.1819608f);
	}
}