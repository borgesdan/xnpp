#ifndef XNA_FRAMEWORK_GRAPHICS_MODEL_HPP
#define XNA_FRAMEWORK_GRAPHICS_MODEL_HPP

#include "Xna/Framework/Matrix.hpp"
#include "Xna/Internal/Macros.hpp"
#include "Xna/CSharp/Collection.hpp"
#include "Xna/Framework/BoundingSphere.hpp"
#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"
#include "Effects/Effect.hpp"
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include "Xna/Framework/Content/ContentReader.hpp"

namespace Xna {
	class ModelBoneCollection;
	class ModelMeshPartCollection;
	class ModelEffectCollection;
	class ModelMeshPart;

	//Represents bone data for a model.
	class ModelBone final {
	public:
		//Gets the name of this bone.
		inline const std::string& Name() const { return impl->name; }
		//Gets the index of this bone in the Bones collection.
		inline size_t Index() const { return impl->index; }
		//Gets or sets the matrix used to transform this bone relative to its parent bone.
		inline const Matrix& Transformation() const { return impl->transform; }
		//Gets or sets the matrix used to transform this bone relative to its parent bone.
		inline void Transformation(Matrix const& value) { impl->transform = value; }
		//Gets the parent of this bone.
		inline const ModelBone& Parent() const { return impl->parent; }
		//Gets a collection of bones that are children of this bone.
		inline const ModelBoneCollection& Children() const;

		ModelBone(std::string const& name, Matrix const& transform, size_t index);
		void SetParentAndChildren(ModelBone const& newParent, std::vector<ModelBone> const& newChildren);

		XNPP_DECLARE_IMPL_WRAPPER(ModelBone, impl);	
	private:
		struct Implementation;
		std::shared_ptr<Implementation> impl;
	};

	//Represents a set of bones associated with a model.
	class ModelBoneCollection final : public CSharp::Collection<ModelBone> {
	public:
		~ModelBoneCollection() override = default;

		XNPP_API ModelBone operator[](std::string const& boneName);
		//Finds a bone with a given name if it exists in the collection.
		XNPP_API bool TryGetValue(std::string const& boneName, ModelBone& value);

		ModelBoneCollection(std::vector<ModelBone> const& bones)
			: CSharp::Collection<ModelBone>(bones) {}

		XNPP_DECLARE_IMPL_WRAPPER(ModelBoneCollection, m_items);
	};

	struct ModelBone::Implementation {
		std::string name;
		Matrix transform{ Matrix::Identity() };
		size_t index{ 0 };
		ModelBone parent;
		ModelBoneCollection children;
	};

	//Represents a mesh that is part of a Model.
	class ModelMesh final {
	public:
		ModelMesh(std::string const& name,
			ModelBone const& parentBone,
			Xna::BoundingSphere const& boundingSphere,
			std::vector<ModelMeshPart>& meshParts);

		inline const std::string& Name() const { return impl->name; }
		inline const ModelBone& ParentBone() const { return impl->parentBone; }
		inline const Xna::BoundingSphere& BoundingSphere() const { return impl->boundingSphere;  }
		inline const ModelMeshPartCollection& MeshParts() const;
		inline const ModelEffectCollection& Effects() const;

		void Draw();

		XNPP_DECLARE_IMPL_WRAPPER(ModelMesh, impl);
	private:
		struct Implementation;
		std::shared_ptr<Implementation> impl;
	};

	//Represents a batch of geometry information to submit to the graphics device during rendering.
	//Each ModelMeshPart is a subdivision of a ModelMesh object.
	//The ModelMesh class is split into multiple ModelMeshPart objects, typically based on material information.
	class ModelMeshPart final {
	public:
		inline ModelMeshPart(size_t vertexOffset, size_t numVertices, size_t startIndex, size_t primitiveCount)
		{
			impl = std::make_shared<Implementation>();
			impl->vertexOffset = vertexOffset;
			impl->numVertices = numVertices;
			impl->startIndex = startIndex;
			impl->primitiveCount = primitiveCount;
		}	

		inline size_t StartIndex() const { return impl->startIndex; }
		inline size_t PrimitiveCount() const { return impl->primitiveCount; }
		inline size_t VertexOffset() const { return impl->vertexOffset; }
		inline size_t NumVertices() const { return impl->numVertices; }
		inline Xna::IndexBuffer& IndexBuffer() { return *impl->indexBuffer; }
		inline Xna::VertexBuffer& VertexBuffer() { return *impl->vertexBuffer; }
		inline Xna::Effect Effect();

