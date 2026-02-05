#ifndef XNA_FRAMEWORK_QUATERNION_HPP
#define XNA_FRAMEWORK_QUATERNION_HPP

#include "Matrix.hpp"
#include "MathHelper.hpp"

namespace Xna {
	//Defines a four-dimensional vector (x,y,z,w), which is used to efficiently rotate an object about the (x, y, z) vector by the angle theta, where w = cos(theta/2).
	struct Quaternion final {
		//Specifies the x-value of the vector component of the quaternion.
		float X{ 0 };
		//Specifies the y-value of the vector component of the quaternion.
		float Y{ 0 };
		//Specifies the z-value of the vector component of the quaternion.
		float Z{ 0 };
		//Specifies the rotation component of the quaternion.
		float W{ 0 };

		//Initializes a new instance of Quaternion.
		constexpr Quaternion() = default;

		//Initializes a new instance of Quaternion.
		constexpr Quaternion(float X, float Y, float Z, float W)
			: X(X), Y(Y), Z(Z), W(W) {
		}

		//Initializes a new instance of Quaternion.
		constexpr Quaternion(Vector3 vectorPart, float scalarPart)
			: X(vectorPart.X), Y(vectorPart.Y), Z(vectorPart.Z), W(scalarPart) {
		}		

		//Returns a Quaternion representing no rotation.
		static constexpr Quaternion Identity() {
			return { 0.0f, 0.0f, 0.0f, 1.0f };
		}

		//Calculates the length squared of a Quaternion.
		float constexpr LengthSquared() const {
			return X * X + Y * Y + Z * Z + W * W;
		}

		//Calculates the length of a Quaternion.
		constexpr float Length() const { return MathHelper::Extensions::Sqrt(LengthSquared()); }

		//Divides each component of a quaternion by the length of the quaternion.
		constexpr void Normalize() {
			const auto num = 1.0F / Length();
			X *= num;
			Y *= num;
			Z *= num;
			W *= num;
		}

		//Divides each component of a quaternion by the length of the quaternion.
		static constexpr Quaternion Normalize(Quaternion quaternion) {
			quaternion.Normalize();
			return quaternion;
		}

		//Calculates the conjugate of a Quaternion.
		constexpr void Conjugate() {
			X = -X;
			Y = -Y;
			Z = -Z;
		}

		//Calculates the conjugate of a Quaternion.
		static constexpr Quaternion Conjugate(Quaternion const& value) {
			Quaternion quaternion;
			quaternion.X = -value.X;
			quaternion.Y = -value.Y;
			quaternion.Z = -value.Z;
			quaternion.W = value.W;
			return quaternion;
		}

		//Returns the inverse of a Quaternion.
		static constexpr Quaternion Inverse(Quaternion const& quaternion) {
			const auto num = 1.0f / quaternion.LengthSquared();
			Quaternion quaternion1;
			quaternion1.X = -quaternion.X * num;
			quaternion1.Y = -quaternion.Y * num;
			quaternion1.Z = -quaternion.Z * num;
			quaternion1.W = quaternion.W * num;
			return quaternion1;
		}

