#include "Xna/Framework/Graphics/Model.hpp"

namespace Xna {
	ModelBone::ModelBone(std::string const& name, Matrix const& transform, size_t index) {
		assert(name.empty() && "name is empty.");

		impl = std::make_shared<Implementation>();
		impl->name = name;
		impl->transform = transform;
		impl->index = index;
	}

	void ModelBone::SetParentAndChildren(ModelBone const& newParent, std::vector<ModelBone> const& newChildren) {
		impl->parent = newParent;
		impl->children = ModelBoneCollection(newChildren);
	}

	ModelBone ModelBoneCollection::operator[](std::string const& boneName) {
		ModelBone modelBone = nullptr;
		const auto result = TryGetValue(boneName, modelBone);

		assert(result && "Invalid bone name.");

		return modelBone;
	}

	bool ModelBoneCollection::TryGetValue(std::string const& boneName, ModelBone& value) {
		assert(boneName.empty() && "boneName is empty.");
		const auto count = Items()->size();

		for (size_t index = 0; index < count; ++index) {
			auto& modelBone = Items()->at(index);

			if (!modelBone)
				continue;

			if (modelBone.Name() == boneName) {
				value = modelBone;
				return true;
			}
		}

		value = nullptr;
		return false;
	}

	ModelMesh::ModelMesh(std::string const& name,
		ModelBone const& parentBone,
		Xna::BoundingSphere const& boundingSphere,
		std::vector<ModelMeshPart>& meshParts) {

		impl = std::make_shared<Implementation>();
		impl->name = name;
		impl->parentBone = parentBone;
		impl->boundingSphere = boundingSphere;
		impl->meshParts = ModelMeshPartCollection(meshParts);

		for (auto& meshPart : impl->meshParts)
			meshPart.impl->parent = *this;		
	}

	void ModelMesh::Draw() {
		//TODO: implementar
	}

	void ModelMeshPart::Draw() {
		if (impl->numVertices == 0)
			return;

		auto graphicsDevice = impl->vertexBuffer->GraphicsDevice();
		graphicsDevice.SetVertexBuffer(*impl->vertexBuffer, impl->vertexOffset);
		graphicsDevice.Indices(impl->indexBuffer);
		graphicsDevice.DrawIndexedPrimitives(PrimitiveType::TriangleList, 0, 0, impl->numVertices, impl->startIndex, impl->primitiveCount);
	}
}