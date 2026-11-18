#pragma once

#include <span>
#include <cstddef>
#include <cstdint>
#include "MathHelper.hpp"

namespace Xna {
	struct Matrix;
	struct Quaternion;
	
	//////////////////////////////////////////////////////////////////
	// POINT														//
	//////////////////////////////////////////////////////////////////

	//Defines a point in 2D space.
	struct Point final {
		//Specifies the x-coordinate of the point.
		int32_t X{ 0 };
		//Specifies the y - coordinate of the point.
		int32_t Y{ 0 };

		//Initializes a new instance of Point.
		constexpr Point() = default;
		//Initializes a new instance of Point.
		constexpr Point(int32_t X, int32_t Y) : X(X), Y(Y) {}

		//Returns the point (0,0).
		static constexpr Point Zero() { return {}; }

		constexpr bool operator==(const Point& other) const noexcept = default;
	};

	//////////////////////////////////////////////////////////////////
	// RECTANGLE													//
	//////////////////////////////////////////////////////////////////

	//Defines a rectangle. 
	struct Rectangle {
		//Specifies the x-coordinate of the rectangle.
		int32_t X{ 0 };
		//Specifies the y - coordinate of the rectangle.
		int32_t Y{ 0 };
		//Specifies the width of the rectangle.
		int32_t Width{ 0 };
		//Specifies the height of the rectangle.
		int32_t Height{ 0 };

		//Initializes a new instance of Rectangle.
		constexpr Rectangle() = default;
		//Initializes a new instance of Rectangle.
		constexpr Rectangle(int32_t X, int32_t Y, int32_t Width, int32_t Height) :
			X(X), Y(Y), Width(Width), Height(Height) {}

		constexpr bool operator==(const Rectangle& other) const noexcept = default;		

		//Returns the x-coordinate of the left side of the rectangle.
		constexpr int32_t Left() const { return X; }
		//Returns the x-coordinate of the right side of the rectangle.
		constexpr int32_t Right() const { return X + Width; }
		//Returns the y-coordinate of the top of the rectangle.
		constexpr int32_t Top() const { return Y; }
		//Returns the y-coordinate of the bottom of the rectangle.
		constexpr int32_t Bottom() const { return Y + Height; }
		//Gets or sets the upper-left value of the Rectangle.
		constexpr Point Location() const { return { X, Y }; }
		//Gets or sets the upper-left value of the Rectangle.
		constexpr void Location(Point const& p) {
			X = p.X;
			Y = p.Y;
		}
		//Gets or sets the width and height values of the Rectangle.
		constexpr Point Size() const { return { Width, Height }; }
		//Gets or sets the width and height values of the Rectangle.
		constexpr void Size(Point const& p) {
			Width = p.X;
			Height = p.Y;
		}
		//Gets the Point that specifies the center of the rectangle.
		constexpr Point Center() const { return { X + Width / 2, Y + Height / 2 }; }
		//Returns a Rectangle with all of its values set to zero.
		constexpr static Rectangle Empty() { return {}; }
		//Gets a value that indicates whether the Rectangle is empty.
		constexpr bool IsEmpty() const { return Width == 0 && Height == 0 && X == 0 && Y == 0; }

		//Changes the position of the Rectangle.
		constexpr void Offset(Point const& amount) {
			X += amount.X;
			Y += amount.Y;
		}

		//Changes the position of the Rectangle.
		constexpr void Offset(int32_t x, int32_t y) {
			X += x;
			Y += y;
		}

		//Pushes the edges of the Rectangle out by the horizontal and vertical values specified.
		constexpr void Inflate(int32_t horizontalAmount, int32_t verticalAmount) {
			X -= horizontalAmount;
			Y -= verticalAmount;
			Width += horizontalAmount * 2;
			Height += verticalAmount * 2;
		}

		//Determines whether this Rectangle contains a specified point or Rectangle.
		constexpr bool Contains(int32_t x, int32_t y) const {
			return X <= x && x < X + Width && Y <= y && y < Y + Height;
		}

		//Determines whether this Rectangle contains a specified point or Rectangle.
		constexpr bool Contains(Point const& value) const {
			return X <= value.X && value.X < X + Width && Y <= value.Y && value.Y < Y + Height;
		}

