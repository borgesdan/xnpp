#ifndef XNA_WRITERS_PRIMITIVESWRITER_HPP
#define XNA_WRITERS_PRIMITIVESWRITER_HPP

#include "../BuiltinTypeWriter.hpp"
#include <cstdint>

namespace Xna {
	struct BooleanWriter final : public BuiltinTypeWriter<bool> {
		inline std::string GetRuntimeReader(TargetPlatform targetPlatform) override {
			return "BooleanReader";
		}
	protected:
		inline void WriteT(ContentWriter& output, const bool* value) override {
			CSharp::BinaryWriter& output1 = output;
			output1.Write(*value);
		}
	};

	struct ByteWriter final : public BuiltinTypeWriter<uint8_t> {
		inline std::string GetRuntimeReader(TargetPlatform targetPlatform) override {
			return "ByteReader";
		}
	protected:
		inline void WriteT(ContentWriter& output, const uint8_t* value) override {
			CSharp::BinaryWriter& output1 = output;
			output1.Write(*value);
		}
	};

	struct CharWriter final : public BuiltinTypeWriter<char> {
		inline std::string GetRuntimeReader(TargetPlatform targetPlatform) override {
			return "CharReader";
		}
	protected:
		inline void WriteT(ContentWriter& output, const char* value) override {
			CSharp::BinaryWriter& output1 = output;
			output1.Write(*value);
		}
	};

	struct Int16Writer final : public BuiltinTypeWriter<int16_t> {
		inline std::string GetRuntimeReader(TargetPlatform targetPlatform) override {
			return "Int16Reader";
		}
	protected:
		inline void WriteT(ContentWriter& output, const int16_t* value) override {
			CSharp::BinaryWriter& output1 = output;
			output1.Write(*value);
		}
	};

	struct Int32Writer final : public BuiltinTypeWriter<int32_t> {
		inline std::string GetRuntimeReader(TargetPlatform targetPlatform) override {
			return "Int32Reader";
		}
	protected:
		inline void WriteT(ContentWriter& output, const int32_t* value) override {
			CSharp::BinaryWriter& output1 = output;
			output1.Write(*value);
		}
	};

	struct Int64Writer final : public BuiltinTypeWriter<int64_t> {
		inline std::string GetRuntimeReader(TargetPlatform targetPlatform) override {
			return "Int64Reader";
		}
	protected:
		inline void WriteT(ContentWriter& output, const int64_t* value) override {
			CSharp::BinaryWriter& output1 = output;
			output1.Write(*value);
		}
	};

	struct UInt16Writer final : public BuiltinTypeWriter<uint16_t> {
		inline std::string GetRuntimeReader(TargetPlatform targetPlatform) override {
			return "UInt16Reader";
		}
	protected:
		inline void WriteT(ContentWriter& output, const uint16_t* value) override {
			CSharp::BinaryWriter& output1 = output;
			output1.Write(*value);
		}
	};

	struct UInt32Writer final : public BuiltinTypeWriter<uint32_t> {
		inline std::string GetRuntimeReader(TargetPlatform targetPlatform) override {
			return "UInt32Reader";
		}
	protected:
		inline void WriteT(ContentWriter& output, const uint32_t* value) override {
			CSharp::BinaryWriter& output1 = output;
			output1.Write(*value);
		}
	};

	struct UInt64Writer final : public BuiltinTypeWriter<uint64_t> {
		inline std::string GetRuntimeReader(TargetPlatform targetPlatform) override {
			return "UInt32Reader";
		}
	protected:
		inline void WriteT(ContentWriter& output, const uint64_t* value) override {
			CSharp::BinaryWriter& output1 = output;
			output1.Write(*value);
		}
	};
}

#endif