#ifndef XNA_FRAMEWORK_VECTOR2_HPP
#define XNA_FRAMEWORK_VECTOR2_HPP

#include <optional>
#include <vector>
#include "MathHelper.hpp"
#include "Xna/Internal/Export.hpp"

namespace Xna {
	struct Matrix;
	struct Quaternion;

	//Defines a vector with two components. 
	struct Vector2 final {
		//Gets or sets the x-component of the vector.
		float X{ 0 };
		//Gets or sets the y-component of the vector.
		float Y{ 0 };

		//Initializes a new instance of Vector2.
		constexpr Vector2() = default;
		//Initializes a new instance of Vector2.
		constexpr Vector2(float value) : X(value), Y(value) {}
		//Initializes a new instance of Vector2.
		constexpr Vector2(float X, float Y)
			: X(X), Y(Y) {
		}		

		//Returns a Vector2 with all of its components set to zero.
		static constexpr Vector2 Zero() { return {}; }
		//Returns a Vector2 with both of its components set to one.
		static constexpr Vector2 One() { return { 1 }; }
		//Returns the unit vector for the x-axis.
		static constexpr Vector2 UnitX() { return { 1, 0 }; }
		//Returns the unit vector for the y-axis.
		static constexpr Vector2 UnitY() { return { 0, 1 }; }

		//Calculates the length of the vector.
		inline constexpr float Length() const {
			return static_cast<float>(MathHelper::Extensions::Sqrt(LengthSquared()));
		}

		//Calculates the length of the vector squared.
		constexpr float LengthSquared() const {
			return (X * X + Y * Y);
		}

		//Divides a vector by a scalar or another vector.
		static constexpr inline float Distance(Vector2 const& value1, Vector2 const& value2) {
			return MathHelper::Extensions::Sqrt(DistanceSquared(value1, value2));
		}

		//Calculates the distance between two vectors squared.
		static constexpr float DistanceSquared(Vector2 const& value1, Vector2 const& value2) {
			const auto x = value1.X - value2.X;
			const auto y = value1.Y - value2.Y;
			return x * x + y * y;
		}

		//Calculates the dot product of two vectors.
		static constexpr float Dot(Vector2 const& value1, Vector2 const& value2) {
			return value1.X * value2.X + value1.Y * value2.Y;
		}

		//Creates a unit vector from the specified vector.
		constexpr void Normalize() {
			const auto normal = 1.0f / Length();
			X *= normal;
			Y *= normal;
		}

		//Creates a unit vector from the specified vector.
		static constexpr Vector2 Normalize(Vector2 value) {			
			value.Normalize();
			return value;
		}

		//Determines the reflect vector of the given vector and normal.
		static constexpr Vector2 Reflect(Vector2 const& vector, Vector2 const& normal) {
			const auto num = vector.X * normal.X + vector.Y * normal.Y;
			return { vector.X - 2.0f * num * normal.X , vector.Y - 2.0f * num * normal.Y };
		}

		//Determines the reflect vector of the given vector and normal.
		static constexpr Vector2 Min(Vector2 const& value1, Vector2 const& value2) {
			return {
				value1.X < value2.X ? value1.X : value2.X,
				value1.Y < value2.Y ? value1.Y : value2.Y
			};
		}

		//Returns a vector that contains the highest value from each matching pair of components.
		static constexpr Vector2 Max(Vector2 const& value1, Vector2 const& value2) {
			return {
				value1.X > value2.X ? value1.X : value2.X,
				value1.Y > value2.Y ? value1.Y : value2.Y
			};
		}

		//Restricts a value to be within a specified range.
		static constexpr Vector2 Clamp(Vector2 const& value1, Vector2 const& min, Vector2 const& max) {
			const auto _maxx = value1.X > max.X ? max.X : value1.X;
			const auto _x = _maxx < min.X ? min.X : _maxx;
			const auto _maxy = value1.Y > max.Y ? max.Y : value1.Y;
			const auto _y = _maxy < min.Y ? min.Y : _maxy;

			return { _x, _y };
		}

		//Performs a linear interpolation between two vectors.
		static constexpr Vector2 Lerp(Vector2 const& value1, Vector2 const& value2, float amount) {
			return{
				value1.X + (value2.X - value1.X) * amount,
				value1.Y + (value2.Y - value1.Y) * amount
			};
		}