		//Determines whether this Rectangle contains a specified point or Rectangle.
		constexpr bool Contains(Rectangle const& value) const {
			return X <= value.X && value.X + value.Width <= X + Width && Y <= value.Y && value.Y + value.Height <= Y + Height;
		}

		//Determines whether a specified Rectangle intersects with this Rectangle.
		constexpr bool Intersects(Rectangle const& value) const {
			return value.X < X + Width && X < value.X + value.Width && value.Y < Y + Height && Y < value.Y + value.Height;
		}

		//Creates a Rectangle defining the area where one rectangle overlaps another rectangle. 
		constexpr static Rectangle Intersect(Rectangle const& value1, Rectangle const& value2) {
			const auto num1 = value1.Right();
			const auto num2 = value2.Right();
			const auto num3 = value1.Bottom();
			const auto num4 = value2.Bottom();
			const auto num5 = value1.X > value2.X ? value1.X : value2.X;
			const auto num6 = value1.Y > value2.Y ? value1.Y : value2.Y;
			const auto num7 = num1 < num2 ? num1 : num2;
			const auto num8 = num3 < num4 ? num3 : num4;

			Rectangle rectangle{};

			if (num7 > num5 && num8 > num6)
			{
				rectangle.X = num5;
				rectangle.Y = num6;
				rectangle.Width = num7 - num5;
				rectangle.Height = num8 - num6;
			}

			return rectangle;
		}

		//Creates a new Rectangle that exactly contains two other rectangles. 
		constexpr static Rectangle Union(Rectangle const& value1, Rectangle const& value2) {
			const auto num1 = value1.Right();
			const auto num2 = value2.Right();
			const auto num3 = value1.Bottom();
			const auto num4 = value2.Bottom();
			const auto num5 = value1.X < value2.X ? value1.X : value2.X;
			const auto num6 = value1.Y < value2.Y ? value1.Y : value2.Y;
			const auto num7 = num1 > num2 ? num1 : num2;
			const auto num8 = num3 > num4 ? num3 : num4;

			Rectangle rectangle;
			rectangle.X = num5;
			rectangle.Y = num6;
			rectangle.Width = num7 - num5;
			rectangle.Height = num8 - num6;

			return rectangle;
		}
	};

	//////////////////////////////////////////////////////////////////
	// VECTOR2														//
	//////////////////////////////////////////////////////////////////

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
		constexpr Vector2(float X, float Y) : X(X), Y(Y) {}
		//Initializes a new instance of Vector2.
		constexpr Vector2(Point const& p) : X(static_cast<float>(p.X)), Y(static_cast<float>(p.Y)) {}

		//Returns a Vector2 with all of its components set to zero.
		static constexpr Vector2 Zero() { return {}; }
		//Returns a Vector2 with both of its components set to one.
		static constexpr Vector2 One() { return { 1 }; }
		//Returns the unit vector for the x-axis.
		static constexpr Vector2 UnitX() { return { 1, 0 }; }
		//Returns the unit vector for the y-axis.
		static constexpr Vector2 UnitY() { return { 0, 1 }; }

		//Calculates the length of the vector.
		constexpr float Length() const { return static_cast<float>(MathHelper::Extensions::Sqrt(LengthSquared())); }
		
		//Calculates the length of the vector squared.
		constexpr float LengthSquared() const { return (X * X + Y * Y); }

		//Divides a vector by a scalar or another vector.
		static constexpr float Distance(Vector2 const& value1, Vector2 const& value2) {
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
		
		static constexpr void Transform(
			std::span<const Vector2> sourceArray,
			const Matrix& matrix,
			std::span<Vector2> destinationArray);

		
		static constexpr void Transform(
			std::span<const Vector2> sourceArray,
			std::size_t sourceIndex,
			Matrix const& matrix,
			std::span<Vector2> destinationArray,
			std::size_t destinationIndex,
			std::size_t length);

		static constexpr void TransformNormal(
			std::span<const Vector2> sourceArray,
			const Matrix& matrix,
			std::span<Vector2> destinationArray);

		static constexpr void TransformNormal(
			std::span<const Vector2> sourceArray,
			std::size_t sourceIndex,
			const Matrix& matrix,
			std::span<Vector2> destinationArray,
			std::size_t destinationIndex,
			std::size_t length);

		static constexpr void Transform(
			std::span<const Vector2> sourceArray,
			const Quaternion& rotation,
			std::span<Vector2> destinationArray);

		static constexpr void Transform(
			std::span<const Vector2> sourceArray,
			std::size_t sourceIndex,
			const Quaternion& rotation,
			std::span<Vector2> destinationArray,
			std::size_t destinationIndex,
			std::size_t length);

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
	};

