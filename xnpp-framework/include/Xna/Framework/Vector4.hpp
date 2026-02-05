#ifndef XNA_FRAMEWORK_VECTOR4_HPP
#define XNA_FRAMEWORK_VECTOR4_HPP

#include "Vector3.hpp"
#include "Xna/Internal/Export.hpp"

namespace Xna {
	//Defines a vector with four components.
	struct Vector4 final {
		//Gets or sets the x-component of the vector.
		float X{ 0 };
		//Gets or sets the y-component of the vector.
		float Y{ 0 };
		//Gets or sets the z-component of the vector.
		float Z{ 0 };
		//Gets or sets the w-component of the vector.
		float W{ 0 };

		//Initializes a new instance of Vector4.
		constexpr Vector4() = default;

		//Initializes a new instance of Vector4.
		constexpr Vector4(float value) :
			X(value), Y(value), Z(value), W(value) {
		}

		//Initializes a new instance of Vector4.
		constexpr Vector4(float X, float Y, float Z, float W)
			: X(X), Y(Y), Z(Z), W(W) {
		}

		//Initializes a new instance of Vector4.
		constexpr Vector4(Vector2 value, float Z, float W)
			: X(value.X), Y(value.Y), Z(Z), W(W) {
		}

		//Initializes a new instance of Vector4.
		constexpr Vector4(Vector3 value, float W)
			: X(value.X), Y(value.Y), Z(value.Z), W(W) {
		}		

		//Returns a Vector4 with all of its components set to zero.
		static constexpr Vector4 Zero() { return {}; }
		//Returns a Vector4 with all of its components set to one.
		static constexpr Vector4 One() { return { 1 }; }
		//Returns the Vector4 (1, 0, 0, 0).
		static constexpr Vector4 UnitX() { return { 1,0,0,0 }; }
		//Returns the Vector4 (0, 1, 0, 0).
		static constexpr Vector4 UnitY() { return { 0,1,0,0 }; }
		//Returns the Vector4 (0, 0, 1, 0).
		static constexpr Vector4 UnitZ() { return { 0,0,1,0 }; }
		//Returns the Vector4 (0, 0, 0, 1).
		static constexpr Vector4 UnitW() { return { 0,0,0,1 }; }

		//Calculates the length of the vector.
		constexpr float Length() const {
			return MathHelper::Extensions::Sqrt(LengthSquared());
		}

		// 	Calculates the length of the vector squared.
		constexpr float LengthSquared() const {
			return (X * X + Y * Y + Z * Z + W * W);
		}

		//Calculates the distance between two vectors.
		static constexpr float Distance(Vector4 const& value1, Vector4 const& value2) {
			return MathHelper::Extensions::Sqrt(DistanceSquared(value1, value2));
		}

		//Calculates the distance between two vectors squared.
		static constexpr float DistanceSquared(Vector4 const& value1, Vector4 const& value2) {
			const auto num1 = value1.X - value2.X;
			const auto num2 = value1.Y - value2.Y;
			const auto num3 = value1.Z - value2.Z;
			const auto num4 = value1.W - value2.W;
			return num1 * num1 + num2 * num2 + num3 * num3 + num4 * num4;
		}

		//Calculates the dot product of two vectors.
		static constexpr float Dot(Vector4 const& vector1, Vector4 const& vector2) {
			return vector1.X * vector2.X + vector1.Y * vector2.Y + vector1.Z * vector2.Z + vector1.W * vector2.W;
		}

		//Creates a unit vector from the specified vector.
		constexpr void Normalize() {
			const auto num = 1.0f / Length();
			X *= num;
			Y *= num;
			Z *= num;
			W *= num;
		}

		//Creates a unit vector from the specified vector.
		constexpr static Vector4 Normalize(Vector4 vector) {			
			vector.Normalize();
			return vector;
		}

		//Returns a vector that contains the lowest value from each matching pair of components.
		static constexpr Vector4 Min(Vector4 const& value1, Vector4 const& value2) {
			Vector4 vector4;
			vector4.X = value1.X < value2.X ? value1.X : value2.X;
			vector4.Y = value1.Y < value2.Y ? value1.Y : value2.Y;
			vector4.Z = value1.Z < value2.Z ? value1.Z : value2.Z;
			vector4.W = value1.W < value2.W ? value1.W : value2.W;
			return vector4;
		}

