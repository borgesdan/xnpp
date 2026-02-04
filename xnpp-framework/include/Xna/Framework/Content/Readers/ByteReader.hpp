#ifndef XNA_CONTENT_READERS_BYTEREADER_HPP
#define XNA_CONTENT_READERS_BYTEREADER_HPP

#include "../ContentTypeReader.hpp"
#include "../ContentReader.hpp"
#include <cstdint>

namespace Xna {
	struct ByteReader : public ContentTypeReaderT<uint8_t> {
	protected:
		inline uint8_t Read(ContentReader& input, uint8_t& existingInstance) override {
			return input.ReadByte();
		}
	};

	struct SByteReader : public ContentTypeReaderT<int8_t> {
	protected:
		inline int8_t Read(ContentReader& input, int8_t& existingInstance) override {
			return input.ReadSByte();
		}
	};
}

#endif