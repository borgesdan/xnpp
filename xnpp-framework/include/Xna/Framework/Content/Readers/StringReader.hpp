#ifndef XNA_CONTENT_READERS_STRINGREADER_HPP
#define XNA_CONTENT_READERS_STRINGREADER_HPP

#include "../ContentTypeReader.hpp"
#include "../ContentReader.hpp"
#include <string>

namespace Xna {
	struct StringReader : public ContentTypeReaderT<std::string> {
	protected:
		inline std::string Read(ContentReader& input, std::string& existingInstance) override {
			return input.ReadString();
		}
	};
}

#endif