#include "Xna/Framework/Graphics/ModelBoneCollection.hpp"
#include "Xna/Framework/Graphics/ModelBone.hpp"
#include "Xna/CSharp/Exception.hpp"

namespace Xna {
	ModelBone ModelBoneCollection::operator[](std::string const& boneName) {
		ModelBone modelBone = nullptr;
		
		if (TryGetValue(boneName, modelBone))
			throw CSharp::InvalidOperationException("KeyNotFoundException");

		return modelBone;
	}

	bool ModelBoneCollection::TryGetValue(std::string const& boneName, ModelBone& value) {
        if (boneName.empty())
            throw CSharp::ArgumentNullException("boneName");

        const auto count = Items()->size();

        for (size_t index = 0; index < count; ++index) {
            auto modelBone = Items()->at(index);
            
            if (!modelBone)
                continue;

            if (modelBone->Name() == boneName) {
                value = *modelBone;
                return true;
            }
        }
        value = nullptr;
        return false;
	}
}