		//Creates a Quaternion from a vector and an angle to rotate about the vector.
		static constexpr Quaternion CreateFromAxisAngle(Vector3 const& axis, float angle) {
			const auto num1 = angle * 0.5f;
			const auto num2 = MathHelper::Extensions::Sin(num1);
			const auto num3 = MathHelper::Extensions::Cos(num1);
			Quaternion fromAxisAngle;
			fromAxisAngle.X = axis.X * num2;
			fromAxisAngle.Y = axis.Y * num2;
			fromAxisAngle.Z = axis.Z * num2;
			fromAxisAngle.W = num3;
			return fromAxisAngle;
		}
		//Creates a new Quaternion from specified yaw, pitch, and roll angles.
		static constexpr Quaternion CreateFromYawPitchRoll(float yaw, float pitch, float roll) {
			const auto num1 = roll * 0.5f;
			const auto num2 = MathHelper::Extensions::Sin(num1);
			const auto num3 = MathHelper::Extensions::Cos(num1);
			const auto num4 = pitch * 0.5f;
			const auto num5 = MathHelper::Extensions::Sin(num4);
			const auto num6 = MathHelper::Extensions::Cos(num4);
			const auto num7 = yaw * 0.5f;
			const auto num8 = MathHelper::Extensions::Sin(num7);
			const auto num9 = MathHelper::Extensions::Cos(num7);
			Quaternion fromYawPitchRoll;
			fromYawPitchRoll.X = (num9 * num5 * num3 + num8 * num6 * num2);
			fromYawPitchRoll.Y = (num8 * num6 * num3 - num9 * num5 * num2);
			fromYawPitchRoll.Z = (num9 * num6 * num2 - num8 * num5 * num3);
			fromYawPitchRoll.W = (num9 * num6 * num3 + num8 * num5 * num2);
			return fromYawPitchRoll;
		}
		//Creates a Quaternion from a rotation Matrix.
		static constexpr Quaternion CreateFromRotationMatrix(Matrix const& matrix) {
			const auto num1 = matrix.M11 + matrix.M22 + matrix.M33;

			Quaternion fromRotationMatrix;
			if (num1 > 0.0)
			{
				const auto num2 = MathHelper::Extensions::Sqrt(num1 + 1.0F);
				fromRotationMatrix.W = num2 * 0.5f;
				const auto num3 = 0.5f / num2;
				fromRotationMatrix.X = (matrix.M23 - matrix.M32) * num3;
				fromRotationMatrix.Y = (matrix.M31 - matrix.M13) * num3;
				fromRotationMatrix.Z = (matrix.M12 - matrix.M21) * num3;
			}
			else if (matrix.M11 >= matrix.M22 && matrix.M11 >= matrix.M33)
			{
				const auto num4 = MathHelper::Extensions::Sqrt(1.0F + matrix.M11 - matrix.M22 - matrix.M33);
				const auto num5 = 0.5f / num4;
				fromRotationMatrix.X = 0.5f * num4;
				fromRotationMatrix.Y = (matrix.M12 + matrix.M21) * num5;
				fromRotationMatrix.Z = (matrix.M13 + matrix.M31) * num5;
				fromRotationMatrix.W = (matrix.M23 - matrix.M32) * num5;
			}
			else if (matrix.M22 > matrix.M33)
			{
				const auto num6 = MathHelper::Extensions::Sqrt(1.0F + matrix.M22 - matrix.M11 - matrix.M33);
				const auto num7 = 0.5f / num6;
				fromRotationMatrix.X = (matrix.M21 + matrix.M12) * num7;
				fromRotationMatrix.Y = 0.5f * num6;
				fromRotationMatrix.Z = (matrix.M32 + matrix.M23) * num7;
				fromRotationMatrix.W = (matrix.M31 - matrix.M13) * num7;
			}
			else
			{
				const auto num8 = MathHelper::Extensions::Sqrt(1.0F + matrix.M33 - matrix.M11 - matrix.M22);
				const auto num9 = 0.5f / num8;
				fromRotationMatrix.X = (matrix.M31 + matrix.M13) * num9;
				fromRotationMatrix.Y = (matrix.M32 + matrix.M23) * num9;
				fromRotationMatrix.Z = 0.5f * num8;
				fromRotationMatrix.W = (matrix.M12 - matrix.M21) * num9;
			}
			return fromRotationMatrix;
		}

		//Calculates the dot product of two Quaternions.
		static constexpr float Dot(Quaternion const& quaternion1, Quaternion const& quaternion2) {
			return quaternion1.X * quaternion2.X + quaternion1.Y * quaternion2.Y + quaternion1.Z * quaternion2.Z + quaternion1.W * quaternion2.W;
		}