		//Overloaded. Returns a Vector2 containing the 2D Cartesian coordinates of a point specified in 2D barycentric (areal) coordinates.
		static constexpr Vector2 Barycentric(Vector2 const& value1, Vector2 const& value2, Vector2 const& value3, float amount1, float amount2) {
			return {
				value1.X + amount1 * (value2.X - value1.X) + amount2 * (value3.X - value1.X),
				value1.Y + amount1 * (value2.Y - value1.Y) + amount2 * (value3.Y - value1.Y)
			};
		}

		//Interpolates between two values using a cubic equation.
		static constexpr Vector2 SmoothStep(Vector2 const& value1, Vector2 const& value2, float amount) {
			amount = amount > 1.0F ? 1.0f : (amount < 0.0F ? 0.0f : amount);
			amount = amount * amount * (3.0F - 2.0F * amount);

			return {
				value1.X + (value2.X - value1.X) * amount,
				value1.Y + (value2.Y - value1.Y) * amount
			};
		}

		//Performs a Catmull-Rom interpolation using the specified positions.
		static constexpr Vector2 CatmullRom(Vector2 const& value1, Vector2 const& value2, Vector2 const& value3, Vector2 const& value4, float amount) {
			const auto am2 = amount * amount;
			const auto am3 = amount * am2;
			Vector2 vector2;
			vector2.X = 0.5F * (2.0F * value2.X + (-value1.X + value3.X) * amount + (2.0F * value1.X - 5.0F * value2.X + 4.0F * value3.X - value4.X) * am2 + (-value1.X + 3.0F * value2.X - 3.0F * value3.X + value4.X) * am3);
			vector2.Y = 0.5F * (2.0F * value2.Y + (-value1.Y + value3.Y) * amount + (2.0F * value1.Y - 5.0F * value2.Y + 4.0F * value3.Y - value4.Y) * am2 + (-value1.Y + 3.0F * value2.Y - 3.0F * value3.Y + value4.Y) * am3);
			return vector2;
		}

		//Performs a Hermite spline interpolation.
		static constexpr Vector2 Hermite(Vector2 const& value1, Vector2 const& tangent1, Vector2 const& value2, Vector2 const& tangent2, float amount) {
			const auto am2 = amount * amount;
			const auto am3 = amount * am2;
			const auto num3 = (2.0F * am3 - 3.0F * am2 + 1.0F);
			const auto num4 = (-2.0F * am3 + 3.0F * am2);
			const auto num5 = am3 - 2.0F * am2 + amount;
			const auto am4 = am3 - am2;

			return{
				value1.X * num3 + value2.X * num4 + tangent1.X * num5 + tangent2.X * am4,
				value1.Y * num3 + value2.Y * num4 + tangent1.Y * num5 + tangent2.Y * am4
			};
		}

		//Transforms the vector (x, y, 0, 1) by the specified matrix.
		static constexpr Vector2 Transform(Vector2 const& position, Matrix const& matrix);
		//Transforms a 2D vector normal by a matrix.
		static constexpr Vector2 TransformNormal(Vector2 const& normal, Matrix const& matrix);
		//Transforms a single Vector2, or the vector normal (x, y, 0, 0), by a specified Quaternion rotation.
		static constexpr Vector2 Transform(Vector2 const& value, Quaternion const& rotation);
		//Transforms an array of Vector2s by a specified Matrix.
		XNPP_API static void Transform(Vector2 const* sourceArray, size_t sourceArrayLength, Matrix const& matrix, Vector2* destinationArray, size_t destinationArrayLength);
		//Transforms an array of Vector2s by a specified Matrix.
		XNPP_API static void Transform(std::vector<Vector2> sourceArray, Matrix const& matrix, std::vector<Vector2>& destinationArray);
		//Transforms a specified range in an array of Vector2s by a specified Matrix and places the results in a specified range in a destination array.
		XNPP_API static void Transform(Vector2 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Matrix const& matrix,
			Vector2* destinationArray, size_t destinationArrayLength, size_t destinationIndex, size_t length);
		//Transforms a specified range in an array of Vector2s by a specified Matrix and places the results in a specified range in a destination array.
		XNPP_API static void Transform(std::vector<Vector2> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector2>& destinationArray, size_t destinationIndex, size_t length);
		//Transforms an array of Vector2 vector normals by a specified Matrix.
		XNPP_API static void TransformNormal(Vector2 const* sourceArray, size_t sourceArrayLength, Matrix const& matrix, Vector2* destinationArray, size_t destinationArrayLength);
		//Transforms an array of Vector2 vector normals by a specified Matrix.
		XNPP_API static void TransformNormal(std::vector<Vector2> const& sourceArray, Matrix const& matrix, std::vector<Vector2>& destinationArray);
		//Transforms a specified range in an array of Vector2 vector normals by a specified Matrix and places the results in a specified range in a destination array.
		XNPP_API static void TransformNormal(Vector2 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Matrix const& matrix,
			Vector2* destinationArray, size_t destinationArrayLength, size_t destinationIndex, size_t length);
		//Transforms a specified range in an array of Vector2 vector normals by a specified Matrix and places the results in a specified range in a destination array.
		XNPP_API static void TransformNormal(std::vector<Vector2> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector2>& destinationArray, size_t destinationIndex, size_t length);
		////Transforms a specified range in an array of Vector2s by a specified Quaternion and places the results in a specified range in a destination array.
		XNPP_API static void Transform(Vector2 const* sourceArray, size_t sourceArrayLength, Quaternion const& rotation, Vector2* destinationArray, size_t destinationArrayLength);
		//Transforms a specified range in an array of Vector2s by a specified Quaternion and places the results in a specified range in a destination array.
		XNPP_API static void Transform(std::vector<Vector2> const& sourceArray, Quaternion const& rotation, std::vector<Vector2>& destinationArray);
		//Transforms an array of Vector2s by a specified Matrix.
		XNPP_API static void Transform(Vector2 const* sourceArray, size_t sourceArrayLength, size_t sourceIndex, Quaternion const& rotation,
			Vector2* destinationArray, size_t destinationArrayLength, size_t destinationIndex, size_t length);
		//Transforms a specified range in an array of Vector2s by a specified Quaternion and places the results in a specified range in a destination array.
		XNPP_API static void Transform(std::vector<Vector2> const& sourceArray, size_t sourceIndex, Quaternion const& rotation, std::vector<Vector2>& destinationArray, size_t destinationIndex, size_t length);