		//Returns a vector that contains the highest value from each matching pair of components.
		static constexpr Vector4 Max(Vector4 const& value1, Vector4 const& value2) {
			Vector4 vector4;
			vector4.X = value1.X > value2.X ? value1.X : value2.X;
			vector4.Y = value1.Y > value2.Y ? value1.Y : value2.Y;
			vector4.Z = value1.Z > value2.Z ? value1.Z : value2.Z;
			vector4.W = value1.W > value2.W ? value1.W : value2.W;
			return vector4;
		}

		//Restricts a value to be within a specified range.
		static Vector4 Clamp(Vector4 const& value1, Vector4 const& min, Vector4 const& max) {
			const auto x = value1.X;
			const auto num1 = x > max.X ? max.X : x;
			const auto num2 = num1 < min.X ? min.X : num1;
			const auto y = value1.Y;
			const auto num3 = y > max.Y ? max.Y : y;
			const auto num4 = num3 < min.Y ? min.Y : num3;
			const auto z = value1.Z;
			const auto num5 = z > max.Z ? max.Z : z;
			const auto num6 = num5 < min.Z ? min.Z : num5;
			const auto w = value1.W;
			const auto num7 = w > max.W ? max.W : w;
			const auto num8 = num7 < min.W ? min.W : num7;
			Vector4 vector4;
			vector4.X = num2;
			vector4.Y = num4;
			vector4.Z = num6;
			vector4.W = num8;
			return vector4;
		}

		//Performs a linear interpolation between two vectors.
		static Vector4 Lerp(Vector4 const& value1, Vector4 const& value2, float amount) {
			Vector4 vector4;
			vector4.X = value1.X + (value2.X - value1.X) * amount;
			vector4.Y = value1.Y + (value2.Y - value1.Y) * amount;
			vector4.Z = value1.Z + (value2.Z - value1.Z) * amount;
			vector4.W = value1.W + (value2.W - value1.W) * amount;
			return vector4;
		}

		//Returns a Vector4 containing the 4D Cartesian coordinates of a point specified in barycentric (areal) coordinates relative to a 4D triangle.
		static Vector4 Barycentric(Vector4 const& value1, Vector4 const& value2, Vector4 const& value3, float amount1, float amount2) {
			Vector4 vector4;
			vector4.X = value1.X + amount1 * (value2.X - value1.X) + amount2 * (value3.X - value1.X);
			vector4.Y = value1.Y + amount1 * (value2.Y - value1.Y) + amount2 * (value3.Y - value1.Y);
			vector4.Z = value1.Z + amount1 * (value2.Z - value1.Z) + amount2 * (value3.Z - value1.Z);
			vector4.W = value1.W + amount1 * (value2.W - value1.W) + amount2 * (value3.W - value1.W);
			return vector4;
		}

		//Interpolates between two values using a cubic equation.
		static Vector4 SmoothStep(Vector4 const& value1, Vector4 const& value2, float amount) {
			amount = amount > 1.0F ? 1.0f : (amount < 0.0F ? 0.0f : amount);
			amount = (amount * amount * (3.0F - 2.0F * amount));
			Vector4 vector4;
			vector4.X = value1.X + (value2.X - value1.X) * amount;
			vector4.Y = value1.Y + (value2.Y - value1.Y) * amount;
			vector4.Z = value1.Z + (value2.Z - value1.Z) * amount;
			vector4.W = value1.W + (value2.W - value1.W) * amount;
			return vector4;
		}

		//Performs a Catmull-Rom interpolation using the specified positions.
		static Vector4 CatmullRom(Vector4 const& value1, Vector4 const& value2, Vector4 const& value3, Vector4 const& value4, float amount) {
			const auto num1 = amount * amount;
			const auto num2 = amount * num1;
			Vector4 vector4;
			vector4.X = 0.5F * (2.0f * value2.X + (-value1.X + value3.X) * amount + (2.0F * value1.X - 5.0f * value2.X + 4.0f * value3.X - value4.X) * num1 + (-value1.X + 3.0f * value2.X - 3.0f * value3.X + value4.X) * num2);
			vector4.Y = 0.5F * (2.0f * value2.Y + (-value1.Y + value3.Y) * amount + (2.0F * value1.Y - 5.0f * value2.Y + 4.0f * value3.Y - value4.Y) * num1 + (-value1.Y + 3.0f * value2.Y - 3.0f * value3.Y + value4.Y) * num2);
			vector4.Z = 0.5F * (2.0f * value2.Z + (-value1.Z + value3.Z) * amount + (2.0F * value1.Z - 5.0f * value2.Z + 4.0f * value3.Z - value4.Z) * num1 + (-value1.Z + 3.0f * value2.Z - 3.0f * value3.Z + value4.Z) * num2);
			vector4.W = 0.5F * (2.0f * value2.W + (-value1.W + value3.W) * amount + (2.0F * value1.W - 5.0f * value2.W + 4.0f * value3.W - value4.W) * num1 + (-value1.W + 3.0f * value2.W - 3.0f * value3.W + value4.W) * num2);
			return vector4;
		}

