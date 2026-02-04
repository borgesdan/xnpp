#ifndef XNA_FRAMEWORK_CONTENT_CONTENTSERIALIZEATTRIBUTE_HPP
#define XNA_FRAMEWORK_CONTENT_CONTENTSERIALIZEATTRIBUTE_HPP

#include <string>
#include "Xna/CSharp/Attribute.hpp"

namespace Xna {
	struct ContentSerializerAttribute : public CSharp::Attribute {
        ~ContentSerializerAttribute() = default;

        std::string ElementName;
        bool FlattenContent{false};
        bool Optional{false};
        bool AllowNull{true};
        bool SharedResource{false};
        //string.IsNullOrEmpty(this.collectionItemName) ? "Item"
        std::string CollectionItemName;

        bool HasCollectionItemName() const noexcept {
            return !CollectionItemName.empty();
        }
	};
}

#endif