		//Interpolates between two quaternions, using spherical linear interpolation.
		static constexpr Quaternion Slerp(Quaternion const& quaternion1, Quaternion const& quaternion2, float amount) {
			const auto num1 = amount;
			auto d = quaternion1.X * quaternion2.X + quaternion1.Y * quaternion2.Y + quaternion1.Z * quaternion2.Z + quaternion1.W * quaternion2.W;
			bool flag = false;

			if (d < 0.0) {
				flag = true;
				d = -d;
			}

			float num2 = 0;
			float num3 = 0;

			if (d > 0.99999898672103882) {
				num2 = 1.0f - num1;
				num3 = flag ? -num1 : num1;
			}
			else {
				const auto a = std::acos(d);
				const auto num4 = 1.0F / MathHelper::Extensions::Sin(a);
				num2 = MathHelper::Extensions::Sin((1.0F - num1) * a) * num4;
				num3 = flag ? -MathHelper::Extensions::Sin(num1 * a) * num4 : MathHelper::Extensions::Sin(num1 * a) * num4;
			}
			Quaternion quaternion;
			quaternion.X = num2 * quaternion1.X + num3 * quaternion2.X;
			quaternion.Y = num2 * quaternion1.Y + num3 * quaternion2.Y;
			quaternion.Z = num2 * quaternion1.Z + num3 * quaternion2.Z;
			quaternion.W = num2 * quaternion1.W + num3 * quaternion2.W;
			return quaternion;
		}
		//Linearly interpolates between two quaternions.
		static constexpr Quaternion Lerp(Quaternion const& quaternion1, Quaternion const& quaternion2, float amount) {
			const auto num1 = amount;
			const auto num2 = 1.0f - num1;
			Quaternion quaternion;

			if (quaternion1.X * quaternion2.X + quaternion1.Y * quaternion2.Y + quaternion1.Z * quaternion2.Z + quaternion1.W * quaternion2.W >= 0.0) {
				quaternion.X = num2 * quaternion1.X + num1 * quaternion2.X;
				quaternion.Y = num2 * quaternion1.Y + num1 * quaternion2.Y;
				quaternion.Z = num2 * quaternion1.Z + num1 * quaternion2.Z;
				quaternion.W = num2 * quaternion1.W + num1 * quaternion2.W;
			}
			else {
				quaternion.X = num2 * quaternion1.X - num1 * quaternion2.X;
				quaternion.Y = num2 * quaternion1.Y - num1 * quaternion2.Y;
				quaternion.Z = num2 * quaternion1.Z - num1 * quaternion2.Z;
				quaternion.W = num2 * quaternion1.W - num1 * quaternion2.W;
			}
			const auto num3 = 1.0f / MathHelper::Extensions::Sqrt(quaternion.X * quaternion.X + quaternion.Y * quaternion.Y + quaternion.Z * quaternion.Z + quaternion.W * quaternion.W);
			quaternion.X *= num3;
			quaternion.Y *= num3;
			quaternion.Z *= num3;
			quaternion.W *= num3;
			return quaternion;
		}

		//Concatenates two Quaternions; the result represents the first rotation followed by the second rotation.
		static constexpr Quaternion Concatenate(Quaternion const& value1, Quaternion const& value2) {
			const auto x1 = value2.X;
			const auto y1 = value2.Y;
			const auto z1 = value2.Z;
			const auto w1 = value2.W;
			const auto x2 = value1.X;
			const auto y2 = value1.Y;
			const auto z2 = value1.Z;
			const auto w2 = value1.W;
			const auto num1 = y1 * z2 - z1 * y2;
			const auto num2 = z1 * x2 - x1 * z2;
			const auto num3 = x1 * y2 - y1 * x2;
			const auto num4 = x1 * x2 + y1 * y2 + z1 * z2;
			Quaternion quaternion;
			quaternion.X = (x1 * w2 + x2 * w1) + num1;
			quaternion.Y = (y1 * w2 + y2 * w1) + num2;
			quaternion.Z = (z1 * w2 + z2 * w1) + num3;
			quaternion.W = w1 * w2 - num4;
			return quaternion;
		}

		//Flips the sign of each component of the quaternion.
		static constexpr Quaternion Negate(Quaternion const& quaternion) {
			Quaternion quaternion1;
			quaternion1.X = -quaternion.X;
			quaternion1.Y = -quaternion.Y;
			quaternion1.Z = -quaternion.Z;
			quaternion1.W = -quaternion.W;
			return quaternion1;
		}

		//Adds two Quaternions.
		static constexpr Quaternion Add(Quaternion const& quaternion1, Quaternion const& quaternion2) {
			Quaternion quaternion;
			quaternion.X = quaternion1.X + quaternion2.X;
			quaternion.Y = quaternion1.Y + quaternion2.Y;
			quaternion.Z = quaternion1.Z + quaternion2.Z;
			quaternion.W = quaternion1.W + quaternion2.W;
			return quaternion;
		}