		//Performs a Hermite spline interpolation.
		static Vector4 Hermite(Vector4 const& value1, Vector4 const& tangent1, Vector4 const& value2, Vector4 const& tangent2, float amount) {
			const auto num1 = amount * amount;
			const auto num2 = amount * num1;
			const auto num3 = (2.0F * num2 - 3.0F * num1 + 1.0F);
			const auto num4 = (-2.0F * num2 + 3.0F * num1);
			const auto num5 = num2 - 2.0f * num1 + amount;
			const auto num6 = num2 - num1;
			Vector4 vector4;
			vector4.X = (value1.X * num3 + value2.X * num4 + tangent1.X * num5 + tangent2.X * num6);
			vector4.Y = (value1.Y * num3 + value2.Y * num4 + tangent1.Y * num5 + tangent2.Y * num6);
			vector4.Z = (value1.Z * num3 + value2.Z * num4 + tangent1.Z * num5 + tangent2.Z * num6);
			vector4.W = (value1.W * num3 + value2.W * num4 + tangent1.W * num5 + tangent2.W * num6);
			return vector4;
		}

		//Transforms a Vector2 by the given Matrix.
		static constexpr Vector4 Transform(Vector2 const& position, Matrix const& matrix);
		//Transforms a Vector3 by the given Matrix.
		static constexpr Vector4 Transform(Vector3 const& position, Matrix const& matrix);
		//Transforms a Vector4 by the specified Matrix.
		static constexpr Vector4 Transform(Vector4 const& vector, Matrix const& matrix);
		//Transforms a Vector2 by a specified Quaternion into a Vector4.
		static constexpr Vector4 Transform(Vector2 const& value, Quaternion const& rotation);
		//Transforms a Vector3 by a specified Quaternion into a Vector4.
		static constexpr Vector4 Transform(Vector3 const& value, Quaternion const& rotation);

		//Transforms an array of Vector4s by a specified Matrix.
		XNPP_API static void Transform(Vector4 const* sourceArray, size_t sourceLength, Matrix const& matrix, Vector4* destinationArray, size_t destinationLength);
		//Transforms an array of Vector4s by a specified Matrix.
		XNPP_API static void Transform(std::vector<Vector4> const& sourceArray, size_t sourceLength, Matrix const& matrix, std::vector<Vector4>& destinationArray);
		//Transforms a specified range in an array of Vector4s by a specified Matrix into a specified range in a destination array.
		XNPP_API static void Transform(Vector4 const* sourceArray, size_t sourceLength, size_t sourceIndex, Matrix const& matrix, Vector4* destinationArray, size_t destinationLength, size_t destinationIndex, size_t length);
		////Transforms a specified range in an array of Vector4s by a specified Matrix into a specified range in a destination array.
		XNPP_API static void Transform(std::vector<Vector4> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector4>& destinationArray, size_t destinationIndex, size_t length);
		//Transforms an array of Vector4s by a specified Quaternion.
		XNPP_API static void Transform(Vector4 const* sourceArray, size_t sourceLength, Quaternion const& rotation, Vector4* destinationArray, size_t destinationLength);
		//Transforms an array of Vector4s by a specified Quaternion.
		XNPP_API static void Transform(std::vector<Vector4> const& sourceArray, Quaternion const& rotation, std::vector<Vector4>& destinationArray);
		//Transforms a specified range in an array of Vector4s by a specified Quaternion into a specified range in a destination array.
		XNPP_API static void Transform(Vector4 const* sourceArray, size_t sourceLength, size_t sourceIndex, Quaternion const& rotation, Vector4* destinationArray, size_t destinationLength, size_t destinationIndex, size_t length);
		//Transforms a specified range in an array of Vector4s by a specified Quaternion into a specified range in a destination array.
		XNPP_API static void Transform(std::vector<Vector4> const& sourceArray, size_t sourceIndex, Quaternion const& rotation, std::vector<Vector4>& destinationArray, size_t destinationIndex, size_t length);

