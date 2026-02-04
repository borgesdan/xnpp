#ifndef XNA_CONTENT_READERS_CHARREADER_HPP
#define XNA_CONTENT_READERS_CHARREADER_HPP

#include "../ContentTypeReader.hpp"
#include "../ContentReader.hpp"
#include <cstdint>

namespace Xna {
	struct CharReader : public ContentTypeReaderT<char32_t> {
	protected:
		inline char32_t Read(ContentReader& input, char32_t& existingInstance) override {
			return input.ReadByte();
		}
	};
}

#endif