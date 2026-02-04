#ifndef XNA_FRAMEWORK_VECTOR3_HPP
#define XNA_FRAMEWORK_VECTOR3_HPP

#include "Vector2.hpp"
#include <cmath>


namespace Xna {
	//Defines a vector with three components. 
	struct Vector3 final {
		//Gets or sets the x-component of the vector.
		float X{ 0 };
		//Gets or sets the y-component of the vector.
		float Y{ 0 };
		//Gets or sets the z-component of the vector.
		float Z{ 0 };

		//Initializes a new instance of Vector3.
		constexpr Vector3() = default;

		//Initializes a new instance of Vector3.
		constexpr Vector3(float value)
			: X(value), Y(value), Z(value) {
		}

		//Initializes a new instance of Vector3.
		constexpr Vector3(float X, float Y, float Z)
			: X(X), Y(Y), Z(Z) {
		}

		//Initializes a new instance of Vector3.
		constexpr Vector3(Vector2 const& value, float z)
			: X(value.X), Y(value.Y), Z(z) {
		}		

		//Returns a Vector3 with all of its components set to zero.
		static constexpr Vector3 Zero() { return {}; }
		//Returns a Vector2 with both of its components set to one.
		static constexpr Vector3 One() { return { 1 }; }
		//Returns the x unit Vector3(1, 0, 0).
		static constexpr Vector3 UnitX() { return { 1,0,0 }; }
		//Returns the y unit Vector3 (0, 1, 0). 
		static constexpr Vector3 UnitY() { return { 0,1,0 }; }
		//Returns the z unit Vector3 (0, 0, 1). 
		static constexpr Vector3 UnitZ() { return { 0,0,1 }; }
		//Returns a unit vector designating up (0, 1, 0).
		static constexpr Vector3 Up() { return UnitY(); }
		//Returns a unit Vector3 designating down (0, −1, 0).
		static constexpr Vector3 Down() { return -UnitY(); }
		//Returns a unit Vector3 pointing to the right (1, 0, 0).
		static constexpr Vector3 Right() { return UnitX(); }
		//Returns a unit Vector3 designating left (−1, 0, 0).
		static constexpr Vector3 Left() { return -UnitX(); }
		//Returns a unit Vector3 designating forward in a right-handed coordinate system(0, 0, −1).
		static constexpr Vector3 Forward() { return -UnitZ(); }
		//Returns a unit Vector3 designating backward in a right-handed coordinate system (0, 0, 1).
		static constexpr Vector3 Backward() { return UnitZ(); }

		//Calculates the length of the vector.
		constexpr float Length() const { return MathHelper::Extensions::Sqrt(LengthSquared()); }
		//Calculates the length of the vector squared.
		constexpr float LengthSquared() const { return (X * X + Y * Y + Z * Z); }
		//Calculates the distance between two vectors.
		constexpr static float Distance(Vector3 const& value1, Vector3 const& value2) {
			return MathHelper::Extensions::Sqrt(DistanceSquared(value1, value2));
		}
		//Calculates the distance between two vectors squared.
		static constexpr float DistanceSquared(Vector3 const& value1, Vector3 const& value2) {
			const auto x = value1.X - value2.X;
			const auto y = value1.Y - value2.Y;
			const auto z = value1.Z - value2.Z;
			return x * x + y * y + z * z;
		}
		//Calculates the dot product of two vectors.
		static constexpr float Dot(Vector3 const& vector1, Vector3 const& vector2) {
			return vector1.X * vector2.X + vector1.Y * vector2.Y + vector1.Z * vector2.Z;
		}

		//Creates a unit vector from the specified vector.
		constexpr void Normalize() {
			const auto num = 1.0f / Length();
			X *= num;
			Y *= num;
			Z *= num;
		}

		//Creates a unit vector from the specified vector.
		static constexpr Vector3 Normalize(Vector3 value) {			
			value.Normalize();
			return value;
		}

		//Calculates the cross product of two vectors.
		static constexpr Vector3 Cross(Vector3 const& vector1, Vector3 const& vector2) {
			Vector3 vector3;
			vector3.X = vector1.Y * vector2.Z - vector1.Z * vector2.Y;
			vector3.Y = vector1.Z * vector2.X - vector1.X * vector2.Z;
			vector3.Z = vector1.X * vector2.Y - vector1.Y * vector2.X;
			return vector3;
		}

