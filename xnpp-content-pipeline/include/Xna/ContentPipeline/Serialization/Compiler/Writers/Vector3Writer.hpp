#ifndef XNA_WRITERS_VECTOR3WRITTER_HPP
#define XNA_WRITERS_VECTOR3WRITTER_HPP

#include "../BuiltinTypeWriter.hpp"
#include <Xna/Framework/Vector3.hpp>

namespace Xna {
	struct Vector3Writer final : public BuiltinTypeWriter<Vector3> {
		std::string GetRuntimeReader(TargetPlatform targetPlatform) override {
			return "Vector3Reader";
		}

	protected:
		void WriteT(ContentWriter& output, const Vector3* value) override {
			output.Write(*value);
		}
	};
}

#endif