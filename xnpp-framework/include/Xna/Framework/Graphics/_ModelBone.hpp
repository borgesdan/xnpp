#ifndef XNA_FRAMEWORK_GRAPHICS__MODELBONE_HPP
#define XNA_FRAMEWORK_GRAPHICS__MODELBONE_HPP

#include "Xna/Framework/Matrix.hpp"
#include <string>
#include <vector>
#include <memory>
#include <optional>

namespace Xna {
	struct ModelBoneCollection;

	//Represents bone data for a model.
	class ModelBone final {
	public:
		//Gets the name of this bone.
		inline std::string Name() const;
		//Gets the index of this bone in the Bones collection.
		inline size_t Index() const;
		//Gets or sets the matrix used to transform this bone relative to its parent bone.
		inline Matrix Transformation() const;
		//Gets or sets the matrix used to transform this bone relative to its parent bone.
		inline void Transformation(Matrix const& value);
		//Gets the parent of this bone.
		inline std::optional<ModelBone> Parent() const;
		//Gets a collection of bones that are children of this bone.
		inline std::optional<ModelBoneCollection> Children() const;

		inline ModelBone(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(ModelBone const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }		

	private:
		ModelBone(std::string const& name, Matrix const& transform, size_t index);
		void SetParentAndChildren(ModelBone const& newParent, std::vector<std::optional<ModelBone>>& newChildren);

		struct Implementation;
		std::shared_ptr<Implementation> impl;

		friend struct ModelBoneCollection;
	};
}

#endif