		//Returns the reflection of a vector off a surface that has the specified normal.
		static constexpr Vector3 Reflect(Vector3 const& vector, Vector3 const& normal) {
			const auto num = vector.X * normal.X + vector.Y * normal.Y + vector.Z * normal.Z;
			Vector3 vector3;
			vector3.X = vector.X - 2.0f * num * normal.X;
			vector3.Y = vector.Y - 2.0f * num * normal.Y;
			vector3.Z = vector.Z - 2.0f * num * normal.Z;
			return vector3;
		}

		//Returns a vector that contains the lowest value from each matching pair of components.
		static constexpr Vector3 Min(Vector3 const& value1, Vector3 const& value2) {
			Vector3 vector3;
			vector3.X = value1.X < value2.X ? value1.X : value2.X;
			vector3.Y = value1.Y < value2.Y ? value1.Y : value2.Y;
			vector3.Z = value1.Z < value2.Z ? value1.Z : value2.Z;
			return vector3;
		}

		//Overloaded. Returns a vector that contains the highest value from each matching pair of components.
		static constexpr Vector3 Max(Vector3 const& value1, Vector3 const& value2) {
			Vector3 vector3;
			vector3.X = value1.X > value2.X ? value1.X : value2.X;
			vector3.Y = value1.Y > value2.Y ? value1.Y : value2.Y;
			vector3.Z = value1.Z > value2.Z ? value1.Z : value2.Z;
			return vector3;
		}

		//Restricts a value to be within a specified range.
		static constexpr Vector3 Clamp(Vector3 const& value1, Vector3 const& min, Vector3 const& max) {
			const auto x = value1.X > max.X ? max.X : value1.X;
			const auto _x = x < min.X ? min.X : x;
			const auto y = value1.Y > max.Y ? max.Y : value1.Y;
			const auto _y = y < min.Y ? min.Y : y;
			const auto z = value1.Z > max.Z ? max.Z : value1.Z;
			const auto _z = z < min.Z ? min.Z : z;

			Vector3 vector3;
			vector3.X = _x;
			vector3.Y = _y;
			vector3.Z = _z;
			return vector3;
		}

		//Performs a linear interpolation between two vectors.
		static constexpr Vector3 Lerp(Vector3 const& value1, Vector3 const& value2, float amount) {
			Vector3 vector3;
			vector3.X = value1.X + (value2.X - value1.X) * amount;
			vector3.Y = value1.Y + (value2.Y - value1.Y) * amount;
			vector3.Z = value1.Z + (value2.Z - value1.Z) * amount;
			return vector3;
		}

		//Returns a Vector3 containing the 3D Cartesian coordinates of a point specified in Barycentric coordinates relative to a 3D triangle.
		static constexpr Vector3 Barycentric(Vector3 const& value1, Vector3 const& value2, Vector3 const& value3, float amount1, float amount2) {
			Vector3 vector3;
			vector3.X = value1.X + amount1 * (value2.X - value1.X) + amount2 * (value3.X - value1.X);
			vector3.Y = value1.Y + amount1 * (value2.Y - value1.Y) + amount2 * (value3.Y - value1.Y);
			vector3.Z = value1.Z + amount1 * (value2.Z - value1.Z) + amount2 * (value3.Z - value1.Z);
			return vector3;
		}

		//Interpolates between two values using a cubic equation.
		static constexpr Vector3 SmoothStep(Vector3 const& value1, Vector3 const& value2, float amount) {
			amount = amount > 1.0F ? 1.0f : (amount < 0.0f ? 0.0f : amount);
			amount = (amount * amount * (3.0f - 2.0f * amount));
			Vector3 vector3;
			vector3.X = value1.X + (value2.X - value1.X) * amount;
			vector3.Y = value1.Y + (value2.Y - value1.Y) * amount;
			vector3.Z = value1.Z + (value2.Z - value1.Z) * amount;
			return vector3;
		}

