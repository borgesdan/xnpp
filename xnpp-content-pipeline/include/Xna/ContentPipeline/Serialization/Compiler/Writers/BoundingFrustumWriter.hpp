#ifndef XNA_WRITERS_BOUNDINGFRUSTUMWRITTER_HPP
#define XNA_WRITERS_BOUNDINGFRUSTUMWRITTER_HPP

#include "../BuiltinTypeWriter.hpp"
#include <Xna/Framework/BoundingFrustum.hpp>

namespace Xna {
	struct BoundingFrustumWriter final : public BuiltinTypeWriter<BoundingFrustum> {
		std::string GetRuntimeReader(TargetPlatform targetPlatform) override {
			return "BoundingFrustumReader";
		}
	protected:
		void WriteT(ContentWriter& output, const BoundingFrustum* value) override {
			output.Write(value->Matrix());
		}
	};
}

#endif