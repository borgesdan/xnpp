#ifndef XNA_CONTENT_READERS_INTREADER_HPP
#define XNA_CONTENT_READERS_INTREADER_HPP

#include "../ContentTypeReader.hpp"
#include "../ContentReader.hpp"
#include <cstdint>

namespace Xna {
	struct Int16Reader : public ContentTypeReaderT<int16_t> {
	protected:
		inline int16_t Read(ContentReader& input, int16_t& existingInstance) override {
			return input.ReadInt16();
		}
	};

	struct UInt16Reader : public ContentTypeReaderT<uint16_t> {
	protected:
		inline uint16_t Read(ContentReader& input, uint16_t& existingInstance) override {
			return input.ReadUInt16();
		}
	};

	struct Int32Reader : public ContentTypeReaderT<int32_t> {
	protected:
		inline int32_t Read(ContentReader& input, int32_t& existingInstance) override {
			return input.ReadInt32();
		}
	};

	struct UInt32Reader : public ContentTypeReaderT<uint32_t> {
	protected:
		inline uint32_t Read(ContentReader& input, uint32_t& existingInstance) override {
			return input.ReadUInt32();
		}
	};

	struct Int64Reader : public ContentTypeReaderT<int64_t> {
	protected:
		inline int64_t Read(ContentReader& input, int64_t& existingInstance) override {
			return input.ReadInt64();
		}
	};

	struct UInt64Reader : public ContentTypeReaderT<uint64_t> {
	protected:
		inline uint64_t Read(ContentReader& input, uint64_t& existingInstance) override {
			return input.ReadUInt64();
		}
	};
}

#endif