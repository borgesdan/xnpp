#ifndef XNA_FRAMEWORK_AUDIO_UNSAFENATIVESTRUCTURES_HPP
#define XNA_FRAMEWORK_AUDIO_UNSAFENATIVESTRUCTURES_HPP

#include "Xna/Framework/Vector3.hpp"
#include <cstdint>

namespace Xna {
	class UnsafeNativeStructures final {
	public:
		static constexpr Vector3 FlipHandedness(Vector3 const& vector) {			
			return { vector.X, vector.Y, -vector.Z };
		}

		struct XACT_LISTENER_DATA {
			Vector3 _Forward;
			Vector3 _Up;
			Vector3 _Position;
			Vector3 _Velocity;			
		};

        struct XACT_EMITTER_DATA
        {            
            Vector3 _Forward;
            Vector3 _Up;
            Vector3 _Position;
            Vector3 _Velocity;
            float _innerRadius;
            float _innerRadiusAngle;
            uint32_t ChannelCount;
            float ChannelRadius;
            float CurveDistanceScaler;
            float _DopplerScale;
        };

	private:
		UnsafeNativeStructures(UnsafeNativeStructures&) = default;
		UnsafeNativeStructures(UnsafeNativeStructures&&) = default;
	};
}

#endif