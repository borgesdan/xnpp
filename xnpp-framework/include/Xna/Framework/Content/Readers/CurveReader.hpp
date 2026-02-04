#ifndef XNA_CONTENT_READERS_CURVEREADER_HPP
#define XNA_CONTENT_READERS_CURVEREADER_HPP

#include "../ContentTypeReader.hpp"
#include "../ContentReader.hpp"
#include "Xna/Framework/Curve.hpp"

namespace Xna {
	struct CurveReader : public ContentTypeReaderT<Curve> {
	protected:
		inline Curve Read(ContentReader& input, Curve& existingInstance) override {
            Curve curve = existingInstance;
            curve.PreLoop = static_cast<CurveLoopType>(input.ReadInt32());
            curve.PostLoop = static_cast<CurveLoopType>(input.ReadInt32());
            const auto num1 = input.ReadInt32();

            for (size_t index = 0; index < num1; ++index) {
                const auto position = input.ReadSingle();
                const auto num2 = input.ReadSingle();
                const auto tangentIn = input.ReadSingle();
                const auto tangentOut = input.ReadSingle();
                CurveContinuity continuity = static_cast<CurveContinuity>(input.ReadInt32());
                curve.Keys.Add(CurveKey(position, num2, tangentIn, tangentOut, continuity));
            }
            return curve;
		}

    public:
        bool CanDeserializeIntoExistingObject() const override {
            return true;
        }
	};
}

#endif