		//Returns a vector pointing in the opposite direction.
		static constexpr Vector2 Negate(Vector2 const& value) {
			return { -value.X, -value.Y };
		}

		//Adds two vectors.
		static constexpr Vector2 Add(Vector2 const& value1, Vector2 const& value2) {
			return { value1.X + value2.X, value1.Y + value2.Y };
		}

		//Subtracts a vector from a vector.
		static constexpr Vector2 Subtract(Vector2 const& value1, Vector2 const& value2) {
			return { value1.X - value2.X, value1.Y - value2.Y };
		}

		//Multiplies a vector by a scalar or another vector.
		static constexpr Vector2 Multiply(Vector2 const& value1, Vector2 const& value2) {
			return { value1.X * value2.X, value1.Y * value2.Y };
		}

		//Multiplies a vector by a scalar or another vector.
		static constexpr Vector2 Multiply(Vector2 const& value1, float scaleFactor) {
			return { value1.X * scaleFactor, value1.Y * scaleFactor };
		}

		//Divides a vector by a scalar or another vector.
		static constexpr Vector2 Divide(Vector2 const& value1, Vector2 const& value2) {
			return { value1.X / value2.X, value1.Y / value2.Y };
		}

		//Divides a vector by a scalar or another vector.
		static constexpr Vector2 Divide(Vector2 const& value1, float divider) {
			return { value1.X / divider, value1.Y / divider };
		}

		constexpr Vector2 operator-() const {
			return Negate(*this);
		}

		friend constexpr Vector2 operator+(Vector2 const& value1, Vector2 const& value2) {
			return Vector2::Add(value1, value2);
		}

		friend constexpr Vector2 operator-(Vector2 const& value1, Vector2 const& value2) {
			return Vector2::Subtract(value1, value2);
		}

		friend constexpr Vector2 operator*(Vector2 const& value1, Vector2 const& value2) {
			return Vector2::Multiply(value1, value2);
		}

		friend constexpr Vector2 operator/(Vector2 const& value1, Vector2 const& value2) {
			return Vector2::Divide(value1, value2);
		}

		friend constexpr Vector2 operator*(Vector2 const& value1, float factor) {
			return Vector2::Multiply(value1, factor);
		}

		friend constexpr Vector2 operator/(Vector2 const& value1, float divider) {
			return Vector2::Divide(value1, divider);
		}

		friend constexpr Vector2 operator*(float factor, Vector2 const& value1) {
			return Vector2::Multiply(value1, factor);
		}

		friend constexpr Vector2 operator/(float divider, Vector2 const& value1) {
			return Vector2::Divide(value1, divider);
		}

		constexpr bool operator==(const Vector2& other) const {
			return X == other.X && Y == other.Y;
		}

		constexpr operator std::optional<Vector2>() const {
			return std::make_optional<Vector2>(X, Y);
		}
	};
}

#endif