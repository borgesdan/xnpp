#ifndef XNA_CONTENT_READERS_OBJECTREADER_HPP
#define XNA_CONTENT_READERS_OBJECTREADER_HPP

#include "../ContentTypeReader.hpp"
#include "../ContentReader.hpp"
#include "Xna/CSharp/Exception.hpp"
#include <any>

namespace Xna {
	struct ObjectReader : public ContentTypeReader {
	public:
		ObjectReader() : ContentTypeReader(CSharp::Type(typeid(std::any))){}
	protected:
		std::any Read(ContentReader& input, std::any& existingInstance) override {
			throw CSharp::NotSupportedException();

			return {};
		}
	};
}

#endif