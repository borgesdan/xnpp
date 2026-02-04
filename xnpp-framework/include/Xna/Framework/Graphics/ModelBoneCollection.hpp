#ifndef XNA_FRAMEWORK_GRAPHICS_MODELBONECOLLECTION_HPP
#define XNA_FRAMEWORK_GRAPHICS_MODELBONECOLLECTION_HPP

#include "Xna/CSharp/Collection.hpp"
#include "_ModelBone.hpp"
#include <string>
#include <optional>

namespace Xna {
	struct ModelBoneCollection final : public CSharp::Collection<std::optional<ModelBone>>{
	public:
		~ModelBoneCollection() override {}

		ModelBone operator[](std::string const& boneName);
		bool TryGetValue(std::string const& boneName, ModelBone& value);

	private:		
		ModelBoneCollection(std::vector<std::optional<ModelBone>>& bones)
			: CSharp::Collection<std::optional<ModelBone>>(bones) {}

		friend class ModelBone;
	};
}

#endif