#ifndef XNA_FRAMEWORK_GRAPHICS_MODELBONECOLLECTION_HPP
#define XNA_FRAMEWORK_GRAPHICS_MODELBONECOLLECTION_HPP

#include "Xna/CSharp/Collection.hpp"
#include "_ModelBone.hpp"
#include <string>
#include <optional>
#include "Xna/Internal/Export.hpp"

namespace Xna {
	struct ModelBoneCollection final : public CSharp::Collection<std::optional<ModelBone>>{
	public:
		~ModelBoneCollection() override {}

		XNPP_API ModelBone operator[](std::string const& boneName);
		XNPP_API bool TryGetValue(std::string const& boneName, ModelBone& value);

	private:		
		ModelBoneCollection(std::vector<std::optional<ModelBone>>& bones)
			: CSharp::Collection<std::optional<ModelBone>>(bones) {}

		friend class ModelBone;
	};
}

#endif