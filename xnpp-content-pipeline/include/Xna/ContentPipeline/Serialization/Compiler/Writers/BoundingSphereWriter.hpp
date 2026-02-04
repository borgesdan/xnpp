#ifndef XNA_WRITERS_BOUNDINGSPHEREWRITTER_HPP
#define XNA_WRITERS_BOUNDINGSPHEREWRITTER_HPP

#include "../BuiltinTypeWriter.hpp"
#include <Xna/Framework/BoundingSphere.hpp>

namespace Xna {
	struct BoundingSphereWriter final : public BuiltinTypeWriter<BoundingSphere> {
		std::string GetRuntimeReader(TargetPlatform targetPlatform) override {
			return "BoundingSphereReader";
		}
	protected:
		void WriteT(ContentWriter& output, const BoundingSphere* value) override {
			output.Write(value->Center);
			
			CSharp::BinaryWriter& output1 = output;
			output1.Write(value->Radius);
		}
	};
}

#endif