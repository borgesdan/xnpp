#ifndef XNA_FRAMEWORK_GRAPHICS__MODELMESH_HPP
#define XNA_FRAMEWORK_GRAPHICS__MODELMESH_HPP

#include <memory>
#include <string>
#include <any>
#include <vector>
#include "ModelBone.hpp"
#include "Xna/Framework/BoundingSphere.hpp"

namespace Xna {
	class ModelMeshPart;
	struct ModelMeshPartCollection;
	struct ModelEffectCollection;

	//Represents a mesh that is part of a Model.
	class ModelMesh final {
	public:
		//Gets the name of this mesh.
		inline std::string Name() const;
		//Gets the parent bone for this mesh. The parent bone of a mesh contains a transformation matrix that describes how the mesh is located relative to any parent meshes in a model.
		inline std::optional<ModelBone> ParentBone() const;
		//Gets a collection of effects associated with this mesh.
		inline std::optional<ModelEffectCollection> Effects() const;
		//Gets the ModelMeshPart objects that make up this mesh. Each part of a mesh is composed of a set of primitives that share the same material.
		inline std::optional<ModelMeshPartCollection> MeshParts() const;
		//Gets the BoundingSphere that contains this mesh.
		Xna::BoundingSphere BoundingSphere() const;
		//Gets or sets an object identifying this mesh.
		std::any Tag() const;
		//Gets or sets an object identifying this mesh.
		void Tag(std::any const& value);
		
		//Draws all of the ModelMeshPart objects in this mesh, using their current Effect settings.
		void Draw();

		inline ModelMesh(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(ModelMesh const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		ModelMesh(std::string const& name, ModelBone const& parentBone,
			Xna::BoundingSphere boundingSphere, std::vector<ModelMeshPart> const& meshParts,
			std::any const& tag);

		struct Implementation;
		std::shared_ptr<Implementation> impl;

		friend struct ModelMeshPartCollection;
		friend struct ModelEffectCollection;
	};
}

#endif