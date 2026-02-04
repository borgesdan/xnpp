#ifndef XNA_CONTENT_READERS_BOOLEANREADER_HPP
#define XNA_CONTENT_READERS_BOOLEANREADER_HPP

#include "../ContentTypeReader.hpp"
#include "../ContentReader.hpp"

namespace Xna {
	struct BooleanReader : public ContentTypeReaderT<bool> {
	protected:
		bool Read(ContentReader& input, bool& existingInstance) override {
			return input.ReadBoolean();
		}	
	};
}

#endif