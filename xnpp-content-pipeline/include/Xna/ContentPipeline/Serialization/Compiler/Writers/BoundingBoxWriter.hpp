#ifndef XNA_WRITERS_BOUNDINGBOXWRITTER_HPP
#define XNA_WRITERS_BOUNDINGBOXWRITTER_HPP

#include "../BuiltinTypeWriter.hpp"
#include <Xna/Framework/BoundingBox.hpp>

namespace Xna {
	struct BoundingBoxWriter final : public BuiltinTypeWriter<BoundingBox> {
		std::string GetRuntimeReader(TargetPlatform targetPlatform) override{
			return "BoundingBoxReader";
		}
	protected:
		void WriteT(ContentWriter& output, const BoundingBox* value) override {
			output.Write(value->Min);
			output.Write(value->Max);
		}
	};
}

#endif