		//Subtracts a quaternion from another quaternion.
		static constexpr Quaternion Subtract(Quaternion const& quaternion1, Quaternion const& quaternion2) {
			Quaternion quaternion;
			quaternion.X = quaternion1.X - quaternion2.X;
			quaternion.Y = quaternion1.Y - quaternion2.Y;
			quaternion.Z = quaternion1.Z - quaternion2.Z;
			quaternion.W = quaternion1.W - quaternion2.W;
			return quaternion;
		}

		//Multiplies a quaternion by a scalar or another quaternion.
		static constexpr Quaternion Multiply(Quaternion const& quaternion1, Quaternion const& quaternion2) {
			const auto x1 = quaternion1.X;
			const auto y1 = quaternion1.Y;
			const auto z1 = quaternion1.Z;
			const auto w1 = quaternion1.W;
			const auto x2 = quaternion2.X;
			const auto y2 = quaternion2.Y;
			const auto z2 = quaternion2.Z;
			const auto w2 = quaternion2.W;
			const auto num1 = y1 * z2 - z1 * y2;
			const auto num2 = z1 * x2 - x1 * z2;
			const auto num3 = x1 * y2 - y1 * x2;
			const auto num4 = x1 * x2 + y1 * y2 + z1 * z2;
			Quaternion quaternion;
			quaternion.X = (x1 * w2 + x2 * w1) + num1;
			quaternion.Y = (y1 * w2 + y2 * w1) + num2;
			quaternion.Z = (z1 * w2 + z2 * w1) + num3;
			quaternion.W = w1 * w2 - num4;
			return quaternion;
		}

		//Multiplies a quaternion by a scalar or another quaternion.
		static constexpr Quaternion Multiply(Quaternion const& quaternion1, float scaleFactor) {
			Quaternion quaternion;
			quaternion.X = quaternion1.X * scaleFactor;
			quaternion.Y = quaternion1.Y * scaleFactor;
			quaternion.Z = quaternion1.Z * scaleFactor;
			quaternion.W = quaternion1.W * scaleFactor;
			return quaternion;
		}

		//Divides a Quaternion by another Quaternion.
		static constexpr Quaternion Divide(Quaternion const& quaternion1, Quaternion const& quaternion2) {
			const auto x = quaternion1.X;
			const auto y = quaternion1.Y;
			const auto z = quaternion1.Z;
			const auto w = quaternion1.W;
			const auto num1 = 1.0f / (quaternion2.X * quaternion2.X + quaternion2.Y * quaternion2.Y + quaternion2.Z * quaternion2.Z + quaternion2.W * quaternion2.W);
			const auto num2 = -quaternion2.X * num1;
			const auto num3 = -quaternion2.Y * num1;
			const auto num4 = -quaternion2.Z * num1;
			const auto num5 = quaternion2.W * num1;
			const auto num6 = y * num4 - z * num3;
			const auto num7 = z * num2 - x * num4;
			const auto num8 = x * num3 - y * num2;
			const auto num9 = x * num2 + y * num3 + z * num4;
			Quaternion quaternion;
			quaternion.X = (x * num5 + num2 * w) + num6;
			quaternion.Y = (y * num5 + num3 * w) + num7;
			quaternion.Z = (z * num5 + num4 * w) + num8;
			quaternion.W = w * num5 - num9;
			return quaternion;
		}

		constexpr Quaternion operator-(Quaternion const& quaternion) const {
			return Quaternion::Negate(quaternion);
		}

		constexpr friend Quaternion operator+(Quaternion const& q1, Quaternion const& q2) {
			return Quaternion::Add(q1, q2);
		}

		constexpr friend Quaternion operator-(Quaternion const& q1, Quaternion const& q2) {
			return Quaternion::Add(q1, q2);
		}

		constexpr friend Quaternion operator*(Quaternion const& q1, Quaternion const& q2) {
			return Quaternion::Multiply(q1, q2);
		}

		constexpr friend Quaternion operator/(Quaternion const& q1, Quaternion const& q2) {
			return Quaternion::Divide(q1, q2);
		}

		constexpr friend Quaternion operator*(Quaternion const& q1, float scaleValue) {
			return Quaternion::Multiply(q1, scaleValue);
		}

		constexpr friend Quaternion operator*(float scaleValue, Quaternion const& q1) {
			return Quaternion::Multiply(q1, scaleValue);
		}