		//Returns a vector pointing in the opposite direction.
		static constexpr Vector4 Negate(Vector4 const& value) {
			Vector4 vector4;
			vector4.X = -value.X;
			vector4.Y = -value.Y;
			vector4.Z = -value.Z;
			vector4.W = -value.W;
			return vector4;
		}

		//Adds two vectors.
		static constexpr Vector4 Add(Vector4 const& value1, Vector4 const& value2) {
			Vector4 vector4;
			vector4.X = value1.X + value2.X;
			vector4.Y = value1.Y + value2.Y;
			vector4.Z = value1.Z + value2.Z;
			vector4.W = value1.W + value2.W;
			return vector4;
		}

		//Subtracts a vector from a vector.
		static constexpr Vector4 Subtract(Vector4 const& value1, Vector4 const& value2) {
			Vector4 vector4;
			vector4.X = value1.X - value2.X;
			vector4.Y = value1.Y - value2.Y;
			vector4.Z = value1.Z - value2.Z;
			vector4.W = value1.W - value2.W;
			return vector4;
		}

		//Multiplies a vector by a scalar or another vector.
		static constexpr Vector4 Multiply(Vector4 const& value1, Vector4 const& value2)
		{
			Vector4 vector4;
			vector4.X = value1.X * value2.X;
			vector4.Y = value1.Y * value2.Y;
			vector4.Z = value1.Z * value2.Z;
			vector4.W = value1.W * value2.W;
			return vector4;
		}

		//Multiplies a vector by a scalar or another vector.
		static constexpr Vector4 Multiply(Vector4 const& value1, float scaleFactor)
		{
			Vector4 vector4;
			vector4.X = value1.X * scaleFactor;
			vector4.Y = value1.Y * scaleFactor;
			vector4.Z = value1.Z * scaleFactor;
			vector4.W = value1.W * scaleFactor;
			return vector4;
		}

		//Divide a vector by a scalar or another vector.
		static constexpr Vector4 Divide(Vector4 const& value1, Vector4 const& value2)
		{
			Vector4 vector4;
			vector4.X = value1.X / value2.X;
			vector4.Y = value1.Y / value2.Y;
			vector4.Z = value1.Z / value2.Z;
			vector4.W = value1.W / value2.W;
			return vector4;
		}

		//Divide a vector by a scalar or another vector.
		static constexpr Vector4 Divide(Vector4 const& value1, float divider)
		{
			float num = 1.0f / divider;
			Vector4 vector4;
			vector4.X = value1.X * num;
			vector4.Y = value1.Y * num;
			vector4.Z = value1.Z * num;
			vector4.W = value1.W * num;
			return vector4;
		}

		constexpr Vector4 operator-() const {
			return Vector4::Negate(*this);
		}

		friend constexpr Vector4 operator+(Vector4 const& value1, Vector4 const& value2) {
			return Vector4::Add(value1, value2);
		}

		friend constexpr Vector4 operator-(Vector4 const& value1, Vector4 const& value2) {
			return Vector4::Subtract(value1, value2);
		}

		friend constexpr Vector4 operator*(Vector4 const& value1, Vector4 const& value2) {
			return Vector4::Multiply(value1, value2);
		}

		friend constexpr Vector4 operator*(Vector4 const& value, float factor) {
			return Vector4::Multiply(value, factor);
		}

		friend constexpr Vector4 operator*(float factor, Vector4 const& value) {
			return Vector4::Multiply(value, factor);
		}

		friend constexpr Vector4 operator/(Vector4 const& value1, Vector4 const& value2) {
			return Vector4::Divide(value1, value2);
		}

		friend constexpr Vector4 operator/(Vector4 const& value, float divider) {
			return Vector4::Divide(value, divider);
		}

		friend constexpr Vector4 operator/(float divider, Vector4 const& value) {
			return Vector4::Divide(value, divider);
		}

		constexpr bool operator==(const Vector4& other) const {
			return X == other.X && Y == other.Y && Z == other.Z && W == other.W;
		}

		constexpr operator std::optional<Vector4>() const {
			return std::make_optional<Vector4>(X, Y, Z, W);
		}
	};

}

#endif