		XNPP_DECLARE_IMPL_WRAPPER(ModelMeshPart, impl);
	private:
		void Draw();

		struct Implementation {
			std::unique_ptr<Xna::VertexBuffer> vertexBuffer = nullptr;
			std::unique_ptr<Xna::IndexBuffer> indexBuffer = nullptr;
			size_t startIndex;
			size_t primitiveCount;
			size_t vertexOffset;
			size_t numVertices;
			std::shared_ptr<Xna::Effect> effect;
			ModelMesh parent;
		};
		std::shared_ptr<Implementation> impl;

		friend class ModelMesh;
	};	

	class ModelMeshPartCollection final : public CSharp::Collection<ModelMeshPart> {
	public:
		~ModelMeshPartCollection() override = default;

		ModelMeshPartCollection(std::vector<ModelMeshPart> const& parts)
			: CSharp::Collection<ModelMeshPart>(parts) {}

		ModelMeshPartCollection(std::vector<ModelMeshPart>& parts)
			: CSharp::Collection<ModelMeshPart>(parts) {}

		XNPP_DECLARE_IMPL_WRAPPER(ModelMeshPartCollection, m_items);
	};

	class ModelEffectCollection final : public CSharp::Collection<std::shared_ptr<Xna::Effect>> {
	public:
		~ModelEffectCollection() override = default;

		ModelEffectCollection() : CSharp::Collection<std::shared_ptr<Xna::Effect>>(){}

		ModelEffectCollection(std::vector<std::shared_ptr<Xna::Effect>> const& parts)
			: CSharp::Collection<std::shared_ptr<Xna::Effect>>(parts) {}

		XNPP_DECLARE_IMPL_WRAPPER(ModelEffectCollection, m_items);
	};

	struct ModelMesh::Implementation {
		std::string name;
		ModelBone parentBone = nullptr;
		Xna::BoundingSphere boundingSphere{};
		ModelMeshPartCollection meshParts = nullptr;
		ModelEffectCollection effects = nullptr;
	};	

	class ModelMeshCollection final : public CSharp::Collection<ModelMesh> {
	public:
		~ModelMeshCollection() override = default;

		ModelMeshCollection(std::vector<ModelMesh> const& parts)
			: CSharp::Collection<ModelMesh>(parts) {}

		ModelMeshCollection(std::vector<ModelMesh>& parts)
			: CSharp::Collection<ModelMesh>(parts) {}

		XNPP_DECLARE_IMPL_WRAPPER(ModelMeshCollection, m_items);
	};

	inline const ModelBoneCollection& ModelBone::Children() const { return impl->children; }
	inline const ModelMeshPartCollection& ModelMesh::MeshParts() const { return impl->meshParts; }
	inline const ModelEffectCollection& ModelMesh::Effects() const { return impl->effects; }

	class Model {
	public:
		inline Model() { impl = std::make_shared<Implementation>(); }
		void CopyBoneTransformsTo(std::vector<Matrix>& destinationBoneTransforms);
		void CopyAbsoluteBoneTransformsTo(std::vector<Matrix>& destinationBoneTransforms);
		void CopyBoneTransformsFrom(std::vector<Matrix>& sourceBoneTransforms);
		void Draw(Matrix const& world, Matrix const& view, Matrix const& projection);

		inline const ModelBone& Root() const { return impl->root; }
		inline const ModelBoneCollection& Bones() const { return impl->bones; }
		inline const ModelMeshCollection& Meshes() const { return impl->meshes; }

		XNPP_DECLARE_IMPL_WRAPPER(Model, impl);
	private:
		static Model Read(ContentReader& input);
		void ReadBones(ContentReader& input);
		ModelBone ReadBoneReference(ContentReader& input);
		void ReadMeshes(ContentReader& input);
		static std::vector<ModelMeshPart> ReadMeshParts(ContentReader& input);

	private:
		struct Implementation {
			ModelBone root = nullptr;
			ModelBoneCollection bones = nullptr;
			ModelMeshCollection meshes = nullptr;			
		};

		std::shared_ptr<Implementation> impl;

		static inline std::vector<Matrix> sharedDrawBoneMatrices = std::vector<Matrix>();
	};
}

#endif