		constexpr bool operator==(const Quaternion& other) const {
			return X == other.X && Y == other.Y && Z == other.Z && W == other.W;
		}

		constexpr operator std::optional<Quaternion>() const {
			return std::make_optional<Quaternion>(X, Y, Z, W);
		}
	};

	constexpr Vector3 Vector3::Transform(Vector3 const& value, Quaternion const& rotation)
	{
		const auto num1 = rotation.X + rotation.X;
		const auto num2 = rotation.Y + rotation.Y;
		const auto num3 = rotation.Z + rotation.Z;
		const auto num4 = rotation.W * num1;
		const auto num5 = rotation.W * num2;
		const auto num6 = rotation.W * num3;
		const auto num7 = rotation.X * num1;
		const auto num8 = rotation.X * num2;
		const auto num9 = rotation.X * num3;
		const auto num10 = rotation.Y * num2;
		const auto num11 = rotation.Y * num3;
		const auto num12 = rotation.Z * num3;
		const auto num13 = (value.X * (1.0F - num10 - num12) + value.Y * (num8 - num6) + value.Z * (num9 + num5));
		const auto num14 = (value.X * (num8 + num6) + value.Y * (1.0F - num7 - num12) + value.Z * (num11 - num4));
		const auto num15 = (value.X * (num9 - num5) + value.Y * (num11 + num4) + value.Z * (1.0F - num7 - num10));
		Vector3 vector3;
		vector3.X = num13;
		vector3.Y = num14;
		vector3.Z = num15;
		return vector3;
	}

	constexpr Vector4 Vector4::Transform(Vector2 const& value, Quaternion const& rotation)
	{
		const auto num1 = rotation.X + rotation.X;
		const auto num2 = rotation.Y + rotation.Y;
		const auto num3 = rotation.Z + rotation.Z;
		const auto num4 = rotation.W * num1;
		const auto num5 = rotation.W * num2;
		const auto num6 = rotation.W * num3;
		const auto num7 = rotation.X * num1;
		const auto num8 = rotation.X * num2;
		const auto num9 = rotation.X * num3;
		const auto num10 = rotation.Y * num2;
		const auto num11 = rotation.Y * num3;
		const auto num12 = rotation.Z * num3;
		const auto num13 = (value.X * (1.0F - num10 - num12) + value.Y * (num8 - num6));
		const auto num14 = (value.X * (num8 + num6) + value.Y * (1.0F - num7 - num12));
		const auto num15 = (value.X * (num9 - num5) + value.Y * (num11 + num4));
		Vector4 vector4;
		vector4.X = num13;
		vector4.Y = num14;
		vector4.Z = num15;
		vector4.W = 1.0f;
		return vector4;
	}

	constexpr Vector4 Vector4::Transform(Vector3 const& value, Quaternion const& rotation) {
		const auto num1 = rotation.X + rotation.X;
		const auto num2 = rotation.Y + rotation.Y;
		const auto num3 = rotation.Z + rotation.Z;
		const auto num4 = rotation.W * num1;
		const auto num5 = rotation.W * num2;
		const auto num6 = rotation.W * num3;
		const auto num7 = rotation.X * num1;
		const auto num8 = rotation.X * num2;
		const auto num9 = rotation.X * num3;
		const auto num10 = rotation.Y * num2;
		const auto num11 = rotation.Y * num3;
		const auto num12 = rotation.Z * num3;
		const auto num13 = (value.X * (1.0F - num10 - num12) + value.Y * (num8 - num6) + value.Z * (num9 + num5));
		const auto num14 = (value.X * (num8 + num6) + value.Y * (1.0F - num7 - num12) + value.Z * (num11 - num4));
		const auto num15 = (value.X * (num9 - num5) + value.Y * (num11 + num4) + value.Z * (1.0F - num7 - num10));
		Vector4 vector4;
		vector4.X = num13;
		vector4.Y = num14;
		vector4.Z = num15;
		vector4.W = 1.0f;
		return vector4;
	}