	//////////////////////////////////////////////////////////////////
	// VECTOR3														//
	//////////////////////////////////////////////////////////////////

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
			: X(value), Y(value), Z(value) {}

		//Initializes a new instance of Vector3.
		constexpr Vector3(float X, float Y, float Z)
			: X(X), Y(Y), Z(Z) {}

		//Initializes a new instance of Vector3.
		constexpr Vector3(Vector2 value, float z)
			: X(value.X), Y(value.Y), Z(z) {}

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

		static constexpr void Transform(
			std::span<const Vector3> sourceArray,
			const Matrix& matrix,
			std::span<Vector3> destinationArray);

		static constexpr void Transform(
			std::span<const Vector3> sourceArray,
			std::size_t sourceIndex,
			const Matrix& matrix,
			std::span<Vector3> destinationArray,
			std::size_t destinationIndex,
			std::size_t length);

		static constexpr void TransformNormal(
			std::span<const Vector3> sourceArray,
			const Matrix& matrix,
			std::span<Vector3> destinationArray);

		static constexpr void TransformNormal(
			std::span<const Vector3> sourceArray,
			std::size_t sourceIndex,
			const Matrix& matrix,
			std::span<Vector3> destinationArray,
			std::size_t destinationIndex,
			std::size_t length);

		static constexpr void Transform(
			std::span<const Vector3> sourceArray,
			const Quaternion& rotation,
			std::span<Vector3> destinationArray);

		static constexpr void Transform(
			std::span<const Vector3> sourceArray,
			std::size_t sourceIndex,
			const Quaternion& rotation,
			std::span<Vector3> destinationArray,
			std::size_t destinationIndex,
			std::size_t length);

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
			Vector3 vec = value1;
			vec.X += value2.X;
			vec.Y += value2.Y;
			return vec;
		}

		friend constexpr Vector3 operator-=(Vector3 const& value1, Vector3 const& value2) {
			Vector3 vec = value1;
			vec.X -= value2.X;
			vec.Y -= value2.Y;
			return vec;
		}

		constexpr bool operator==(const Vector3& other) const noexcept = default;
	};

	//////////////////////////////////////////////////////////////////
	// VECTOR4														//
	//////////////////////////////////////////////////////////////////

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
			X(value), Y(value), Z(value), W(value) {}

		//Initializes a new instance of Vector4.
		constexpr Vector4(float X, float Y, float Z, float W)
			: X(X), Y(Y), Z(Z), W(W) {}

		//Initializes a new instance of Vector4.
		constexpr Vector4(Vector2 value, float Z, float W)
			: X(value.X), Y(value.Y), Z(Z), W(W) {}

		//Initializes a new instance of Vector4.
		constexpr Vector4(Vector3 value, float W)
			: X(value.X), Y(value.Y), Z(value.Z), W(W) {}

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

		static constexpr void Transform(
			std::span<const Vector4> sourceArray,
			const Matrix& matrix,
			std::span<Vector4> destinationArray);

		static constexpr void Transform(
			std::span<const Vector4> sourceArray,
			std::size_t sourceIndex,
			const Matrix& matrix,
			std::span<Vector4> destinationArray,
			std::size_t destinationIndex,
			std::size_t length);

		static constexpr void Transform(
			std::span<const Vector4> sourceArray,
			const Quaternion& rotation,
			std::span<Vector4> destinationArray);

		static constexpr void Transform(
			std::span<const Vector4> sourceArray,
			std::size_t sourceIndex,
			const Quaternion& rotation,
			std::span<Vector4> destinationArray,
			std::size_t destinationIndex,
			std::size_t length);


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
	};

	//////////////////////////////////////////////////////////////////
	// Matrix														//
	//////////////////////////////////////////////////////////////////


}