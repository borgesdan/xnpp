#ifndef XNA_INTERMEDIATE_XMLLISTITEMSERIALIZER_HPP
#define XNA_INTERMEDIATE_XMLLISTITEMSERIALIZER_HPP

#include "ContentTypeSerializer.hpp"

namespace Xna {
	class XmlListReader {

	};

	template <typename T>
	struct XmlListItemSerializer : public ContentTypeSerializerT<T> {
	protected:
		inline XmlListItemSerializer() {}
		inline XmlListItemSerializer(std::string const& xmlTypeName)
			:ContentTypeSerializerT<T>(CSharp::Type(typeid(T)), xmlTypeName) {}

		virtual T Deserialize(XmlListReader& input) = 0;

		virtual T Deserialize(IntermediateReader& input, ContentSerializerAttribute const& format, T& existingInstance) {
			return T();
		}
	};
}

#endif