	constexpr Matrix Matrix::CreateFromQuaternion(Quaternion const& quaternion) {
		const auto num1 = quaternion.X * quaternion.X;
		const auto num2 = quaternion.Y * quaternion.Y;
		const auto num3 = quaternion.Z * quaternion.Z;
		const auto num4 = quaternion.X * quaternion.Y;
		const auto num5 = quaternion.Z * quaternion.W;
		const auto num6 = quaternion.Z * quaternion.X;
		const auto num7 = quaternion.Y * quaternion.W;
		const auto num8 = quaternion.Y * quaternion.Z;
		const auto num9 = quaternion.X * quaternion.W;
		Matrix fromQuaternion;
		fromQuaternion.M11 = (1.0f - 2.0f * (num2 + num3));
		fromQuaternion.M12 = (2.0f * (num4 + num5));
		fromQuaternion.M13 = (2.0f * (num6 - num7));
		fromQuaternion.M14 = 0.0f;
		fromQuaternion.M21 = (2.0f * (num4 - num5));
		fromQuaternion.M22 = (1.0f - 2.0f * (num3 + num1));
		fromQuaternion.M23 = (2.0f * (num8 + num9));
		fromQuaternion.M24 = 0.0f;
		fromQuaternion.M31 = (2.0f * (num6 + num7));
		fromQuaternion.M32 = (2.0f * (num8 - num9));
		fromQuaternion.M33 = (1.0f - 2.0f * (num2 + num1));
		fromQuaternion.M34 = 0.0f;
		fromQuaternion.M41 = 0.0f;
		fromQuaternion.M42 = 0.0f;
		fromQuaternion.M43 = 0.0f;
		fromQuaternion.M44 = 1.0f;
		return fromQuaternion;
	}

	constexpr Matrix Matrix::Transform(Matrix const& value, Quaternion const& rotation) {
		const auto num1 = rotation.X + rotation.X;
		const auto num2 = rotation.Y + rotation.Y;
		const auto num3 = rotation.Z + rotation.Z;
		const auto num4 = rotation.W * num1;
		const auto num5 = rotation.W * num2;
		const auto num6 = rotation.W * num3;
		const auto num7 = rotation.X * num1;
		const auto num8 = rotation.X * num2;
		const auto num9 = rotation.X * num3;
		const auto num10 = rotation.Y * num2;
		const auto num11 = rotation.Y * num3;
		const auto num12 = rotation.Z * num3;
		const auto num13 = 1.0f - num10 - num12;
		const auto num14 = num8 - num6;
		const auto num15 = num9 + num5;
		const auto num16 = num8 + num6;
		const auto num17 = 1.0f - num7 - num12;
		const auto num18 = num11 - num4;
		const auto num19 = num9 - num5;
		const auto num20 = num11 + num4;
		const auto num21 = 1.0f - num7 - num10;
		Matrix matrix;
		matrix.M11 = (value.M11 * num13 + value.M12 * num14 + value.M13 * num15);
		matrix.M12 = (value.M11 * num16 + value.M12 * num17 + value.M13 * num18);
		matrix.M13 = (value.M11 * num19 + value.M12 * num20 + value.M13 * num21);
		matrix.M14 = value.M14;
		matrix.M21 = (value.M21 * num13 + value.M22 * num14 + value.M23 * num15);
		matrix.M22 = (value.M21 * num16 + value.M22 * num17 + value.M23 * num18);
		matrix.M23 = (value.M21 * num19 + value.M22 * num20 + value.M23 * num21);
		matrix.M24 = value.M24;
		matrix.M31 = (value.M31 * num13 + value.M32 * num14 + value.M33 * num15);
		matrix.M32 = (value.M31 * num16 + value.M32 * num17 + value.M33 * num18);
		matrix.M33 = (value.M31 * num19 + value.M32 * num20 + value.M33 * num21);
		matrix.M34 = value.M34;
		matrix.M41 = (value.M41 * num13 + value.M42 * num14 + value.M43 * num15);
		matrix.M42 = (value.M41 * num16 + value.M42 * num17 + value.M43 * num18);
		matrix.M43 = (value.M41 * num19 + value.M42 * num20 + value.M43 * num21);
		matrix.M44 = value.M44;
		return matrix;
	}

	constexpr Matrix Matrix::CreateFromYawPitchRoll(float yaw, float pitch, float roll) {
		const auto result1 = Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);
		const auto result2 = Matrix::CreateFromQuaternion(result1);
		return result2;
	}
}

#endif