		//Performs a Catmull-Rom interpolation using the specified positions.
		static constexpr Vector3 CatmullRom(Vector3 const& value1, Vector3 const& value2, Vector3 const& value3, Vector3 const& value4, float amount) {
			const auto num1 = amount * amount;
			const auto num2 = amount * num1;
			Vector3 vector3;
			vector3.X = 0.5f * (2.0f * value2.X + (-value1.X + value3.X) * amount + (2.0f * value1.X - 5.0f * value2.X + 4.0f * value3.X - value4.X) * num1 + (-value1.X + 3.0f * value2.X - 3.0f * value3.X + value4.X) * num2);
			vector3.Y = 0.5f * (2.0f * value2.Y + (-value1.Y + value3.Y) * amount + (2.0f * value1.Y - 5.0f * value2.Y + 4.0f * value3.Y - value4.Y) * num1 + (-value1.Y + 3.0f * value2.Y - 3.0f * value3.Y + value4.Y) * num2);
			vector3.Z = 0.5f * (2.0f * value2.Z + (-value1.Z + value3.Z) * amount + (2.0f * value1.Z - 5.0f * value2.Z + 4.0f * value3.Z - value4.Z) * num1 + (-value1.Z + 3.0f * value2.Z - 3.0f * value3.Z + value4.Z) * num2);
			return vector3;
		}

		//Performs a Hermite spline interpolation.
		static Vector3 Hermite(Vector3 const& value1, Vector3 const& tangent1, Vector3 const& value2, Vector3 const& tangent2, float amount) {
			const auto num1 = amount * amount;
			const auto num2 = amount * num1;
			const auto num3 = 2.0F * num2 - 3.0F * num1 + 1.0F;
			const auto num4 = -2.0F * num2 + 3.0F * num1;
			const auto num5 = num2 - 2.0f * num1 + amount;
			const auto num6 = num2 - num1;
			Vector3 vector3;
			vector3.X = value1.X * num3 + value2.X * num4 + tangent1.X * num5 + tangent2.X * num6;
			vector3.Y = value1.Y * num3 + value2.Y * num4 + tangent1.Y * num5 + tangent2.Y * num6;
			vector3.Z = value1.Z * num3 + value2.Z * num4 + tangent1.Z * num5 + tangent2.Z * num6;
			return vector3;
		}

		//Transforms a 3D vector by the given matrix.
		static constexpr Vector3 Transform(Vector3 const& position, Matrix const& matrix);
		//Transforms a 3D vector normal by a matrix.
		static constexpr Vector3 TransformNormal(Vector3 const& normal, Matrix const& matrix);
		//Transforms a Vector3 by a specified Quaternion rotation.
		static constexpr Vector3 Transform(Vector3 const& value, Quaternion const& rotation);
		//Transforms a source array of Vector3s by a specified Matrix and writes the results to an existing destination array.
		static void Transform(Vector3 const* sourceArray, size_t sourceArrayLength, Matrix const& matrix, Vector3* destinationArray, size_t destinationLength);
		//Transforms a source array of Vector3s by a specified Matrix and writes the results to an existing destination array.
		static void Transform(std::vector<Vector3> const& sourceArray, Matrix const& matrix, std::vector<Vector3>& destinationArray);
		//Applies a specified transform Matrix to a specified range of an array of Vector3s and writes the results into a specified range of a destination array.
		static void Transform(Vector3 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Matrix const& matrix, Vector3* destinationArray, size_t destinationLength, size_t destinationIndex, size_t length);
		//Applies a specified transform Matrix to a specified range of an array of Vector3s and writes the results into a specified range of a destination array.
		static void Transform(std::vector<Vector3> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector3>& destinationArray, size_t destinationIndex, size_t length);
		//ransforms an array of 3D vector normals by a specified Matrix.
		static void TransformNormal(Vector3 const* sourceArray, size_t sourceArrayLength, Matrix const& matrix, Vector3* destinationArray, size_t destionationArrayLength);
		//ransforms an array of 3D vector normals by a specified Matrix.
		static void TransformNormal(std::vector<Vector3> const& sourceArray, Matrix const& matrix, std::vector<Vector3>& destinationArray);
		//Transforms a specified range in an array of 3D vector normals by a specified Matrix and writes the results to a specified range in a destination array.
		static void TransformNormal(Vector3 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Matrix const& matrix, Vector3* destinationArray, size_t destinationLength, size_t destinationIndex, size_t length);
		//Transforms a specified range in an array of 3D vector normals by a specified Matrix and writes the results to a specified range in a destination array.
		static void TransformNormal(std::vector<Vector3> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector3>& destinationArray, size_t destinationIndex, size_t length);
		
