#ifndef XNA_WRITERS_RECTANGLERITTER_HPP
#define XNA_WRITERS_RECTANGLERITTER_HPP

#include "../BuiltinTypeWriter.hpp"
#include <Xna/Framework/Rectangle.hpp>

namespace Xna {
	struct RectangleWriter final : public BuiltinTypeWriter<Rectangle> {
	public:
		std::string GetRuntimeReader(TargetPlatform targetPlatform) override {
			return "RectangleReader";
		}

	protected:
		void WriteT(ContentWriter& output, const Rectangle* value) override {
			ContentWriter::Base& output1 = output;

			output1.Write(value->X);
			output1.Write(value->Y);
			output1.Write(value->Width);
			output1.Write(value->Height);
		}		
	};
}

#endif