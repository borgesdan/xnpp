#ifndef XNA_FRAMEWORK_GRAPHICS_MODELBONE_HPP
#define XNA_FRAMEWORK_GRAPHICS_MODELBONE_HPP

#include "_ModelBone.hpp"
#include "ModelBoneCollection.hpp"

namespace Xna {
	struct ModelBone::Implementation {
		std::string name;
		Matrix transform{Matrix::Identity()};
		size_t index{0};
		std::optional<ModelBone> parent;
		std::optional<ModelBoneCollection> children;
	};

	inline std::string ModelBone::Name() const { return impl->name; }
	inline size_t ModelBone::Index() const { return impl->index; }
	inline Matrix ModelBone::Transformation() const { return impl->transform; }
	inline void ModelBone::Transformation(Matrix const& value) { impl->transform = value; }
	inline std::optional<ModelBone> ModelBone::Parent() const { return impl->parent; }
	inline std::optional<ModelBoneCollection> ModelBone::Children() const { return impl->children; }
	
}

#endif