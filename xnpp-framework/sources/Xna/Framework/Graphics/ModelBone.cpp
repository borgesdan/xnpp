#include "Xna/Framework/Graphics/ModelBone.hpp"

namespace Xna {
	ModelBone::ModelBone(std::string const& name, Matrix const& transform, size_t index) {
		impl = std::make_shared<Implementation>();
		impl->name = name;
		impl->transform = transform;
		impl->index = index;
	}

	void ModelBone::SetParentAndChildren(ModelBone const& newParent, std::vector<std::optional<ModelBone>>& newChildren) {
		impl->parent = newParent;
		impl->children = ModelBoneCollection(newChildren);
	}
}