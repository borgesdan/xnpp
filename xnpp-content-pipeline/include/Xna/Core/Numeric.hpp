#ifndef XNA_CORE_NUMERIC_HPP
#define XNA_CORE_NUMERIC_HPP

#include <cstdint>

namespace Xna::Core {
	struct Point {
		int32_t X{ 0 };
		int32_t Y{ 0 };
	};

	struct Rectangle {
		int32_t X{ 0 };
		int32_t Y{ 0 };
		int32_t Width{ 0 };
		int32_t Height{ 0 };
	};

	struct Vector2 {
		float X{ 0 };
		float Y{ 0 };
	};

	struct Vector3 {
		float X{ 0 };
		float Y{ 0 };
		float Z{ 0 };
	};

	struct Vector4 {
		float X{ 0 };
		float Y{ 0 };
		float Z{ 0 };
		float W{ 0 };
	};

	struct Quaternion {
		float X{ 0 };
		float Y{ 0 };
		float Z{ 0 };
		float W{ 0 };
	};

	struct Matrix {
		float M11{ 0 };
		float M12{ 0 };
		float M13{ 0 };
		float M14{ 0 };
		float M21{ 0 };
		float M22{ 0 };
		float M23{ 0 };
		float M24{ 0 };
		float M31{ 0 };
		float M32{ 0 };
		float M33{ 0 };
		float M34{ 0 };
		float M41{ 0 };
		float M42{ 0 };
		float M43{ 0 };
		float M44{ 0 };
	};
}

#endif