		//Returns a vector pointing in the opposite direction.
		static constexpr Vector3 Negate(Vector3 const& value)
		{
			Vector3 vector3;
			vector3.X = -value.X;
			vector3.Y = -value.Y;
			vector3.Z = -value.Z;
			return vector3;
		}

		//Adds two vectors.
		static constexpr Vector3 Add(Vector3 const& value1, Vector3 const& value2) {
			Vector3 vector3;
			vector3.X = value1.X + value2.X;
			vector3.Y = value1.Y + value2.Y;
			vector3.Z = value1.Z + value2.Z;
			return vector3;
		}

		//Subtracts a vector from a vector.
		static constexpr Vector3 Subtract(Vector3 const& value1, Vector3 const& value2) {
			Vector3 vector3;
			vector3.X = value1.X - value2.X;
			vector3.Y = value1.Y - value2.Y;
			vector3.Z = value1.Z - value2.Z;
			return vector3;
		}

		//Multiplies a vector by a scalar or another vector.
		static constexpr Vector3 Multiply(Vector3 const& value1, Vector3 const& value2) {
			Vector3 vector3;
			vector3.X = value1.X * value2.X;
			vector3.Y = value1.Y * value2.Y;
			vector3.Z = value1.Z * value2.Z;
			return vector3;
		}

		//Multiplies a vector by a scalar or another vector.
		static constexpr Vector3 Multiply(Vector3 const& value1, float scaleFactor) {
			Vector3 vector3;
			vector3.X = value1.X * scaleFactor;
			vector3.Y = value1.Y * scaleFactor;
			vector3.Z = value1.Z * scaleFactor;
			return vector3;
		}

		//Divide a vector by a scalar or another vector.
		static constexpr Vector3 Divide(Vector3 const& value1, Vector3 const& value2) {
			Vector3 vector3;
			vector3.X = value1.X / value2.X;
			vector3.Y = value1.Y / value2.Y;
			vector3.Z = value1.Z / value2.Z;
			return vector3;
		}

		//Divide a vector by a scalar or another vector.
		static constexpr Vector3 Divide(Vector3 const& value1, float divider) {
			float num = 1.0f / divider;
			Vector3 vector3;
			vector3.X = value1.X * num;
			vector3.Y = value1.Y * num;
			vector3.Z = value1.Z * num;
			return vector3;
		}

		constexpr Vector3 operator-() const {
			return Vector3::Negate(*this);
		}

		friend constexpr Vector3 operator+(Vector3 const& value1, Vector3 const& value2) {
			return Vector3::Add(value1, value2);
		}

		friend constexpr Vector3 operator-(Vector3 const& value1, Vector3 const& value2) {
			return Vector3::Subtract(value1, value2);
		}

		friend constexpr Vector3 operator*(Vector3 const& value1, Vector3 const& value2) {
			return Vector3::Multiply(value1, value2);
		}

		friend constexpr Vector3 operator*(Vector3 const& value, float factor) {
			return Vector3::Multiply(value, factor);
		}

		friend constexpr Vector3 operator*(float factor, Vector3 const& value) {
			return Vector3::Multiply(value, factor);
		}

		friend constexpr Vector3 operator/(Vector3 const& value1, Vector3 const& value2) {
			return Vector3::Divide(value1, value2);
		}

		friend constexpr Vector3 operator/(Vector3 const& value, float divider) {
			return Vector3::Divide(value, divider);
		}

		friend constexpr Vector3 operator/(float divider, Vector3 const& value) {
			return Vector3::Divide(value, divider);
		}

		friend constexpr Vector3 operator+=(Vector3 const& value1, Vector3 const& value2) {
			auto vec = value1;
			vec.X += value2.X;
			vec.Y += value2.Y;
			return vec;
		}

		friend constexpr Vector3 operator-=(Vector3 const& value1, Vector3 const& value2) {
			auto vec = value1;
			vec.X -= value2.X;
			vec.Y -= value2.Y;
			return vec;
		}

		constexpr bool operator==(const Vector3& other) const {
			return X == other.X && Y == other.Y && Z == other.Z;
		}

		constexpr operator std::optional<Vector3>() const {
			return std::make_optional<Vector3>(X, Y, Z);
		}
	};
}

#endif