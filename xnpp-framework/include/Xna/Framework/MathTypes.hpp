#pragma once

#include <span>
#include <cstddef>
#include <cstdint>
#include <cassert>
#include "MathHelper.hpp"

namespace Xna {
	struct Matrix;
	struct Quaternion;
	struct Plane;

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
		constexpr Point() noexcept = default;
		//Initializes a new instance of Point.
		constexpr Point(int32_t X, int32_t Y) noexcept : X(X), Y(Y) {}

		//Returns the point (0,0).
		[[nodiscard]] static constexpr Point Zero() noexcept { return {}; }

		[[nodiscard]] constexpr bool operator==(const Point& other) const noexcept = default;
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
		constexpr Rectangle() noexcept = default;
		//Initializes a new instance of Rectangle.
		constexpr Rectangle(int32_t X, int32_t Y, int32_t Width, int32_t Height) noexcept :
			X(X), Y(Y), Width(Width), Height(Height) {}

		[[nodiscard]] constexpr bool operator==(const Rectangle& other) const noexcept = default;

		//Returns the x-coordinate of the left side of the rectangle.
		[[nodiscard]] constexpr int32_t Left() const noexcept { return X; }
		//Returns the x-coordinate of the right side of the rectangle.
		[[nodiscard]] constexpr int32_t Right() const noexcept { return X + Width; }
		//Returns the y-coordinate of the top of the rectangle.
		[[nodiscard]] constexpr int32_t Top() const noexcept { return Y; }
		//Returns the y-coordinate of the bottom of the rectangle.
		[[nodiscard]] constexpr int32_t Bottom() const noexcept { return Y + Height; }
		//Gets or sets the upper-left value of the Rectangle.
		[[nodiscard]] constexpr Point Location() const noexcept { return { X, Y }; }
		//Gets or sets the upper-left value of the Rectangle.
		constexpr void Location(Point p) noexcept {
			X = p.X;
			Y = p.Y;
		}
		//Gets or sets the width and height values of the Rectangle.
		[[nodiscard]] constexpr Point Size() const noexcept { return { Width, Height }; }

		//Gets or sets the width and height values of the Rectangle.
		constexpr void Size(Point p) noexcept {
			Width = p.X;
			Height = p.Y;
		}

		//Gets the Point that specifies the center of the rectangle.
		[[nodiscard]] constexpr Point Center() const noexcept { return { X + Width / 2, Y + Height / 2 }; }
		//Returns a Rectangle with all of its values set to zero.
		[[nodiscard]] constexpr static Rectangle Empty() noexcept { return {}; }
		//Gets a value that indicates whether the Rectangle is empty.
		[[nodiscard]] constexpr bool IsEmpty() const noexcept { return Width == 0 && Height == 0 && X == 0 && Y == 0; }

		//Changes the position of the Rectangle.
		constexpr void Offset(Point amount) noexcept {
			X += amount.X;
			Y += amount.Y;
		}

		//Changes the position of the Rectangle.
		constexpr void Offset(int32_t x, int32_t y) noexcept {
			X += x;
			Y += y;
		}

		//Pushes the edges of the Rectangle out by the horizontal and vertical values specified.
		constexpr void Inflate(int32_t horizontalAmount, int32_t verticalAmount) noexcept {
			X -= horizontalAmount;
			Y -= verticalAmount;
			Width += horizontalAmount * 2;
			Height += verticalAmount * 2;
		}

		//Determines whether this Rectangle contains a specified point or Rectangle.
		[[nodiscard]] constexpr bool Contains(int32_t x, int32_t y) const noexcept {
			return X <= x && x < X + Width && Y <= y && y < Y + Height;
		}

		//Determines whether this Rectangle contains a specified point or Rectangle.
		[[nodiscard]] constexpr bool Contains(Point value) const noexcept {
			return X <= value.X && value.X < X + Width && Y <= value.Y && value.Y < Y + Height;
		}

		//Determines whether this Rectangle contains a specified point or Rectangle.
		[[nodiscard]] constexpr bool Contains(Rectangle value) const noexcept {
			return X <= value.X && value.X + value.Width <= X + Width && Y <= value.Y && value.Y + value.Height <= Y + Height;
		}

		//Determines whether a specified Rectangle intersects with this Rectangle.
		[[nodiscard]] constexpr bool Intersects(Rectangle value) const noexcept {
			return value.X < X + Width && X < value.X + value.Width && value.Y < Y + Height && Y < value.Y + value.Height;
		}

		//Creates a Rectangle defining the area where one rectangle overlaps another rectangle. 
		[[nodiscard]] constexpr static Rectangle Intersect(Rectangle const& value1, Rectangle const& value2) noexcept {
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
		[[nodiscard]] constexpr static Rectangle Union(Rectangle const& value1, Rectangle const& value2) noexcept {
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
		constexpr Vector2() noexcept = default;
		//Initializes a new instance of Vector2.
		constexpr Vector2(float value) noexcept : X(value), Y(value) {}
		//Initializes a new instance of Vector2.
		constexpr Vector2(float X, float Y) noexcept : X(X), Y(Y) {}
		//Initializes a new instance of Vector2.
		constexpr Vector2(Point const& p) noexcept : X(static_cast<float>(p.X)), Y(static_cast<float>(p.Y)) {}

		//Returns a Vector2 with all of its components set to zero.
		[[nodiscard]] static constexpr Vector2 Zero() noexcept { return {}; }
		//Returns a Vector2 with both of its components set to one.
		[[nodiscard]] static constexpr Vector2 One() noexcept { return { 1 }; }
		//Returns the unit vector for the x-axis.
		[[nodiscard]] static constexpr Vector2 UnitX() noexcept { return { 1, 0 }; }
		//Returns the unit vector for the y-axis.
		[[nodiscard]] static constexpr Vector2 UnitY() noexcept { return { 0, 1 }; }

		//Calculates the length of the vector.
		[[nodiscard]] constexpr float Length() const noexcept { return static_cast<float>(MathHelper::Sqrt(LengthSquared())); }

		//Calculates the length of the vector squared.
		[[nodiscard]] constexpr float LengthSquared() const noexcept { return (X * X + Y * Y); }

		//Divides a vector by a scalar or another vector.
		[[nodiscard]] static constexpr float Distance(Vector2 const& value1, Vector2 const& value2) noexcept {
			return MathHelper::Sqrt(DistanceSquared(value1, value2));
		}

		//Calculates the distance between two vectors squared.
		[[nodiscard]] static constexpr float DistanceSquared(Vector2 const& value1, Vector2 const& value2) noexcept {
			const auto x = value1.X - value2.X;
			const auto y = value1.Y - value2.Y;
			return x * x + y * y;
		}

		//Calculates the dot product of two vectors.
		[[nodiscard]] static constexpr float Dot(Vector2 const& value1, Vector2 const& value2) noexcept {
			return value1.X * value2.X + value1.Y * value2.Y;
		}

		//Creates a unit vector from the specified vector.
		constexpr void Normalize() noexcept {
			const auto normal = 1.0f / Length();
			X *= normal;
			Y *= normal;
		}

		//Creates a unit vector from the specified vector.
		[[nodiscard]] static constexpr Vector2 Normalize(Vector2 value) noexcept {
			value.Normalize();
			return value;
		}

		//Determines the reflect vector of the given vector and normal.
		[[nodiscard]] static constexpr Vector2 Reflect(Vector2 const& vector, Vector2 const& normal) noexcept {
			const auto num = vector.X * normal.X + vector.Y * normal.Y;
			return { vector.X - 2.0f * num * normal.X , vector.Y - 2.0f * num * normal.Y };
		}

		//Determines the reflect vector of the given vector and normal.
		[[nodiscard]] static constexpr Vector2 Min(Vector2 const& value1, Vector2 const& value2) noexcept {
			return {
				value1.X < value2.X ? value1.X : value2.X,
				value1.Y < value2.Y ? value1.Y : value2.Y
			};
		}

		//Returns a vector that contains the highest value from each matching pair of components.
		[[nodiscard]] static constexpr Vector2 Max(Vector2 const& value1, Vector2 const& value2) noexcept {
			return {
				value1.X > value2.X ? value1.X : value2.X,
				value1.Y > value2.Y ? value1.Y : value2.Y
			};
		}

		//Restricts a value to be within a specified range.
		[[nodiscard]] static constexpr Vector2 Clamp(Vector2 const& value1, Vector2 const& min, Vector2 const& max) noexcept {
			const auto _maxx = value1.X > max.X ? max.X : value1.X;
			const auto _x = _maxx < min.X ? min.X : _maxx;
			const auto _maxy = value1.Y > max.Y ? max.Y : value1.Y;
			const auto _y = _maxy < min.Y ? min.Y : _maxy;

			return { _x, _y };
		}

		//Performs a linear interpolation between two vectors.
		[[nodiscard]] static constexpr Vector2 Lerp(Vector2 const& value1, Vector2 const& value2, float amount) noexcept {
			return{
				value1.X + (value2.X - value1.X) * amount,
				value1.Y + (value2.Y - value1.Y) * amount
			};
		}

		//Overloaded. Returns a Vector2 containing the 2D Cartesian coordinates of a point specified in 2D barycentric (areal) coordinates.
		[[nodiscard]] static constexpr Vector2 Barycentric(Vector2 const& value1, Vector2 const& value2, Vector2 const& value3, float amount1, float amount2) noexcept {
			return {
				value1.X + amount1 * (value2.X - value1.X) + amount2 * (value3.X - value1.X),
				value1.Y + amount1 * (value2.Y - value1.Y) + amount2 * (value3.Y - value1.Y)
			};
		}

		//Interpolates between two values using a cubic equation.
		[[nodiscard]] static constexpr Vector2 SmoothStep(Vector2 const& value1, Vector2 const& value2, float amount) noexcept {
			amount = amount > 1.0F ? 1.0f : (amount < 0.0F ? 0.0f : amount);
			amount = amount * amount * (3.0F - 2.0F * amount);

			return {
				value1.X + (value2.X - value1.X) * amount,
				value1.Y + (value2.Y - value1.Y) * amount
			};
		}

		//Performs a Catmull-Rom interpolation using the specified positions.
		[[nodiscard]] static constexpr Vector2 CatmullRom(Vector2 const& value1, Vector2 const& value2, Vector2 const& value3, Vector2 const& value4, float amount) noexcept {
			const auto am2 = amount * amount;
			const auto am3 = amount * am2;
			Vector2 vector2;
			vector2.X = 0.5F * (2.0F * value2.X + (-value1.X + value3.X) * amount + (2.0F * value1.X - 5.0F * value2.X + 4.0F * value3.X - value4.X) * am2 + (-value1.X + 3.0F * value2.X - 3.0F * value3.X + value4.X) * am3);
			vector2.Y = 0.5F * (2.0F * value2.Y + (-value1.Y + value3.Y) * amount + (2.0F * value1.Y - 5.0F * value2.Y + 4.0F * value3.Y - value4.Y) * am2 + (-value1.Y + 3.0F * value2.Y - 3.0F * value3.Y + value4.Y) * am3);
			return vector2;
		}

		//Performs a Hermite spline interpolation.
		[[nodiscard]] static constexpr Vector2 Hermite(Vector2 const& value1, Vector2 const& tangent1, Vector2 const& value2, Vector2 const& tangent2, float amount) noexcept {
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
		[[nodiscard]] static constexpr Vector2 Transform(Vector2 const& position, Matrix const& matrix) noexcept;
		//Transforms a 2D vector normal by a matrix.
		[[nodiscard]] static constexpr Vector2 TransformNormal(Vector2 const& normal, Matrix const& matrix) noexcept;
		//Transforms a single Vector2, or the vector normal (x, y, 0, 0), by a specified Quaternion rotation.
		[[nodiscard]] static constexpr Vector2 Transform(Vector2 const& value, Quaternion const& rotation) noexcept;

		static constexpr void Transform(
			std::span<const Vector2> sourceArray,
			const Matrix& matrix,
			std::span<Vector2> destinationArray) noexcept;

		static constexpr void Transform(
			std::span<const Vector2> sourceArray,
			std::size_t sourceIndex,
			Matrix const& matrix,
			std::span<Vector2> destinationArray,
			std::size_t destinationIndex,
			std::size_t length) noexcept;

		static constexpr void TransformNormal(
			std::span<const Vector2> sourceArray,
			const Matrix& matrix,
			std::span<Vector2> destinationArray) noexcept;

		static constexpr void TransformNormal(
			std::span<const Vector2> sourceArray,
			std::size_t sourceIndex,
			const Matrix& matrix,
			std::span<Vector2> destinationArray,
			std::size_t destinationIndex,
			std::size_t length) noexcept;

		static constexpr void Transform(
			std::span<const Vector2> sourceArray,
			const Quaternion& rotation,
			std::span<Vector2> destinationArray) noexcept;

		static constexpr void Transform(
			std::span<const Vector2> sourceArray,
			std::size_t sourceIndex,
			const Quaternion& rotation,
			std::span<Vector2> destinationArray,
			std::size_t destinationIndex,
			std::size_t length) noexcept;

		//Returns a vector pointing in the opposite direction.
		[[nodiscard]] static constexpr Vector2 Negate(Vector2 const& value) noexcept {
			return { -value.X, -value.Y };
		}

		//Adds two vectors.
		[[nodiscard]] static constexpr Vector2 Add(Vector2 const& value1, Vector2 const& value2) noexcept {
			return { value1.X + value2.X, value1.Y + value2.Y };
		}

		//Subtracts a vector from a vector.
		[[nodiscard]] static constexpr Vector2 Subtract(Vector2 const& value1, Vector2 const& value2) noexcept {
			return { value1.X - value2.X, value1.Y - value2.Y };
		}

		//Multiplies a vector by a scalar or another vector.
		[[nodiscard]] static constexpr Vector2 Multiply(Vector2 const& value1, Vector2 const& value2) noexcept {
			return { value1.X * value2.X, value1.Y * value2.Y };
		}

		//Multiplies a vector by a scalar or another vector.
		[[nodiscard]] static constexpr Vector2 Multiply(Vector2 const& value1, float scaleFactor) noexcept {
			return { value1.X * scaleFactor, value1.Y * scaleFactor };
		}

		//Divides a vector by a scalar or another vector.
		[[nodiscard]] static constexpr Vector2 Divide(Vector2 const& value1, Vector2 const& value2) noexcept {
			return { value1.X / value2.X, value1.Y / value2.Y };
		}

		//Divides a vector by a scalar or another vector.
		[[nodiscard]] static constexpr Vector2 Divide(Vector2 const& value1, float divider) noexcept {
			return { value1.X / divider, value1.Y / divider };
		}

		[[nodiscard]] constexpr Vector2 operator-() const {
			return Negate(*this);
		}

		[[nodiscard]] friend constexpr Vector2 operator+(Vector2 const& value1, Vector2 const& value2) noexcept {
			return Vector2::Add(value1, value2);
		}

		[[nodiscard]] friend constexpr Vector2 operator-(Vector2 const& value1, Vector2 const& value2) noexcept {
			return Vector2::Subtract(value1, value2);
		}

		[[nodiscard]] friend constexpr Vector2 operator*(Vector2 const& value1, Vector2 const& value2) noexcept {
			return Vector2::Multiply(value1, value2);
		}

		[[nodiscard]] friend constexpr Vector2 operator/(Vector2 const& value1, Vector2 const& value2) noexcept {
			return Vector2::Divide(value1, value2);
		}

		[[nodiscard]] friend constexpr Vector2 operator*(Vector2 const& value1, float factor) noexcept {
			return Vector2::Multiply(value1, factor);
		}

		[[nodiscard]] friend constexpr Vector2 operator/(Vector2 const& value1, float divider) noexcept {
			return Vector2::Divide(value1, divider);
		}

		[[nodiscard]] friend constexpr Vector2 operator*(float factor, Vector2 const& value1) noexcept {
			return Vector2::Multiply(value1, factor);
		}

		[[nodiscard]] friend constexpr Vector2 operator/(float divider, Vector2 const& value1) noexcept {
			return Vector2::Divide(value1, divider);
		}

		[[nodiscard]] constexpr bool operator==(const Vector2& other) const noexcept = default;
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
		constexpr Vector3() noexcept = default;
		//Initializes a new instance of Vector3.
		constexpr Vector3(float value) noexcept : X(value), Y(value), Z(value) {}
		//Initializes a new instance of Vector3.
		constexpr Vector3(float X, float Y, float Z) noexcept : X(X), Y(Y), Z(Z) {}
		//Initializes a new instance of Vector3.
		constexpr Vector3(Vector2 value, float z) noexcept : X(value.X), Y(value.Y), Z(z) {}

		//Returns a Vector3 with all of its components set to zero.
		[[nodiscard]] static constexpr Vector3 Zero() noexcept { return {}; }
		//Returns a Vector2 with both of its components set to one.
		[[nodiscard]] static constexpr Vector3 One() noexcept { return { 1 }; }
		//Returns the x unit Vector3(1, 0, 0).
		[[nodiscard]] static constexpr Vector3 UnitX() noexcept { return { 1,0,0 }; }
		//Returns the y unit Vector3 (0, 1, 0). 
		[[nodiscard]] static constexpr Vector3 UnitY() noexcept { return { 0,1,0 }; }
		//Returns the z unit Vector3 (0, 0, 1). 
		[[nodiscard]] static constexpr Vector3 UnitZ() noexcept { return { 0,0,1 }; }
		//Returns a unit vector designating up (0, 1, 0).
		[[nodiscard]] static constexpr Vector3 Up() noexcept { return UnitY(); }
		//Returns a unit Vector3 designating down (0, −1, 0).
		[[nodiscard]] static constexpr Vector3 Down() noexcept { return -UnitY(); }
		//Returns a unit Vector3 pointing to the right (1, 0, 0).
		[[nodiscard]] static constexpr Vector3 Right() noexcept { return UnitX(); }
		//Returns a unit Vector3 designating left (−1, 0, 0).
		[[nodiscard]] static constexpr Vector3 Left() noexcept { return -UnitX(); }
		//Returns a unit Vector3 designating forward in a right-handed coordinate system(0, 0, −1).
		[[nodiscard]] static constexpr Vector3 Forward() noexcept { return -UnitZ(); }
		//Returns a unit Vector3 designating backward in a right-handed coordinate system (0, 0, 1).
		[[nodiscard]] static constexpr Vector3 Backward() noexcept { return UnitZ(); }

		//Calculates the length of the vector.
		[[nodiscard]] constexpr float Length() const noexcept { return MathHelper::Sqrt(LengthSquared()); }
		//Calculates the length of the vector squared.
		[[nodiscard]] constexpr float LengthSquared() const noexcept { return (X * X + Y * Y + Z * Z); }
		//Calculates the distance between two vectors.
		[[nodiscard]] static constexpr float Distance(Vector3 const& value1, Vector3 const& value2) noexcept {
			return MathHelper::Sqrt(DistanceSquared(value1, value2));
		}
		//Calculates the distance between two vectors squared.
		[[nodiscard]] static constexpr float DistanceSquared(Vector3 const& value1, Vector3 const& value2) noexcept {
			const auto x = value1.X - value2.X;
			const auto y = value1.Y - value2.Y;
			const auto z = value1.Z - value2.Z;
			return x * x + y * y + z * z;
		}
		//Calculates the dot product of two vectors.
		[[nodiscard]] static constexpr float Dot(Vector3 const& vector1, Vector3 const& vector2) noexcept {
			return vector1.X * vector2.X + vector1.Y * vector2.Y + vector1.Z * vector2.Z;
		}

		//Creates a unit vector from the specified vector.
		[[nodiscard]] constexpr void Normalize() noexcept {
			const auto num = 1.0f / Length();
			X *= num;
			Y *= num;
			Z *= num;
		}

		//Creates a unit vector from the specified vector.
		[[nodiscard]] static constexpr Vector3 Normalize(Vector3 value) noexcept {
			value.Normalize();
			return value;
		}

		//Calculates the cross product of two vectors.
		[[nodiscard]] static constexpr Vector3 Cross(Vector3 const& vector1, Vector3 const& vector2) noexcept {
			Vector3 vector3;
			vector3.X = vector1.Y * vector2.Z - vector1.Z * vector2.Y;
			vector3.Y = vector1.Z * vector2.X - vector1.X * vector2.Z;
			vector3.Z = vector1.X * vector2.Y - vector1.Y * vector2.X;
			return vector3;
		}

		//Returns the reflection of a vector off a surface that has the specified normal.
		[[nodiscard]] static constexpr Vector3 Reflect(Vector3 const& vector, Vector3 const& normal) noexcept {
			const auto num = vector.X * normal.X + vector.Y * normal.Y + vector.Z * normal.Z;
			Vector3 vector3;
			vector3.X = vector.X - 2.0f * num * normal.X;
			vector3.Y = vector.Y - 2.0f * num * normal.Y;
			vector3.Z = vector.Z - 2.0f * num * normal.Z;
			return vector3;
		}

		//Returns a vector that contains the lowest value from each matching pair of components.
		[[nodiscard]] static constexpr Vector3 Min(Vector3 const& value1, Vector3 const& value2) noexcept {
			Vector3 vector3;
			vector3.X = value1.X < value2.X ? value1.X : value2.X;
			vector3.Y = value1.Y < value2.Y ? value1.Y : value2.Y;
			vector3.Z = value1.Z < value2.Z ? value1.Z : value2.Z;
			return vector3;
		}

		//Overloaded. Returns a vector that contains the highest value from each matching pair of components.
		[[nodiscard]] static constexpr Vector3 Max(Vector3 const& value1, Vector3 const& value2) noexcept {
			Vector3 vector3;
			vector3.X = value1.X > value2.X ? value1.X : value2.X;
			vector3.Y = value1.Y > value2.Y ? value1.Y : value2.Y;
			vector3.Z = value1.Z > value2.Z ? value1.Z : value2.Z;
			return vector3;
		}

		//Restricts a value to be within a specified range.
		[[nodiscard]] static constexpr Vector3 Clamp(Vector3 const& value1, Vector3 const& min, Vector3 const& max) noexcept {
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
		[[nodiscard]] static constexpr Vector3 Lerp(Vector3 const& value1, Vector3 const& value2, float amount) noexcept {
			Vector3 vector3;
			vector3.X = value1.X + (value2.X - value1.X) * amount;
			vector3.Y = value1.Y + (value2.Y - value1.Y) * amount;
			vector3.Z = value1.Z + (value2.Z - value1.Z) * amount;
			return vector3;
		}

		//Returns a Vector3 containing the 3D Cartesian coordinates of a point specified in Barycentric coordinates relative to a 3D triangle.
		[[nodiscard]] static constexpr Vector3 Barycentric(Vector3 const& value1, Vector3 const& value2, Vector3 const& value3, float amount1, float amount2) noexcept {
			Vector3 vector3;
			vector3.X = value1.X + amount1 * (value2.X - value1.X) + amount2 * (value3.X - value1.X);
			vector3.Y = value1.Y + amount1 * (value2.Y - value1.Y) + amount2 * (value3.Y - value1.Y);
			vector3.Z = value1.Z + amount1 * (value2.Z - value1.Z) + amount2 * (value3.Z - value1.Z);
			return vector3;
		}

		//Interpolates between two values using a cubic equation.
		[[nodiscard]] static constexpr Vector3 SmoothStep(Vector3 const& value1, Vector3 const& value2, float amount) noexcept {
			amount = amount > 1.0F ? 1.0f : (amount < 0.0f ? 0.0f : amount);
			amount = (amount * amount * (3.0f - 2.0f * amount));
			Vector3 vector3;
			vector3.X = value1.X + (value2.X - value1.X) * amount;
			vector3.Y = value1.Y + (value2.Y - value1.Y) * amount;
			vector3.Z = value1.Z + (value2.Z - value1.Z) * amount;
			return vector3;
		}

		//Performs a Catmull-Rom interpolation using the specified positions.
		[[nodiscard]] static constexpr Vector3 CatmullRom(Vector3 const& value1, Vector3 const& value2, Vector3 const& value3, Vector3 const& value4, float amount) noexcept {
			const auto num1 = amount * amount;
			const auto num2 = amount * num1;
			Vector3 vector3;
			vector3.X = 0.5f * (2.0f * value2.X + (-value1.X + value3.X) * amount + (2.0f * value1.X - 5.0f * value2.X + 4.0f * value3.X - value4.X) * num1 + (-value1.X + 3.0f * value2.X - 3.0f * value3.X + value4.X) * num2);
			vector3.Y = 0.5f * (2.0f * value2.Y + (-value1.Y + value3.Y) * amount + (2.0f * value1.Y - 5.0f * value2.Y + 4.0f * value3.Y - value4.Y) * num1 + (-value1.Y + 3.0f * value2.Y - 3.0f * value3.Y + value4.Y) * num2);
			vector3.Z = 0.5f * (2.0f * value2.Z + (-value1.Z + value3.Z) * amount + (2.0f * value1.Z - 5.0f * value2.Z + 4.0f * value3.Z - value4.Z) * num1 + (-value1.Z + 3.0f * value2.Z - 3.0f * value3.Z + value4.Z) * num2);
			return vector3;
		}

		//Performs a Hermite spline interpolation.
		[[nodiscard]] static constexpr Vector3 Hermite(Vector3 const& value1, Vector3 const& tangent1, Vector3 const& value2, Vector3 const& tangent2, float amount) noexcept {
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
		[[nodiscard]] static constexpr Vector3 Transform(Vector3 const& position, Matrix const& matrix) noexcept;
		//Transforms a 3D vector normal by a matrix.
		[[nodiscard]] static constexpr Vector3 TransformNormal(Vector3 const& normal, Matrix const& matrix) noexcept;
		//Transforms a Vector3 by a specified Quaternion rotation.
		[[nodiscard]] static constexpr Vector3 Transform(Vector3 const& value, Quaternion const& rotation) noexcept;

		static constexpr void Transform(
			std::span<const Vector3> sourceArray,
			const Matrix& matrix,
			std::span<Vector3> destinationArray) noexcept;

		static constexpr void Transform(
			std::span<const Vector3> sourceArray,
			std::size_t sourceIndex,
			const Matrix& matrix,
			std::span<Vector3> destinationArray,
			std::size_t destinationIndex,
			std::size_t length) noexcept;

		static constexpr void TransformNormal(
			std::span<const Vector3> sourceArray,
			const Matrix& matrix,
			std::span<Vector3> destinationArray) noexcept;

		static constexpr void TransformNormal(
			std::span<const Vector3> sourceArray,
			std::size_t sourceIndex,
			const Matrix& matrix,
			std::span<Vector3> destinationArray,
			std::size_t destinationIndex,
			std::size_t length) noexcept;

		static constexpr void Transform(
			std::span<const Vector3> sourceArray,
			const Quaternion& rotation,
			std::span<Vector3> destinationArray) noexcept;

		static constexpr void Transform(
			std::span<const Vector3> sourceArray,
			std::size_t sourceIndex,
			const Quaternion& rotation,
			std::span<Vector3> destinationArray,
			std::size_t destinationIndex,
			std::size_t length) noexcept;

		//Returns a vector pointing in the opposite direction.
		[[nodiscard]] static constexpr Vector3 Negate(Vector3 const& value) noexcept
		{
			Vector3 vector3;
			vector3.X = -value.X;
			vector3.Y = -value.Y;
			vector3.Z = -value.Z;
			return vector3;
		}

		//Adds two vectors.
		[[nodiscard]] static constexpr Vector3 Add(Vector3 const& value1, Vector3 const& value2) noexcept {
			Vector3 vector3;
			vector3.X = value1.X + value2.X;
			vector3.Y = value1.Y + value2.Y;
			vector3.Z = value1.Z + value2.Z;
			return vector3;
		}

		//Subtracts a vector from a vector.
		[[nodiscard]] static constexpr Vector3 Subtract(Vector3 const& value1, Vector3 const& value2) noexcept {
			Vector3 vector3;
			vector3.X = value1.X - value2.X;
			vector3.Y = value1.Y - value2.Y;
			vector3.Z = value1.Z - value2.Z;
			return vector3;
		}

		//Multiplies a vector by a scalar or another vector.
		[[nodiscard]] static constexpr Vector3 Multiply(Vector3 const& value1, Vector3 const& value2) noexcept {
			Vector3 vector3;
			vector3.X = value1.X * value2.X;
			vector3.Y = value1.Y * value2.Y;
			vector3.Z = value1.Z * value2.Z;
			return vector3;
		}

		//Multiplies a vector by a scalar or another vector.
		[[nodiscard]] static constexpr Vector3 Multiply(Vector3 const& value1, float scaleFactor) noexcept {
			Vector3 vector3;
			vector3.X = value1.X * scaleFactor;
			vector3.Y = value1.Y * scaleFactor;
			vector3.Z = value1.Z * scaleFactor;
			return vector3;
		}

		//Divide a vector by a scalar or another vector.
		[[nodiscard]] static constexpr Vector3 Divide(Vector3 const& value1, Vector3 const& value2) noexcept {
			Vector3 vector3;
			vector3.X = value1.X / value2.X;
			vector3.Y = value1.Y / value2.Y;
			vector3.Z = value1.Z / value2.Z;
			return vector3;
		}

		//Divide a vector by a scalar or another vector.
		[[nodiscard]] static constexpr Vector3 Divide(Vector3 const& value1, float divider) noexcept {
			float num = 1.0f / divider;
			Vector3 vector3;
			vector3.X = value1.X * num;
			vector3.Y = value1.Y * num;
			vector3.Z = value1.Z * num;
			return vector3;
		}

		[[nodiscard]] constexpr Vector3 operator-() const {
			return Vector3::Negate(*this);
		}

		[[nodiscard]] friend constexpr Vector3 operator+(Vector3 const& value1, Vector3 const& value2) noexcept {
			return Vector3::Add(value1, value2);
		}

		[[nodiscard]] friend constexpr Vector3 operator-(Vector3 const& value1, Vector3 const& value2) noexcept {
			return Vector3::Subtract(value1, value2);
		}

		[[nodiscard]] friend constexpr Vector3 operator*(Vector3 const& value1, Vector3 const& value2) noexcept {
			return Vector3::Multiply(value1, value2);
		}

		[[nodiscard]] friend constexpr Vector3 operator*(Vector3 const& value, float factor) noexcept {
			return Vector3::Multiply(value, factor);
		}

		[[nodiscard]] friend constexpr Vector3 operator*(float factor, Vector3 const& value) noexcept {
			return Vector3::Multiply(value, factor);
		}

		[[nodiscard]] friend constexpr Vector3 operator/(Vector3 const& value1, Vector3 const& value2) noexcept {
			return Vector3::Divide(value1, value2);
		}

		[[nodiscard]] friend constexpr Vector3 operator/(Vector3 const& value, float divider) noexcept {
			return Vector3::Divide(value, divider);
		}

		[[nodiscard]] friend constexpr Vector3 operator/(float divider, Vector3 const& value) noexcept {
			return Vector3::Divide(value, divider);
		}

		[[nodiscard]] friend constexpr Vector3 operator+=(Vector3 const& value1, Vector3 const& value2) noexcept {
			Vector3 vec = value1;
			vec.X += value2.X;
			vec.Y += value2.Y;
			return vec;
		}

		[[nodiscard]] friend constexpr Vector3 operator-=(Vector3 const& value1, Vector3 const& value2) noexcept {
			Vector3 vec = value1;
			vec.X -= value2.X;
			vec.Y -= value2.Y;
			return vec;
		}

		[[nodiscard]] constexpr bool operator==(const Vector3& other) const noexcept = default;
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
		constexpr Vector4() noexcept = default;
		//Initializes a new instance of Vector4.
		constexpr Vector4(float value) noexcept : X(value), Y(value), Z(value), W(value) {}
		//Initializes a new instance of Vector4.
		constexpr Vector4(float X, float Y, float Z, float W) noexcept : X(X), Y(Y), Z(Z), W(W) {}
		//Initializes a new instance of Vector4.
		constexpr Vector4(Vector2 value, float Z, float W) noexcept : X(value.X), Y(value.Y), Z(Z), W(W) {}
		//Initializes a new instance of Vector4.
		constexpr Vector4(Vector3 value, float W) noexcept : X(value.X), Y(value.Y), Z(value.Z), W(W) {}

		//Returns a Vector4 with all of its components set to zero.
		[[nodiscard]] static constexpr Vector4 Zero() noexcept { return {}; }
		//Returns a Vector4 with all of its components set to one.
		[[nodiscard]] static constexpr Vector4 One() noexcept { return { 1 }; }
		//Returns the Vector4 (1, 0, 0, 0).
		[[nodiscard]] static constexpr Vector4 UnitX() noexcept { return { 1,0,0,0 }; }
		//Returns the Vector4 (0, 1, 0, 0).
		[[nodiscard]] static constexpr Vector4 UnitY() noexcept { return { 0,1,0,0 }; }
		//Returns the Vector4 (0, 0, 1, 0).
		[[nodiscard]] static constexpr Vector4 UnitZ() noexcept { return { 0,0,1,0 }; }
		//Returns the Vector4 (0, 0, 0, 1).
		[[nodiscard]] static constexpr Vector4 UnitW() noexcept { return { 0,0,0,1 }; }

		//Calculates the length of the vector.
		[[nodiscard]] constexpr float Length() const noexcept {
			return MathHelper::Sqrt(LengthSquared());
		}

		// 	Calculates the length of the vector squared.
		[[nodiscard]] constexpr float LengthSquared() const noexcept {
			return (X * X + Y * Y + Z * Z + W * W);
		}

		//Calculates the distance between two vectors.
		[[nodiscard]] static constexpr float Distance(Vector4 const& value1, Vector4 const& value2) noexcept {
			return MathHelper::Sqrt(DistanceSquared(value1, value2));
		}

		//Calculates the distance between two vectors squared.
		[[nodiscard]] static constexpr float DistanceSquared(Vector4 const& value1, Vector4 const& value2) noexcept {
			const auto num1 = value1.X - value2.X;
			const auto num2 = value1.Y - value2.Y;
			const auto num3 = value1.Z - value2.Z;
			const auto num4 = value1.W - value2.W;
			return num1 * num1 + num2 * num2 + num3 * num3 + num4 * num4;
		}

		//Calculates the dot product of two vectors.
		[[nodiscard]] static constexpr float Dot(Vector4 const& vector1, Vector4 const& vector2) noexcept {
			return vector1.X * vector2.X + vector1.Y * vector2.Y + vector1.Z * vector2.Z + vector1.W * vector2.W;
		}

		//Creates a unit vector from the specified vector.
		constexpr void Normalize() noexcept {
			const auto num = 1.0f / Length();
			X *= num;
			Y *= num;
			Z *= num;
			W *= num;
		}

		//Creates a unit vector from the specified vector.
		[[nodiscard]] constexpr static Vector4 Normalize(Vector4 vector) noexcept {
			vector.Normalize();
			return vector;
		}

		//Returns a vector that contains the lowest value from each matching pair of components.
		[[nodiscard]] static constexpr Vector4 Min(Vector4 const& value1, Vector4 const& value2) noexcept {
			Vector4 vector4;
			vector4.X = value1.X < value2.X ? value1.X : value2.X;
			vector4.Y = value1.Y < value2.Y ? value1.Y : value2.Y;
			vector4.Z = value1.Z < value2.Z ? value1.Z : value2.Z;
			vector4.W = value1.W < value2.W ? value1.W : value2.W;
			return vector4;
		}

		//Returns a vector that contains the highest value from each matching pair of components.
		[[nodiscard]] static constexpr Vector4 Max(Vector4 const& value1, Vector4 const& value2) noexcept {
			Vector4 vector4;
			vector4.X = value1.X > value2.X ? value1.X : value2.X;
			vector4.Y = value1.Y > value2.Y ? value1.Y : value2.Y;
			vector4.Z = value1.Z > value2.Z ? value1.Z : value2.Z;
			vector4.W = value1.W > value2.W ? value1.W : value2.W;
			return vector4;
		}

		//Restricts a value to be within a specified range.
		[[nodiscard]] static Vector4 Clamp(Vector4 const& value1, Vector4 const& min, Vector4 const& max) noexcept {
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
		[[nodiscard]] static Vector4 Lerp(Vector4 const& value1, Vector4 const& value2, float amount) noexcept {
			Vector4 vector4;
			vector4.X = value1.X + (value2.X - value1.X) * amount;
			vector4.Y = value1.Y + (value2.Y - value1.Y) * amount;
			vector4.Z = value1.Z + (value2.Z - value1.Z) * amount;
			vector4.W = value1.W + (value2.W - value1.W) * amount;
			return vector4;
		}

		//Returns a Vector4 containing the 4D Cartesian coordinates of a point specified in barycentric (areal) coordinates relative to a 4D triangle.
		[[nodiscard]] static Vector4 Barycentric(Vector4 const& value1, Vector4 const& value2, Vector4 const& value3, float amount1, float amount2) noexcept {
			Vector4 vector4;
			vector4.X = value1.X + amount1 * (value2.X - value1.X) + amount2 * (value3.X - value1.X);
			vector4.Y = value1.Y + amount1 * (value2.Y - value1.Y) + amount2 * (value3.Y - value1.Y);
			vector4.Z = value1.Z + amount1 * (value2.Z - value1.Z) + amount2 * (value3.Z - value1.Z);
			vector4.W = value1.W + amount1 * (value2.W - value1.W) + amount2 * (value3.W - value1.W);
			return vector4;
		}

		//Interpolates between two values using a cubic equation.
		[[nodiscard]] static Vector4 SmoothStep(Vector4 const& value1, Vector4 const& value2, float amount) noexcept {
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
		[[nodiscard]] static Vector4 CatmullRom(Vector4 const& value1, Vector4 const& value2, Vector4 const& value3, Vector4 const& value4, float amount) noexcept {
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
		[[nodiscard]] static Vector4 Hermite(Vector4 const& value1, Vector4 const& tangent1, Vector4 const& value2, Vector4 const& tangent2, float amount) noexcept {
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
		[[nodiscard]] static constexpr Vector4 Transform(Vector2 const& position, Matrix const& matrix) noexcept;
		//Transforms a Vector3 by the given Matrix.
		[[nodiscard]] static constexpr Vector4 Transform(Vector3 const& position, Matrix const& matrix) noexcept;
		//Transforms a Vector4 by the specified Matrix.
		[[nodiscard]] static constexpr Vector4 Transform(Vector4 const& vector, Matrix const& matrix) noexcept;
		//Transforms a Vector2 by a specified Quaternion into a Vector4.
		[[nodiscard]] static constexpr Vector4 Transform(Vector2 const& value, Quaternion const& rotation) noexcept;
		//Transforms a Vector3 by a specified Quaternion into a Vector4.
		[[nodiscard]] static constexpr Vector4 Transform(Vector3 const& value, Quaternion const& rotation) noexcept;

		static constexpr void Transform(
			std::span<const Vector4> sourceArray,
			const Matrix& matrix,
			std::span<Vector4> destinationArray) noexcept;

		static constexpr void Transform(
			std::span<const Vector4> sourceArray,
			std::size_t sourceIndex,
			const Matrix& matrix,
			std::span<Vector4> destinationArray,
			std::size_t destinationIndex,
			std::size_t length) noexcept;

		static constexpr void Transform(
			std::span<const Vector4> sourceArray,
			const Quaternion& rotation,
			std::span<Vector4> destinationArray) noexcept;

		static constexpr void Transform(
			std::span<const Vector4> sourceArray,
			std::size_t sourceIndex,
			const Quaternion& rotation,
			std::span<Vector4> destinationArray,
			std::size_t destinationIndex,
			std::size_t length) noexcept;


		//Returns a vector pointing in the opposite direction.
		[[nodiscard]] static constexpr Vector4 Negate(Vector4 const& value) noexcept {
			Vector4 vector4;
			vector4.X = -value.X;
			vector4.Y = -value.Y;
			vector4.Z = -value.Z;
			vector4.W = -value.W;
			return vector4;
		}

		//Adds two vectors.
		[[nodiscard]] static constexpr Vector4 Add(Vector4 const& value1, Vector4 const& value2) noexcept {
			Vector4 vector4;
			vector4.X = value1.X + value2.X;
			vector4.Y = value1.Y + value2.Y;
			vector4.Z = value1.Z + value2.Z;
			vector4.W = value1.W + value2.W;
			return vector4;
		}

		//Subtracts a vector from a vector.
		[[nodiscard]] static constexpr Vector4 Subtract(Vector4 const& value1, Vector4 const& value2) noexcept {
			Vector4 vector4;
			vector4.X = value1.X - value2.X;
			vector4.Y = value1.Y - value2.Y;
			vector4.Z = value1.Z - value2.Z;
			vector4.W = value1.W - value2.W;
			return vector4;
		}

		//Multiplies a vector by a scalar or another vector.
		[[nodiscard]] static constexpr Vector4 Multiply(Vector4 const& value1, Vector4 const& value2) noexcept
		{
			Vector4 vector4;
			vector4.X = value1.X * value2.X;
			vector4.Y = value1.Y * value2.Y;
			vector4.Z = value1.Z * value2.Z;
			vector4.W = value1.W * value2.W;
			return vector4;
		}

		//Multiplies a vector by a scalar or another vector.
		[[nodiscard]] static constexpr Vector4 Multiply(Vector4 const& value1, float scaleFactor) noexcept
		{
			Vector4 vector4;
			vector4.X = value1.X * scaleFactor;
			vector4.Y = value1.Y * scaleFactor;
			vector4.Z = value1.Z * scaleFactor;
			vector4.W = value1.W * scaleFactor;
			return vector4;
		}

		//Divide a vector by a scalar or another vector.
		[[nodiscard]] static constexpr Vector4 Divide(Vector4 const& value1, Vector4 const& value2) noexcept
		{
			Vector4 vector4;
			vector4.X = value1.X / value2.X;
			vector4.Y = value1.Y / value2.Y;
			vector4.Z = value1.Z / value2.Z;
			vector4.W = value1.W / value2.W;
			return vector4;
		}

		//Divide a vector by a scalar or another vector.
		[[nodiscard]] static constexpr Vector4 Divide(Vector4 const& value1, float divider) noexcept
		{
			float num = 1.0f / divider;
			Vector4 vector4;
			vector4.X = value1.X * num;
			vector4.Y = value1.Y * num;
			vector4.Z = value1.Z * num;
			vector4.W = value1.W * num;
			return vector4;
		}

		[[nodiscard]] constexpr Vector4 operator-() const noexcept {
			return Vector4::Negate(*this);
		}

		[[nodiscard]] friend constexpr Vector4 operator+(Vector4 const& value1, Vector4 const& value2) noexcept {
			return Vector4::Add(value1, value2);
		}

		[[nodiscard]] friend constexpr Vector4 operator-(Vector4 const& value1, Vector4 const& value2) noexcept {
			return Vector4::Subtract(value1, value2);
		}

		[[nodiscard]] friend constexpr Vector4 operator*(Vector4 const& value1, Vector4 const& value2) noexcept {
			return Vector4::Multiply(value1, value2);
		}

		[[nodiscard]] friend constexpr Vector4 operator*(Vector4 const& value, float factor) noexcept {
			return Vector4::Multiply(value, factor);
		}

		[[nodiscard]] friend constexpr Vector4 operator*(float factor, Vector4 const& value) noexcept {
			return Vector4::Multiply(value, factor);
		}

		[[nodiscard]] friend constexpr Vector4 operator/(Vector4 const& value1, Vector4 const& value2) noexcept {
			return Vector4::Divide(value1, value2);
		}

		[[nodiscard]] friend constexpr Vector4 operator/(Vector4 const& value, float divider) noexcept {
			return Vector4::Divide(value, divider);
		}

		[[nodiscard]] friend constexpr Vector4 operator/(float divider, Vector4 const& value) noexcept {
			return Vector4::Divide(value, divider);
		}

		[[nodiscard]] constexpr bool operator==(const Vector4& other) const noexcept = default;
	};

	//////////////////////////////////////////////////////////////////
	// MATRIX														//
	//////////////////////////////////////////////////////////////////

	//Defines a matrix.
	struct Matrix final {
		//Value at row 1 column 1 of the matrix.
		float M11{ 0 };
		//Value at row 1 column 2 of the matrix.
		float M12{ 0 };
		//Value at row 1 column 3 of the matrix.
		float M13{ 0 };
		//Value at row 1 column 4 of the matrix.
		float M14{ 0 };
		//Value at row 2 column 1 of the matrix.
		float M21{ 0 };
		//Value at row 2 column 2 of the matrix.
		float M22{ 0 };
		//Value at row 2 column 3 of the matrix.
		float M23{ 0 };
		//Value at row 2 column 4 of the matrix.
		float M24{ 0 };
		//Value at row 3 column 1 of the matrix.
		float M31{ 0 };
		//Value at row 3 column 2 of the matrix.
		float M32{ 0 };
		//Value at row 3 column 3 of the matrix.
		float M33{ 0 };
		//Value at row 3 column 4 of the matrix.
		float M34{ 0 };
		//Value at row 4 column 1 of the matrix.
		float M41{ 0 };
		//Value at row 4 column 2 of the matrix.
		float M42{ 0 };
		//Value at row 4 column 3 of the matrix.
		float M43{ 0 };
		//Value at row 4 column 4 of the matrix.
		float M44{ 0 };

		//Initializes a new instance of Matrix.
		constexpr Matrix() noexcept = default;

		//Initializes a new instance of Matrix.
		constexpr Matrix(
			float M11, float M12, float M13, float M14,
			float M21, float M22, float M23, float M24,
			float M31, float M32, float M33, float M34,
			float M41, float M42, float M43, float M44) noexcept :
			M11(M11), M12(M12), M13(M13), M14(M14),
			M21(M21), M22(M22), M23(M23), M24(M24),
			M31(M31), M32(M32), M33(M33), M34(M34),
			M41(M41), M42(M42), M43(M43), M44(M44) {}

		//Returns an instance of the identity matrix.
		[[nodiscard]] constexpr static const Matrix& Identity() noexcept;

		//Gets and sets the up vector of the Matrix.
		[[nodiscard]] constexpr Vector3 Up() const noexcept { return Vector3(M21, M22, M23); }
		//Gets and sets the down vector of the Matrix.
		[[nodiscard]] constexpr Vector3 Down() const noexcept { return Vector3(-M21, -M22, -M23); }
		//Gets and sets the right vector of the Matrix.
		[[nodiscard]] constexpr Vector3 Right() const noexcept { return Vector3(M11, M12, M13); }
		//Gets and sets the left vector of the Matrix.
		[[nodiscard]] constexpr Vector3 Left() const noexcept { return Vector3(-M11, -M12, -M13); }
		//Gets and sets the forward vector of the Matrix.
		[[nodiscard]] constexpr Vector3 Forward() const noexcept { return Vector3(-M31, -M32, -M33); }
		//Gets and sets the backward vector of the Matrix.
		[[nodiscard]] constexpr Vector3 Backward() const noexcept { return Vector3(M31, M32, M33); }
		//Gets and sets the translation vector of the Matrix.
		[[nodiscard]] constexpr Vector3 Translation() const noexcept { return Vector3(M41, M42, M43); }

		//Gets and sets the up vector of the Matrix.
		[[nodiscard]] constexpr void Up(Vector3 const& value) noexcept {
			M21 = value.X;
			M22 = value.Y;
			M23 = value.Z;
		}

		//Gets and sets the down vector of the Matrix.
		[[nodiscard]] constexpr void Down(Vector3 const& value) noexcept {
			M21 = -value.X;
			M22 = -value.Y;
			M23 = -value.Z;
		}

		//Gets and sets the right vector of the Matrix.
		[[nodiscard]] constexpr void Right(Vector3 const& value) noexcept {
			M11 = value.X;
			M12 = value.Y;
			M13 = value.Z;
		}

		//Gets and sets the left vector of the Matrix.
		[[nodiscard]] constexpr void Left(Vector3 const& value) noexcept {
			M11 = -value.X;
			M12 = -value.Y;
			M13 = -value.Z;
		}

		//Gets and sets the forward vector of the Matrix.
		[[nodiscard]] constexpr void Forward(Vector3 const& value) noexcept {
			M31 = -value.X;
			M32 = -value.Y;
			M33 = -value.Z;
		}

		//Gets and sets the backward vector of the Matrix.
		[[nodiscard]] constexpr void Backward(Vector3 const& value) noexcept {
			M31 = value.X;
			M32 = value.Y;
			M33 = value.Z;
		}

		//Gets and sets the translation vector of the Matrix.
		[[nodiscard]] constexpr void Translation(Vector3 const& value) noexcept {
			M41 = value.X;
			M42 = value.Y;
			M43 = value.Z;
		}

		//Creates a spherical billboard that rotates around a specified object position.
		[[nodiscard]] static constexpr Matrix CreateBillboard(Vector3 const& objectPosition, Vector3 const& cameraPosition, Vector3 const& cameraUpVector, const Vector3* cameraForwardVector) noexcept {
			Vector3 result1;
			result1.X = objectPosition.X - cameraPosition.X;
			result1.Y = objectPosition.Y - cameraPosition.Y;
			result1.Z = objectPosition.Z - cameraPosition.Z;
			const auto d = result1.LengthSquared();

			if (static_cast<double>(d) < 9.9999997473787516E-05)
				result1 = cameraForwardVector ? -*cameraForwardVector : Vector3::Forward();
			else
				result1 = Vector3::Multiply(result1, 1.0f / std::sqrt(d));

			auto result2 = Vector3::Cross(cameraUpVector, result1);
			result2.Normalize();

			const auto result3 = Vector3::Cross(result1, result2);
			Matrix billboard;
			billboard.M11 = result2.X;
			billboard.M12 = result2.Y;
			billboard.M13 = result2.Z;
			billboard.M14 = 0.0f;
			billboard.M21 = result3.X;
			billboard.M22 = result3.Y;
			billboard.M23 = result3.Z;
			billboard.M24 = 0.0f;
			billboard.M31 = result1.X;
			billboard.M32 = result1.Y;
			billboard.M33 = result1.Z;
			billboard.M34 = 0.0f;
			billboard.M41 = objectPosition.X;
			billboard.M42 = objectPosition.Y;
			billboard.M43 = objectPosition.Z;
			billboard.M44 = 1;
			return billboard;
		}

		//Creates a cylindrical billboard that rotates around a specified axis.
		[[nodiscard]] static constexpr Matrix CreateConstrainedBillboard(
			Vector3 const& objectPosition,
			Vector3 const& cameraPosition,
			Vector3 const& rotateAxis,
			const Vector3* cameraForwardVector,
			const Vector3* objectForwardVector) noexcept {
			Vector3 result1;
			result1.X = objectPosition.X - cameraPosition.X;
			result1.Y = objectPosition.Y - cameraPosition.Y;
			result1.Z = objectPosition.Z - cameraPosition.Z;
			const auto d = result1.LengthSquared();
			if (static_cast<double>(d) < 9.9999997473787516E-05)
				result1 = cameraForwardVector ? -*cameraForwardVector : Vector3::Forward();
			else
				result1 = Vector3::Multiply(result1, 1.0f / std::sqrt(d));

			const auto vector2 = rotateAxis;
			float result2 = Vector3::Dot(rotateAxis, result1);
			Vector3 result3;
			Vector3 result4;
			if (MathHelper::Abs(result2) > 0.998254656791687) {
				if (objectForwardVector)
				{
					result3 = *objectForwardVector;
					result2 = Vector3::Dot(rotateAxis, result3);

					if (MathHelper::Abs(result2) > 0.998254656791687)
						result3 = MathHelper::Abs((rotateAxis.X * Vector3::Forward().X + rotateAxis.Y * Vector3::Forward().Y + rotateAxis.Z * Vector3::Forward().Z)) > 0.998254656791687 ? Vector3::Right() : Vector3::Forward();
				}
				else
					result3 = MathHelper::Abs((rotateAxis.X * Vector3::Forward().X + rotateAxis.Y * Vector3::Forward().Y + rotateAxis.Z * Vector3::Forward().Z)) > 0.998254656791687 ? Vector3::Right() : Vector3::Forward();

				result4 = Vector3::Cross(rotateAxis, result3);
				result4.Normalize();
				result3 = Vector3::Cross(result4, rotateAxis);
				result3.Normalize();
			}
			else
			{
				result4 = Vector3::Cross(rotateAxis, result1);
				result4.Normalize();
				result3 = Vector3::Cross(result4, vector2);
				result3.Normalize();
			}
			Matrix constrainedBillboard;
			constrainedBillboard.M11 = result4.X;
			constrainedBillboard.M12 = result4.Y;
			constrainedBillboard.M13 = result4.Z;
			constrainedBillboard.M14 = 0.0f;
			constrainedBillboard.M21 = vector2.X;
			constrainedBillboard.M22 = vector2.Y;
			constrainedBillboard.M23 = vector2.Z;
			constrainedBillboard.M24 = 0.0f;
			constrainedBillboard.M31 = result3.X;
			constrainedBillboard.M32 = result3.Y;
			constrainedBillboard.M33 = result3.Z;
			constrainedBillboard.M34 = 0.0f;
			constrainedBillboard.M41 = objectPosition.X;
			constrainedBillboard.M42 = objectPosition.Y;
			constrainedBillboard.M43 = objectPosition.Z;
			constrainedBillboard.M44 = 1.0f;
			return constrainedBillboard;
		}

		//Creates a translation Matrix.
		[[nodiscard]] static constexpr Matrix CreateTranslation(Vector3 const& position) noexcept {
			Matrix translation;
			translation.M11 = 1.0f;
			translation.M12 = 0.0f;
			translation.M13 = 0.0f;
			translation.M14 = 0.0f;
			translation.M21 = 0.0f;
			translation.M22 = 1.0f;
			translation.M23 = 0.0f;
			translation.M24 = 0.0f;
			translation.M31 = 0.0f;
			translation.M32 = 0.0f;
			translation.M33 = 1.0f;
			translation.M34 = 0.0f;
			translation.M41 = position.X;
			translation.M42 = position.Y;
			translation.M43 = position.Z;
			translation.M44 = 1.0f;
			return translation;
		}

		//Creates a translation Matrix.
		[[nodiscard]] static constexpr Matrix CreateTranslation(float xPosition, float yPosition, float zPosition) noexcept {
			Matrix translation;
			translation.M11 = 1.0f;
			translation.M12 = 0.0f;
			translation.M13 = 0.0f;
			translation.M14 = 0.0f;
			translation.M21 = 0.0f;
			translation.M22 = 1.0f;
			translation.M23 = 0.0f;
			translation.M24 = 0.0f;
			translation.M31 = 0.0f;
			translation.M32 = 0.0f;
			translation.M33 = 1.0f;
			translation.M34 = 0.0f;
			translation.M41 = xPosition;
			translation.M42 = yPosition;
			translation.M43 = zPosition;
			translation.M44 = 1.0f;
			return translation;
		}

		//Creates a scaling Matrix.
		[[nodiscard]] static constexpr Matrix CreateScale(float xScale, float yScale, float zScale) noexcept {
			Matrix scale;
			scale.M11 = xScale;
			scale.M12 = 0.0f;
			scale.M13 = 0.0f;
			scale.M14 = 0.0f;
			scale.M21 = 0.0f;
			scale.M22 = yScale;
			scale.M23 = 0.0f;
			scale.M24 = 0.0f;
			scale.M31 = 0.0f;
			scale.M32 = 0.0f;
			scale.M33 = zScale;
			scale.M34 = 0.0f;
			scale.M41 = 0.0f;
			scale.M42 = 0.0f;
			scale.M43 = 0.0f;
			scale.M44 = 1.0f;
			return scale;
		}

		//Creates a scaling Matrix.
		[[nodiscard]] static constexpr Matrix CreateScale(Vector3 const& scales) noexcept {
			Matrix scale;
			scale.M11 = scales.X;
			scale.M12 = 0.0f;
			scale.M13 = 0.0f;
			scale.M14 = 0.0f;
			scale.M21 = 0.0f;
			scale.M22 = scales.Y;
			scale.M23 = 0.0f;
			scale.M24 = 0.0f;
			scale.M31 = 0.0f;
			scale.M32 = 0.0f;
			scale.M33 = scales.Z;
			scale.M34 = 0.0f;
			scale.M41 = 0.0f;
			scale.M42 = 0.0f;
			scale.M43 = 0.0f;
			scale.M44 = 1.0f;
			return scale;
		}

		//Creates a scaling Matrix.
		[[nodiscard]] static constexpr Matrix CreateScale(float scale) noexcept {
			Matrix scale1;
			scale1.M11 = scale;
			scale1.M12 = 0.0f;
			scale1.M13 = 0.0f;
			scale1.M14 = 0.0f;
			scale1.M21 = 0.0f;
			scale1.M22 = scale;
			scale1.M23 = 0.0f;
			scale1.M24 = 0.0f;
			scale1.M31 = 0.0f;
			scale1.M32 = 0.0f;
			scale1.M33 = scale;
			scale1.M34 = 0.0f;
			scale1.M41 = 0.0f;
			scale1.M42 = 0.0f;
			scale1.M43 = 0.0f;
			scale1.M44 = 1.0f;
			return scale1;
		}

		//Returns an x-axis rotation matrix.
		[[nodiscard]] static constexpr Matrix CreateRotationX(float radians) noexcept {
			const auto num1 = MathHelper::Cos(radians);
			const auto num2 = MathHelper::Sin(radians);
			Matrix rotationX;
			rotationX.M11 = 1.0f;
			rotationX.M12 = 0.0f;
			rotationX.M13 = 0.0f;
			rotationX.M14 = 0.0f;
			rotationX.M21 = 0.0f;
			rotationX.M22 = num1;
			rotationX.M23 = num2;
			rotationX.M24 = 0.0f;
			rotationX.M31 = 0.0f;
			rotationX.M32 = -num2;
			rotationX.M33 = num1;
			rotationX.M34 = 0.0f;
			rotationX.M41 = 0.0f;
			rotationX.M42 = 0.0f;
			rotationX.M43 = 0.0f;
			rotationX.M44 = 1.00f;
			return rotationX;
		}

		//Returns a y-axis rotation matrix.
		[[nodiscard]] static constexpr Matrix CreateRotationY(float radians) noexcept {
			const auto num1 = MathHelper::Cos(radians);
			const auto num2 = MathHelper::Sin(radians);
			Matrix rotationY;
			rotationY.M11 = num1;
			rotationY.M12 = 0.0f;
			rotationY.M13 = -num2;
			rotationY.M14 = 0.0f;
			rotationY.M21 = 0.0f;
			rotationY.M22 = 1.0f;
			rotationY.M23 = 0.0f;
			rotationY.M24 = 0.0f;
			rotationY.M31 = num2;
			rotationY.M32 = 0.0f;
			rotationY.M33 = num1;
			rotationY.M34 = 0.0f;
			rotationY.M41 = 0.0f;
			rotationY.M42 = 0.0f;
			rotationY.M43 = 0.0f;
			rotationY.M44 = 1.0f;
			return rotationY;
		}

		//Returns an z-axis rotation matrix.
		[[nodiscard]] static constexpr Matrix CreateRotationZ(float radians) noexcept {
			const auto num1 = MathHelper::Cos(radians);
			const auto num2 = MathHelper::Sin(radians);
			Matrix rotationZ;
			rotationZ.M11 = num1;
			rotationZ.M12 = num2;
			rotationZ.M13 = 0.0f;
			rotationZ.M14 = 0.0f;
			rotationZ.M21 = -num2;
			rotationZ.M22 = num1;
			rotationZ.M23 = 0.0f;
			rotationZ.M24 = 0.0f;
			rotationZ.M31 = 0.0f;
			rotationZ.M32 = 0.0f;
			rotationZ.M33 = 1.0f;
			rotationZ.M34 = 0.0f;
			rotationZ.M41 = 0.0f;
			rotationZ.M42 = 0.0f;
			rotationZ.M43 = 0.0f;
			rotationZ.M44 = 1.0f;
			return rotationZ;
		}

		//Creates a new Matrix that rotates around an arbitrary vector.
		[[nodiscard]] static constexpr Matrix CreateFromAxisAngle(Vector3 const& axis, float angle) noexcept {
			const auto x = axis.X;
			const auto y = axis.Y;
			const auto z = axis.Z;
			const auto num1 = MathHelper::Sin(angle);
			const auto num2 = MathHelper::Cos(angle);
			const auto num3 = x * x;
			const auto num4 = y * y;
			const auto num5 = z * z;
			const auto num6 = x * y;
			const auto num7 = x * z;
			const auto num8 = y * z;
			Matrix fromAxisAngle;
			fromAxisAngle.M11 = num3 + num2 * (1.0f - num3);
			fromAxisAngle.M12 = (num6 - num2 * num6 + num1 * z);
			fromAxisAngle.M13 = (num7 - num2 * num7 - num1 * y);
			fromAxisAngle.M14 = 0.0f;
			fromAxisAngle.M21 = (num6 - num2 * num6 - num1 * z);
			fromAxisAngle.M22 = num4 + num2 * (1.0f - num4);
			fromAxisAngle.M23 = (num8 - num2 * num8 + num1 * x);
			fromAxisAngle.M24 = 0.0f;
			fromAxisAngle.M31 = (num7 - num2 * num7 + num1 * y);
			fromAxisAngle.M32 = (num8 - num2 * num8 - num1 * x);
			fromAxisAngle.M33 = num5 + num2 * (1.0f - num5);
			fromAxisAngle.M34 = 0.0f;
			fromAxisAngle.M41 = 0.0f;
			fromAxisAngle.M42 = 0.0f;
			fromAxisAngle.M43 = 0.0f;
			fromAxisAngle.M44 = 1.0f;
			return fromAxisAngle;
		}

		//Builds a perspective projection matrix based on a field of view.
		[[nodiscard]] static constexpr Matrix CreatePerspectiveFieldOfView(float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance) noexcept {
			assert((fieldOfView > 0.0 || fieldOfView < 3.1415927410125732) && "fieldOfView out of range.");
			assert(nearPlaneDistance > 0.0 && "nearPlaneDistance <= 0");
			assert(farPlaneDistance > 0.0 && "farPlaneDistance <= 0");
			assert((nearPlaneDistance < farPlaneDistance) && "(nearPlaneDistance >= farPlaneDistance");

			const auto num1 = 1.0f / MathHelper::Tan(fieldOfView * 0.5f);
			const auto num2 = num1 / aspectRatio;
			Matrix perspectiveFieldOfView;
			perspectiveFieldOfView.M11 = num2;
			perspectiveFieldOfView.M12 = perspectiveFieldOfView.M13 = perspectiveFieldOfView.M14 = 0.0f;
			perspectiveFieldOfView.M22 = num1;
			perspectiveFieldOfView.M21 = perspectiveFieldOfView.M23 = perspectiveFieldOfView.M24 = 0.0f;
			perspectiveFieldOfView.M31 = perspectiveFieldOfView.M32 = 0.0f;
			perspectiveFieldOfView.M33 = farPlaneDistance / (nearPlaneDistance - farPlaneDistance);
			perspectiveFieldOfView.M34 = -1.0f;
			perspectiveFieldOfView.M41 = perspectiveFieldOfView.M42 = perspectiveFieldOfView.M44 = 0.0f;
			perspectiveFieldOfView.M43 = nearPlaneDistance * farPlaneDistance / (nearPlaneDistance - farPlaneDistance);
			return perspectiveFieldOfView;
		}

		//Builds a perspective projection matrix.
		[[nodiscard]] static constexpr Matrix CreatePerspective(float width, float height, float nearPlaneDistance, float farPlaneDistance) noexcept {
			assert(nearPlaneDistance > 0.0 && "nearPlaneDistance <= 0");
			assert(farPlaneDistance > 0.0 && "farPlaneDistance <= 0");
			assert((nearPlaneDistance < farPlaneDistance) && "(nearPlaneDistance >= farPlaneDistance");

			Matrix perspective;
			perspective.M11 = 2.0f * nearPlaneDistance / width;
			perspective.M12 = perspective.M13 = perspective.M14 = 0.0f;
			perspective.M22 = 2.0f * nearPlaneDistance / height;
			perspective.M21 = perspective.M23 = perspective.M24 = 0.0f;
			perspective.M33 = farPlaneDistance / (nearPlaneDistance - farPlaneDistance);
			perspective.M31 = perspective.M32 = 0.0f;
			perspective.M34 = -1.0f;
			perspective.M41 = perspective.M42 = perspective.M44 = 0.0f;
			perspective.M43 = (nearPlaneDistance * farPlaneDistance / (nearPlaneDistance - farPlaneDistance));
			return perspective;
		}

		//Builds a customized, orthogonal projection matrix.
		[[nodiscard]] static constexpr Matrix CreatePerspectiveOffCenter(float left, float right, float bottom, float top, float nearPlaneDistance, float farPlaneDistance) noexcept {
			assert(nearPlaneDistance > 0.0 && "nearPlaneDistance <= 0");
			assert(farPlaneDistance > 0.0 && "farPlaneDistance <= 0");
			assert((nearPlaneDistance < farPlaneDistance) && "(nearPlaneDistance >= farPlaneDistance");

			Matrix perspectiveOffCenter;
			perspectiveOffCenter.M11 = (2.0F * nearPlaneDistance / (right - left));
			perspectiveOffCenter.M12 = perspectiveOffCenter.M13 = perspectiveOffCenter.M14 = 0.0f;
			perspectiveOffCenter.M22 = (2.0F * nearPlaneDistance / (top - bottom));
			perspectiveOffCenter.M21 = perspectiveOffCenter.M23 = perspectiveOffCenter.M24 = 0.0f;
			perspectiveOffCenter.M31 = (left + right) / (right - left);
			perspectiveOffCenter.M32 = (top + bottom) / (top - bottom);
			perspectiveOffCenter.M33 = farPlaneDistance / (nearPlaneDistance - farPlaneDistance);
			perspectiveOffCenter.M34 = -1.0f;
			perspectiveOffCenter.M43 = (nearPlaneDistance * farPlaneDistance / (nearPlaneDistance - farPlaneDistance));
			perspectiveOffCenter.M41 = perspectiveOffCenter.M42 = perspectiveOffCenter.M44 = 0.0f;
			return perspectiveOffCenter;
		}

		//Builds an orthogonal projection matrix.
		[[nodiscard]] static constexpr Matrix CreateOrthographic(float width, float height, float zNearPlane, float zFarPlane) noexcept {
			Matrix orthographic;
			orthographic.M11 = 2.0f / width;
			orthographic.M12 = orthographic.M13 = orthographic.M14 = 0.0f;
			orthographic.M22 = 2.0f / height;
			orthographic.M21 = orthographic.M23 = orthographic.M24 = 0.0f;
			orthographic.M33 = (1.0F / (zNearPlane - zFarPlane));
			orthographic.M31 = orthographic.M32 = orthographic.M34 = 0.0f;
			orthographic.M41 = orthographic.M42 = 0.0f;
			orthographic.M43 = zNearPlane / (zNearPlane - zFarPlane);
			orthographic.M44 = 1.0f;
			return orthographic;
		}

		//Builds a customized, orthogonal projection matrix.
		[[nodiscard]] static constexpr Matrix CreateOrthographicOffCenter(
			float left, float right, float bottom, float top, float zNearPlane, float zFarPlane) noexcept {
			Matrix orthographicOffCenter;
			orthographicOffCenter.M11 = (2.0F / (right - left));
			orthographicOffCenter.M12 = orthographicOffCenter.M13 = orthographicOffCenter.M14 = 0.0f;
			orthographicOffCenter.M22 = (2.0F / (top - bottom));
			orthographicOffCenter.M21 = orthographicOffCenter.M23 = orthographicOffCenter.M24 = 0.0f;
			orthographicOffCenter.M33 = (1.0F / (zNearPlane - zFarPlane));
			orthographicOffCenter.M31 = orthographicOffCenter.M32 = orthographicOffCenter.M34 = 0.0f;
			orthographicOffCenter.M41 = ((left + right) / (left - right));
			orthographicOffCenter.M42 = ((top + bottom) / (bottom - top));
			orthographicOffCenter.M43 = zNearPlane / (zNearPlane - zFarPlane);
			orthographicOffCenter.M44 = 1.0f;
			return orthographicOffCenter;
		}

		//Creates a view matrix.
		[[nodiscard]] static constexpr Matrix CreateLookAt(
			Vector3 const& cameraPosition, Vector3 const& cameraTarget, Vector3 const& cameraUpVector) noexcept {
			const auto vector3_1 = Vector3::Normalize(cameraPosition - cameraTarget);
			const auto vector3_2 = Vector3::Normalize(Vector3::Cross(cameraUpVector, vector3_1));
			const auto vector1 = Vector3::Cross(vector3_1, vector3_2);
			Matrix lookAt;
			lookAt.M11 = vector3_2.X;
			lookAt.M12 = vector1.X;
			lookAt.M13 = vector3_1.X;
			lookAt.M14 = 0.0f;
			lookAt.M21 = vector3_2.Y;
			lookAt.M22 = vector1.Y;
			lookAt.M23 = vector3_1.Y;
			lookAt.M24 = 0.0f;
			lookAt.M31 = vector3_2.Z;
			lookAt.M32 = vector1.Z;
			lookAt.M33 = vector3_1.Z;
			lookAt.M34 = 0.0f;
			lookAt.M41 = -Vector3::Dot(vector3_2, cameraPosition);
			lookAt.M42 = -Vector3::Dot(vector1, cameraPosition);
			lookAt.M43 = -Vector3::Dot(vector3_1, cameraPosition);
			lookAt.M44 = 1.0f;
			return lookAt;
		}
		//Creates a world matrix.
		[[nodiscard]] static constexpr Matrix CreateWorld(
			Vector3 const& position, Vector3 const& forward, Vector3 const& up) noexcept {
			const auto vector3_1 = Vector3::Normalize(-forward);
			const auto vector2 = Vector3::Normalize(Vector3::Cross(up, vector3_1));
			const auto vector3_2 = Vector3::Cross(vector3_1, vector2);
			Matrix world;
			world.M11 = vector2.X;
			world.M12 = vector2.Y;
			world.M13 = vector2.Z;
			world.M14 = 0.0f;
			world.M21 = vector3_2.X;
			world.M22 = vector3_2.Y;
			world.M23 = vector3_2.Z;
			world.M24 = 0.0f;
			world.M31 = vector3_1.X;
			world.M32 = vector3_1.Y;
			world.M33 = vector3_1.Z;
			world.M34 = 0.0f;
			world.M41 = position.X;
			world.M42 = position.Y;
			world.M43 = position.Z;
			world.M44 = 1.0f;
			return world;
		}

		//Creates a rotation Matrix from a Quaternion.
		[[nodiscard]] static constexpr Matrix CreateFromQuaternion(Quaternion const& quaternion) noexcept;
		//Creates a new rotation matrix from a specified yaw, pitch, and roll.
		[[nodiscard]] static constexpr Matrix CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept;
		[[nodiscard]] static constexpr Matrix CreateShadow(Vector3 const& lightDirection, Plane const& plane) noexcept;
		[[nodiscard]] static constexpr Matrix CreateReflection(Plane value) noexcept;

		//Transforms a Matrix by applying a Quaternion rotation.
		[[nodiscard]] static constexpr Matrix Transform(Matrix const& value, Quaternion const& rotation) noexcept;

		//Transposes the rows and columns of a matrix.
		[[nodiscard]] static constexpr Matrix Transpose(Matrix const& matrix) noexcept {
			Matrix matrix1;
			matrix1.M11 = matrix.M11;
			matrix1.M12 = matrix.M21;
			matrix1.M13 = matrix.M31;
			matrix1.M14 = matrix.M41;
			matrix1.M21 = matrix.M12;
			matrix1.M22 = matrix.M22;
			matrix1.M23 = matrix.M32;
			matrix1.M24 = matrix.M42;
			matrix1.M31 = matrix.M13;
			matrix1.M32 = matrix.M23;
			matrix1.M33 = matrix.M33;
			matrix1.M34 = matrix.M43;
			matrix1.M41 = matrix.M14;
			matrix1.M42 = matrix.M24;
			matrix1.M43 = matrix.M34;
			matrix1.M44 = matrix.M44;
			return matrix1;
		}

		//Calculates the determinant of the matrix.
		[[nodiscard]] constexpr float Determinant() const noexcept {
			const auto num1 = (M33 * M44 - M34 * M43);
			const auto num2 = (M32 * M44 - M34 * M42);
			const auto num3 = (M32 * M43 - M33 * M42);
			const auto num4 = (M31 * M44 - M34 * M41);
			const auto num5 = (M31 * M43 - M33 * M41);
			const auto num6 = (M31 * M42 - M32 * M41);
			return
				(M11 * (M22 * num1 - M23 * num2 + M24 * num3) - M12
					* (M21 * num1 - M23 * num4 + M24 * num5) + M13
					* (M21 * num2 - M22 * num4 + M24 * num6) - M14
					* (M21 * num3 - M22 * num5 + M23 * num6));
		}

		//Calculates the inverse of a matrix.
		[[nodiscard]] static constexpr Matrix Invert(Matrix const& matrix) noexcept {
			const auto m11 = matrix.M11;
			const auto m12 = matrix.M12;
			const auto m13 = matrix.M13;
			const auto m14 = matrix.M14;
			const auto m21 = matrix.M21;
			const auto m22 = matrix.M22;
			const auto m23 = matrix.M23;
			const auto m24 = matrix.M24;
			const auto m31 = matrix.M31;
			const auto m32 = matrix.M32;
			const auto m33 = matrix.M33;
			const auto m34 = matrix.M34;
			const auto m41 = matrix.M41;
			const auto m42 = matrix.M42;
			const auto m43 = matrix.M43;
			const auto m44 = matrix.M44;

			const auto num1 = (m33 * m44 - m34 * m43);
			const auto num2 = (m32 * m44 - m34 * m42);
			const auto num3 = (m32 * m43 - m33 * m42);
			const auto num4 = (m31 * m44 - m34 * m41);
			const auto num5 = (m31 * m43 - m33 * m41);
			const auto num6 = (m31 * m42 - m32 * m41);
			const auto num7 = (m22 * num1 - m23 * num2 + m24 * num3);
			const auto num8 = -(m21 * num1 - m23 * num4 + m24 * num5);
			const auto num9 = (m21 * num2 - m22 * num4 + m24 * num6);
			const auto num10 = -(m21 * num3 - m22 * num5 + m23 * num6);
			const auto num11 = (1.0F / (m11 * num7 + m12 * num8 + m13 * num9 + m14 * num10));

			Matrix matrix1;
			matrix1.M11 = num7 * num11;
			matrix1.M21 = num8 * num11;
			matrix1.M31 = num9 * num11;
			matrix1.M41 = num10 * num11;
			matrix1.M12 = -(m12 * num1 - m13 * num2 + m14 * num3) * num11;
			matrix1.M22 = (m11 * num1 - m13 * num4 + m14 * num5) * num11;
			matrix1.M32 = -(m11 * num2 - m12 * num4 + m14 * num6) * num11;
			matrix1.M42 = (m11 * num3 - m12 * num5 + m13 * num6) * num11;

			const auto num12 = (m23 * m44 - m24 * m43);
			const auto num13 = (m22 * m44 - m24 * m42);
			const auto num14 = (m22 * m43 - m23 * m42);
			const auto num15 = (m21 * m44 - m24 * m41);
			const auto num16 = (m21 * m43 - m23 * m41);
			const auto num17 = (m21 * m42 - m22 * m41);

			matrix1.M13 = (m12 * num12 - m13 * num13 + m14 * num14) * num11;
			matrix1.M23 = -(m11 * num12 - m13 * num15 + m14 * num16) * num11;
			matrix1.M33 = (m11 * num13 - m12 * num15 + m14 * num17) * num11;
			matrix1.M43 = -(m11 * num14 - m12 * num16 + m13 * num17) * num11;

			const auto num18 = (m23 * m34 - m24 * m33);
			const auto num19 = (m22 * m34 - m24 * m32);
			const auto num20 = (m22 * m33 - m23 * m32);
			const auto num21 = (m21 * m34 - m24 * m31);
			const auto num22 = (m21 * m33 - m23 * m31);
			const auto num23 = (m21 * m32 - m22 * m31);

			matrix1.M14 = -(m12 * num18 - m13 * num19 + m14 * num20) * num11;
			matrix1.M24 = (m11 * num18 - m13 * num21 + m14 * num22) * num11;
			matrix1.M34 = -(m11 * num19 - m12 * num21 + m14 * num23) * num11;
			matrix1.M44 = (m11 * num20 - m12 * num22 + m13 * num23) * num11;

			return matrix1;
		}

		//Linearly interpolates between the corresponding values of two matrices.
		[[nodiscard]] static constexpr Matrix Lerp(Matrix const& matrix1, Matrix const& matrix2, float amount) noexcept {
			Matrix matrix;
			matrix.M11 = matrix1.M11 + (matrix2.M11 - matrix1.M11) * amount;
			matrix.M12 = matrix1.M12 + (matrix2.M12 - matrix1.M12) * amount;
			matrix.M13 = matrix1.M13 + (matrix2.M13 - matrix1.M13) * amount;
			matrix.M14 = matrix1.M14 + (matrix2.M14 - matrix1.M14) * amount;
			matrix.M21 = matrix1.M21 + (matrix2.M21 - matrix1.M21) * amount;
			matrix.M22 = matrix1.M22 + (matrix2.M22 - matrix1.M22) * amount;
			matrix.M23 = matrix1.M23 + (matrix2.M23 - matrix1.M23) * amount;
			matrix.M24 = matrix1.M24 + (matrix2.M24 - matrix1.M24) * amount;
			matrix.M31 = matrix1.M31 + (matrix2.M31 - matrix1.M31) * amount;
			matrix.M32 = matrix1.M32 + (matrix2.M32 - matrix1.M32) * amount;
			matrix.M33 = matrix1.M33 + (matrix2.M33 - matrix1.M33) * amount;
			matrix.M34 = matrix1.M34 + (matrix2.M34 - matrix1.M34) * amount;
			matrix.M41 = matrix1.M41 + (matrix2.M41 - matrix1.M41) * amount;
			matrix.M42 = matrix1.M42 + (matrix2.M42 - matrix1.M42) * amount;
			matrix.M43 = matrix1.M43 + (matrix2.M43 - matrix1.M43) * amount;
			matrix.M44 = matrix1.M44 + (matrix2.M44 - matrix1.M44) * amount;
			return matrix;
		}

		//Negates individual elements of a matrix.
		[[nodiscard]] static constexpr Matrix Negate(Matrix const& matrix) noexcept {
			Matrix matrix1;
			matrix1.M11 = -matrix.M11;
			matrix1.M12 = -matrix.M12;
			matrix1.M13 = -matrix.M13;
			matrix1.M14 = -matrix.M14;
			matrix1.M21 = -matrix.M21;
			matrix1.M22 = -matrix.M22;
			matrix1.M23 = -matrix.M23;
			matrix1.M24 = -matrix.M24;
			matrix1.M31 = -matrix.M31;
			matrix1.M32 = -matrix.M32;
			matrix1.M33 = -matrix.M33;
			matrix1.M34 = -matrix.M34;
			matrix1.M41 = -matrix.M41;
			matrix1.M42 = -matrix.M42;
			matrix1.M43 = -matrix.M43;
			matrix1.M44 = -matrix.M44;
			return matrix1;
		}

		//Adds a matrix to another matrix.
		[[nodiscard]] static constexpr Matrix Add(Matrix const& matrix1, Matrix const& matrix2) noexcept {
			Matrix matrix;
			matrix.M11 = matrix1.M11 + matrix2.M11;
			matrix.M12 = matrix1.M12 + matrix2.M12;
			matrix.M13 = matrix1.M13 + matrix2.M13;
			matrix.M14 = matrix1.M14 + matrix2.M14;
			matrix.M21 = matrix1.M21 + matrix2.M21;
			matrix.M22 = matrix1.M22 + matrix2.M22;
			matrix.M23 = matrix1.M23 + matrix2.M23;
			matrix.M24 = matrix1.M24 + matrix2.M24;
			matrix.M31 = matrix1.M31 + matrix2.M31;
			matrix.M32 = matrix1.M32 + matrix2.M32;
			matrix.M33 = matrix1.M33 + matrix2.M33;
			matrix.M34 = matrix1.M34 + matrix2.M34;
			matrix.M41 = matrix1.M41 + matrix2.M41;
			matrix.M42 = matrix1.M42 + matrix2.M42;
			matrix.M43 = matrix1.M43 + matrix2.M43;
			matrix.M44 = matrix1.M44 + matrix2.M44;
			return matrix;
		}

		//Subtracts matrices.
		[[nodiscard]] static constexpr Matrix Subtract(Matrix const& matrix1, Matrix const& matrix2) noexcept {
			Matrix matrix;
			matrix.M11 = matrix1.M11 - matrix2.M11;
			matrix.M12 = matrix1.M12 - matrix2.M12;
			matrix.M13 = matrix1.M13 - matrix2.M13;
			matrix.M14 = matrix1.M14 - matrix2.M14;
			matrix.M21 = matrix1.M21 - matrix2.M21;
			matrix.M22 = matrix1.M22 - matrix2.M22;
			matrix.M23 = matrix1.M23 - matrix2.M23;
			matrix.M24 = matrix1.M24 - matrix2.M24;
			matrix.M31 = matrix1.M31 - matrix2.M31;
			matrix.M32 = matrix1.M32 - matrix2.M32;
			matrix.M33 = matrix1.M33 - matrix2.M33;
			matrix.M34 = matrix1.M34 - matrix2.M34;
			matrix.M41 = matrix1.M41 - matrix2.M41;
			matrix.M42 = matrix1.M42 - matrix2.M42;
			matrix.M43 = matrix1.M43 - matrix2.M43;
			matrix.M44 = matrix1.M44 - matrix2.M44;
			return matrix;
		}

		//Multiplies a matrix by a scalar value or another matrix.
		[[nodiscard]] static constexpr Matrix Multiply(Matrix const& matrix1, Matrix const& matrix2) noexcept {
			Matrix matrix;
			matrix.M11 = (matrix1.M11 * matrix2.M11 + matrix1.M12 * matrix2.M21 + matrix1.M13 * matrix2.M31 + matrix1.M14 * matrix2.M41);
			matrix.M12 = (matrix1.M11 * matrix2.M12 + matrix1.M12 * matrix2.M22 + matrix1.M13 * matrix2.M32 + matrix1.M14 * matrix2.M42);
			matrix.M13 = (matrix1.M11 * matrix2.M13 + matrix1.M12 * matrix2.M23 + matrix1.M13 * matrix2.M33 + matrix1.M14 * matrix2.M43);
			matrix.M14 = (matrix1.M11 * matrix2.M14 + matrix1.M12 * matrix2.M24 + matrix1.M13 * matrix2.M34 + matrix1.M14 * matrix2.M44);
			matrix.M21 = (matrix1.M21 * matrix2.M11 + matrix1.M22 * matrix2.M21 + matrix1.M23 * matrix2.M31 + matrix1.M24 * matrix2.M41);
			matrix.M22 = (matrix1.M21 * matrix2.M12 + matrix1.M22 * matrix2.M22 + matrix1.M23 * matrix2.M32 + matrix1.M24 * matrix2.M42);
			matrix.M23 = (matrix1.M21 * matrix2.M13 + matrix1.M22 * matrix2.M23 + matrix1.M23 * matrix2.M33 + matrix1.M24 * matrix2.M43);
			matrix.M24 = (matrix1.M21 * matrix2.M14 + matrix1.M22 * matrix2.M24 + matrix1.M23 * matrix2.M34 + matrix1.M24 * matrix2.M44);
			matrix.M31 = (matrix1.M31 * matrix2.M11 + matrix1.M32 * matrix2.M21 + matrix1.M33 * matrix2.M31 + matrix1.M34 * matrix2.M41);
			matrix.M32 = (matrix1.M31 * matrix2.M12 + matrix1.M32 * matrix2.M22 + matrix1.M33 * matrix2.M32 + matrix1.M34 * matrix2.M42);
			matrix.M33 = (matrix1.M31 * matrix2.M13 + matrix1.M32 * matrix2.M23 + matrix1.M33 * matrix2.M33 + matrix1.M34 * matrix2.M43);
			matrix.M34 = (matrix1.M31 * matrix2.M14 + matrix1.M32 * matrix2.M24 + matrix1.M33 * matrix2.M34 + matrix1.M34 * matrix2.M44);
			matrix.M41 = (matrix1.M41 * matrix2.M11 + matrix1.M42 * matrix2.M21 + matrix1.M43 * matrix2.M31 + matrix1.M44 * matrix2.M41);
			matrix.M42 = (matrix1.M41 * matrix2.M12 + matrix1.M42 * matrix2.M22 + matrix1.M43 * matrix2.M32 + matrix1.M44 * matrix2.M42);
			matrix.M43 = (matrix1.M41 * matrix2.M13 + matrix1.M42 * matrix2.M23 + matrix1.M43 * matrix2.M33 + matrix1.M44 * matrix2.M43);
			matrix.M44 = (matrix1.M41 * matrix2.M14 + matrix1.M42 * matrix2.M24 + matrix1.M43 * matrix2.M34 + matrix1.M44 * matrix2.M44);
			return matrix;
		}

		//Multiplies a matrix by a scalar value or another matrix.
		[[nodiscard]] static constexpr Matrix Multiply(Matrix const& matrix1, float scaleFactor) noexcept {
			float num = scaleFactor;
			Matrix matrix;
			matrix.M11 = matrix1.M11 * num;
			matrix.M12 = matrix1.M12 * num;
			matrix.M13 = matrix1.M13 * num;
			matrix.M14 = matrix1.M14 * num;
			matrix.M21 = matrix1.M21 * num;
			matrix.M22 = matrix1.M22 * num;
			matrix.M23 = matrix1.M23 * num;
			matrix.M24 = matrix1.M24 * num;
			matrix.M31 = matrix1.M31 * num;
			matrix.M32 = matrix1.M32 * num;
			matrix.M33 = matrix1.M33 * num;
			matrix.M34 = matrix1.M34 * num;
			matrix.M41 = matrix1.M41 * num;
			matrix.M42 = matrix1.M42 * num;
			matrix.M43 = matrix1.M43 * num;
			matrix.M44 = matrix1.M44 * num;
			return matrix;
		}

		//Divides a matrix by a scalar value or the components of another matrix.
		[[nodiscard]] static constexpr Matrix Divide(Matrix const& matrix1, Matrix const& matrix2) noexcept {
			Matrix matrix;
			matrix.M11 = matrix1.M11 / matrix2.M11;
			matrix.M12 = matrix1.M12 / matrix2.M12;
			matrix.M13 = matrix1.M13 / matrix2.M13;
			matrix.M14 = matrix1.M14 / matrix2.M14;
			matrix.M21 = matrix1.M21 / matrix2.M21;
			matrix.M22 = matrix1.M22 / matrix2.M22;
			matrix.M23 = matrix1.M23 / matrix2.M23;
			matrix.M24 = matrix1.M24 / matrix2.M24;
			matrix.M31 = matrix1.M31 / matrix2.M31;
			matrix.M32 = matrix1.M32 / matrix2.M32;
			matrix.M33 = matrix1.M33 / matrix2.M33;
			matrix.M34 = matrix1.M34 / matrix2.M34;
			matrix.M41 = matrix1.M41 / matrix2.M41;
			matrix.M42 = matrix1.M42 / matrix2.M42;
			matrix.M43 = matrix1.M43 / matrix2.M43;
			matrix.M44 = matrix1.M44 / matrix2.M44;
			return matrix;
		}

		//Divides a matrix by a scalar value or the components of another matrix.
		[[nodiscard]] static constexpr Matrix Divide(Matrix const& matrix1, float divider) noexcept {
			float num = 1.0f / divider;
			Matrix matrix;
			matrix.M11 = matrix1.M11 * num;
			matrix.M12 = matrix1.M12 * num;
			matrix.M13 = matrix1.M13 * num;
			matrix.M14 = matrix1.M14 * num;
			matrix.M21 = matrix1.M21 * num;
			matrix.M22 = matrix1.M22 * num;
			matrix.M23 = matrix1.M23 * num;
			matrix.M24 = matrix1.M24 * num;
			matrix.M31 = matrix1.M31 * num;
			matrix.M32 = matrix1.M32 * num;
			matrix.M33 = matrix1.M33 * num;
			matrix.M34 = matrix1.M34 * num;
			matrix.M41 = matrix1.M41 * num;
			matrix.M42 = matrix1.M42 * num;
			matrix.M43 = matrix1.M43 * num;
			matrix.M44 = matrix1.M44 * num;
			return matrix;
		}

		[[nodiscard]] constexpr Matrix operator-() const noexcept {
			return Matrix::Negate(*this);
		}

		[[nodiscard]] friend constexpr Matrix operator+(Matrix const& matrix1, Matrix const& matrix2) noexcept {
			return Matrix::Add(matrix1, matrix2);
		}

		[[nodiscard]] friend constexpr Matrix operator-(Matrix const& matrix1, Matrix const& matrix2) noexcept {
			return Matrix::Subtract(matrix1, matrix2);
		}

		[[nodiscard]] friend constexpr Matrix operator*(Matrix const& matrix1, Matrix const& matrix2) noexcept {
			return Matrix::Multiply(matrix1, matrix2);
		}

		[[nodiscard]] friend constexpr Matrix operator*(Matrix const& matrix, float factor) noexcept {
			return Matrix::Multiply(matrix, factor);
		}

		[[nodiscard]] friend constexpr Matrix operator*(float factor, Matrix const& matrix) noexcept {
			return Matrix::Multiply(matrix, factor);
		}

		[[nodiscard]] friend constexpr Matrix operator/(Matrix const& matrix1, Matrix const& matrix2) noexcept {
			return Matrix::Divide(matrix1, matrix2);
		}

		[[nodiscard]] friend constexpr Matrix operator/(Matrix const& matrix, float divider) noexcept {
			return Matrix::Divide(matrix, divider);
		}

		[[nodiscard]] constexpr bool operator==(const Matrix& other) const noexcept = default;
	};

	//////////////////////////////////////////////////////////////////
	// QUATERNION													//
	//////////////////////////////////////////////////////////////////

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
		constexpr Quaternion() noexcept = default;
		//Initializes a new instance of Quaternion.
		constexpr Quaternion(float X, float Y, float Z, float W) noexcept : X(X), Y(Y), Z(Z), W(W) {}
		//Initializes a new instance of Quaternion.
		constexpr Quaternion(Vector3 vectorPart, float scalarPart) noexcept : X(vectorPart.X), Y(vectorPart.Y), Z(vectorPart.Z), W(scalarPart) {}

		//Returns a Quaternion representing no rotation.
		[[nodiscard]] static constexpr Quaternion Identity() noexcept {
			return { 0.0f, 0.0f, 0.0f, 1.0f };
		}

		//Calculates the length squared of a Quaternion.
		[[nodiscard]] constexpr float LengthSquared() const noexcept {
			return X * X + Y * Y + Z * Z + W * W;
		}

		//Calculates the length of a Quaternion.
		[[nodiscard]] constexpr float Length() const noexcept { return MathHelper::Sqrt(LengthSquared()); }

		//Divides each component of a quaternion by the length of the quaternion.
		constexpr void Normalize() noexcept {
			const auto num = 1.0F / Length();
			X *= num;
			Y *= num;
			Z *= num;
			W *= num;
		}

		//Divides each component of a quaternion by the length of the quaternion.
		[[nodiscard]] static constexpr Quaternion Normalize(Quaternion quaternion) noexcept {
			quaternion.Normalize();
			return quaternion;
		}

		//Calculates the conjugate of a Quaternion.
		constexpr void Conjugate() noexcept {
			X = -X;
			Y = -Y;
			Z = -Z;
		}

		//Calculates the conjugate of a Quaternion.
		[[nodiscard]] static constexpr Quaternion Conjugate(Quaternion const& value) noexcept {
			Quaternion quaternion;
			quaternion.X = -value.X;
			quaternion.Y = -value.Y;
			quaternion.Z = -value.Z;
			quaternion.W = value.W;
			return quaternion;
		}

		//Returns the inverse of a Quaternion.
		[[nodiscard]] static constexpr Quaternion Inverse(Quaternion const& quaternion) noexcept {
			const auto num = 1.0f / quaternion.LengthSquared();
			Quaternion quaternion1;
			quaternion1.X = -quaternion.X * num;
			quaternion1.Y = -quaternion.Y * num;
			quaternion1.Z = -quaternion.Z * num;
			quaternion1.W = quaternion.W * num;
			return quaternion1;
		}

		//Creates a Quaternion from a vector and an angle to rotate about the vector.
		[[nodiscard]] static constexpr Quaternion CreateFromAxisAngle(Vector3 const& axis, float angle) noexcept {
			const auto num1 = angle * 0.5f;
			const auto num2 = MathHelper::Sin(num1);
			const auto num3 = MathHelper::Cos(num1);
			Quaternion fromAxisAngle;
			fromAxisAngle.X = axis.X * num2;
			fromAxisAngle.Y = axis.Y * num2;
			fromAxisAngle.Z = axis.Z * num2;
			fromAxisAngle.W = num3;
			return fromAxisAngle;
		}
		//Creates a new Quaternion from specified yaw, pitch, and roll angles.
		[[nodiscard]] static constexpr Quaternion CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept {
			const auto num1 = roll * 0.5f;
			const auto num2 = MathHelper::Sin(num1);
			const auto num3 = MathHelper::Cos(num1);
			const auto num4 = pitch * 0.5f;
			const auto num5 = MathHelper::Sin(num4);
			const auto num6 = MathHelper::Cos(num4);
			const auto num7 = yaw * 0.5f;
			const auto num8 = MathHelper::Sin(num7);
			const auto num9 = MathHelper::Cos(num7);
			Quaternion fromYawPitchRoll;
			fromYawPitchRoll.X = (num9 * num5 * num3 + num8 * num6 * num2);
			fromYawPitchRoll.Y = (num8 * num6 * num3 - num9 * num5 * num2);
			fromYawPitchRoll.Z = (num9 * num6 * num2 - num8 * num5 * num3);
			fromYawPitchRoll.W = (num9 * num6 * num3 + num8 * num5 * num2);
			return fromYawPitchRoll;
		}
		//Creates a Quaternion from a rotation Matrix.
		[[nodiscard]] static constexpr Quaternion CreateFromRotationMatrix(Matrix const& matrix) noexcept {
			const auto num1 = matrix.M11 + matrix.M22 + matrix.M33;

			Quaternion fromRotationMatrix;
			if (num1 > 0.0)
			{
				const auto num2 = MathHelper::Sqrt(num1 + 1.0F);
				fromRotationMatrix.W = num2 * 0.5f;
				const auto num3 = 0.5f / num2;
				fromRotationMatrix.X = (matrix.M23 - matrix.M32) * num3;
				fromRotationMatrix.Y = (matrix.M31 - matrix.M13) * num3;
				fromRotationMatrix.Z = (matrix.M12 - matrix.M21) * num3;
			}
			else if (matrix.M11 >= matrix.M22 && matrix.M11 >= matrix.M33)
			{
				const auto num4 = MathHelper::Sqrt(1.0F + matrix.M11 - matrix.M22 - matrix.M33);
				const auto num5 = 0.5f / num4;
				fromRotationMatrix.X = 0.5f * num4;
				fromRotationMatrix.Y = (matrix.M12 + matrix.M21) * num5;
				fromRotationMatrix.Z = (matrix.M13 + matrix.M31) * num5;
				fromRotationMatrix.W = (matrix.M23 - matrix.M32) * num5;
			}
			else if (matrix.M22 > matrix.M33)
			{
				const auto num6 = MathHelper::Sqrt(1.0F + matrix.M22 - matrix.M11 - matrix.M33);
				const auto num7 = 0.5f / num6;
				fromRotationMatrix.X = (matrix.M21 + matrix.M12) * num7;
				fromRotationMatrix.Y = 0.5f * num6;
				fromRotationMatrix.Z = (matrix.M32 + matrix.M23) * num7;
				fromRotationMatrix.W = (matrix.M31 - matrix.M13) * num7;
			}
			else
			{
				const auto num8 = MathHelper::Sqrt(1.0F + matrix.M33 - matrix.M11 - matrix.M22);
				const auto num9 = 0.5f / num8;
				fromRotationMatrix.X = (matrix.M31 + matrix.M13) * num9;
				fromRotationMatrix.Y = (matrix.M32 + matrix.M23) * num9;
				fromRotationMatrix.Z = 0.5f * num8;
				fromRotationMatrix.W = (matrix.M12 - matrix.M21) * num9;
			}
			return fromRotationMatrix;
		}

		//Calculates the dot product of two Quaternions.
		[[nodiscard]] static constexpr float Dot(Quaternion const& quaternion1, Quaternion const& quaternion2) noexcept {
			return quaternion1.X * quaternion2.X + quaternion1.Y * quaternion2.Y + quaternion1.Z * quaternion2.Z + quaternion1.W * quaternion2.W;
		}

		//Interpolates between two quaternions, using spherical linear interpolation.
		[[nodiscard]] static constexpr Quaternion Slerp(Quaternion const& quaternion1, Quaternion const& quaternion2, float amount) noexcept {
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
				const auto num4 = 1.0F / MathHelper::Sin(a);
				num2 = MathHelper::Sin((1.0F - num1) * a) * num4;
				num3 = flag ? -MathHelper::Sin(num1 * a) * num4 : MathHelper::Sin(num1 * a) * num4;
			}
			Quaternion quaternion;
			quaternion.X = num2 * quaternion1.X + num3 * quaternion2.X;
			quaternion.Y = num2 * quaternion1.Y + num3 * quaternion2.Y;
			quaternion.Z = num2 * quaternion1.Z + num3 * quaternion2.Z;
			quaternion.W = num2 * quaternion1.W + num3 * quaternion2.W;
			return quaternion;
		}

		//Linearly interpolates between two quaternions.
		[[nodiscard]] static constexpr Quaternion Lerp(Quaternion const& quaternion1, Quaternion const& quaternion2, float amount) noexcept {
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
			const auto num3 = 1.0f / MathHelper::Sqrt(quaternion.X * quaternion.X + quaternion.Y * quaternion.Y + quaternion.Z * quaternion.Z + quaternion.W * quaternion.W);
			quaternion.X *= num3;
			quaternion.Y *= num3;
			quaternion.Z *= num3;
			quaternion.W *= num3;
			return quaternion;
		}

		//Concatenates two Quaternions; the result represents the first rotation followed by the second rotation.
		[[nodiscard]] static constexpr Quaternion Concatenate(Quaternion const& value1, Quaternion const& value2) noexcept {
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
		[[nodiscard]] static constexpr Quaternion Negate(Quaternion const& quaternion) noexcept {
			Quaternion quaternion1;
			quaternion1.X = -quaternion.X;
			quaternion1.Y = -quaternion.Y;
			quaternion1.Z = -quaternion.Z;
			quaternion1.W = -quaternion.W;
			return quaternion1;
		}

		//Adds two Quaternions.
		[[nodiscard]] static constexpr Quaternion Add(Quaternion const& quaternion1, Quaternion const& quaternion2) noexcept {
			Quaternion quaternion;
			quaternion.X = quaternion1.X + quaternion2.X;
			quaternion.Y = quaternion1.Y + quaternion2.Y;
			quaternion.Z = quaternion1.Z + quaternion2.Z;
			quaternion.W = quaternion1.W + quaternion2.W;
			return quaternion;
		}

		//Subtracts a quaternion from another quaternion.
		[[nodiscard]] static constexpr Quaternion Subtract(Quaternion const& quaternion1, Quaternion const& quaternion2) noexcept {
			Quaternion quaternion;
			quaternion.X = quaternion1.X - quaternion2.X;
			quaternion.Y = quaternion1.Y - quaternion2.Y;
			quaternion.Z = quaternion1.Z - quaternion2.Z;
			quaternion.W = quaternion1.W - quaternion2.W;
			return quaternion;
		}

		//Multiplies a quaternion by a scalar or another quaternion.
		[[nodiscard]] static constexpr Quaternion Multiply(Quaternion const& quaternion1, Quaternion const& quaternion2) noexcept {
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
		[[nodiscard]] static constexpr Quaternion Multiply(Quaternion const& quaternion1, float scaleFactor) noexcept {
			Quaternion quaternion;
			quaternion.X = quaternion1.X * scaleFactor;
			quaternion.Y = quaternion1.Y * scaleFactor;
			quaternion.Z = quaternion1.Z * scaleFactor;
			quaternion.W = quaternion1.W * scaleFactor;
			return quaternion;
		}

		//Divides a Quaternion by another Quaternion.
		[[nodiscard]] static constexpr Quaternion Divide(Quaternion const& quaternion1, Quaternion const& quaternion2) noexcept {
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

		[[nodiscard]] constexpr Quaternion operator-(Quaternion const& quaternion) const noexcept {
			return Quaternion::Negate(quaternion);
		}

		[[nodiscard]] constexpr friend Quaternion operator+(Quaternion const& q1, Quaternion const& q2) noexcept {
			return Quaternion::Add(q1, q2);
		}

		[[nodiscard]] constexpr friend Quaternion operator-(Quaternion const& q1, Quaternion const& q2) noexcept {
			return Quaternion::Add(q1, q2);
		}

		[[nodiscard]] constexpr friend Quaternion operator*(Quaternion const& q1, Quaternion const& q2) noexcept {
			return Quaternion::Multiply(q1, q2);
		}

		[[nodiscard]] constexpr friend Quaternion operator/(Quaternion const& q1, Quaternion const& q2) noexcept {
			return Quaternion::Divide(q1, q2);
		}

		[[nodiscard]] constexpr friend Quaternion operator*(Quaternion const& q1, float scaleValue) noexcept {
			return Quaternion::Multiply(q1, scaleValue);
		}

		[[nodiscard]] constexpr friend Quaternion operator*(float scaleValue, Quaternion const& q1) noexcept {
			return Quaternion::Multiply(q1, scaleValue);
		}

		[[nodiscard]] constexpr bool operator==(const Quaternion& other) const noexcept = default;
	};

	//////////////////////////////////////////////////////////////////
	// IMPLS														//
	//////////////////////////////////////////////////////////////////

	constexpr Vector2 Vector2::Transform(Vector2 const& position, Matrix const& matrix) noexcept {
		const auto posx = (position.X * matrix.M11 + position.Y * matrix.M21) + matrix.M41;
		const auto posy = (position.X * matrix.M12 + position.Y * matrix.M22) + matrix.M42;

		return{ posx, posy };
	}

	constexpr Vector2 Vector2::TransformNormal(Vector2 const& normal, Matrix const& matrix) noexcept {
		const auto posx = normal.X * matrix.M11 + normal.Y * matrix.M21;
		const auto posy = normal.X * matrix.M12 + normal.Y * matrix.M22;
		return { posx, posy };
	}

	constexpr Vector3 Vector3::Transform(Vector3 const& position, Matrix const& matrix) noexcept {
		const auto num1 = (position.X * matrix.M11 + position.Y * matrix.M21 + position.Z * matrix.M31) + matrix.M41;
		const auto num2 = (position.X * matrix.M12 + position.Y * matrix.M22 + position.Z * matrix.M32) + matrix.M42;
		const auto num3 = (position.X * matrix.M13 + position.Y * matrix.M23 + position.Z * matrix.M33) + matrix.M43;
		Vector3 vector3;
		vector3.X = num1;
		vector3.Y = num2;
		vector3.Z = num3;
		return vector3;
	}

	constexpr Vector3 Vector3::TransformNormal(Vector3 const& normal, Matrix const& matrix) noexcept
	{
		const auto num1 = normal.X * matrix.M11 + normal.Y * matrix.M21 + normal.Z * matrix.M31;
		const auto num2 = normal.X * matrix.M12 + normal.Y * matrix.M22 + normal.Z * matrix.M32;
		const auto num3 = normal.X * matrix.M13 + normal.Y * matrix.M23 + normal.Z * matrix.M33;
		Vector3 vector3;
		vector3.X = num1;
		vector3.Y = num2;
		vector3.Z = num3;
		return vector3;
	}

	constexpr Vector4 Vector4::Transform(Vector2 const& position, Matrix const& matrix) noexcept
	{
		const auto num1 = (position.X * matrix.M11 + position.Y * matrix.M21) + matrix.M41;
		const auto num2 = (position.X * matrix.M12 + position.Y * matrix.M22) + matrix.M42;
		const auto num3 = (position.X * matrix.M13 + position.Y * matrix.M23) + matrix.M43;
		const auto num4 = (position.X * matrix.M14 + position.Y * matrix.M24) + matrix.M44;
		Vector4 vector4;
		vector4.X = num1;
		vector4.Y = num2;
		vector4.Z = num3;
		vector4.W = num4;
		return vector4;
	}

	constexpr Vector4 Vector4::Transform(Vector3 const& position, Matrix const& matrix) noexcept
	{
		const auto num1 = (position.X * matrix.M11 + position.Y * matrix.M21 + position.Z * matrix.M31) + matrix.M41;
		const auto num2 = (position.X * matrix.M12 + position.Y * matrix.M22 + position.Z * matrix.M32) + matrix.M42;
		const auto num3 = (position.X * matrix.M13 + position.Y * matrix.M23 + position.Z * matrix.M33) + matrix.M43;
		const auto num4 = (position.X * matrix.M14 + position.Y * matrix.M24 + position.Z * matrix.M34) + matrix.M44;
		Vector4 vector4;
		vector4.X = num1;
		vector4.Y = num2;
		vector4.Z = num3;
		vector4.W = num4;
		return vector4;
	}

	constexpr Vector4 Vector4::Transform(Vector4 const& vector, Matrix const& matrix) noexcept {
		const auto num1 = (vector.X * matrix.M11 + vector.Y * matrix.M21 + vector.Z * matrix.M31 + vector.W * matrix.M41);
		const auto num2 = (vector.X * matrix.M12 + vector.Y * matrix.M22 + vector.Z * matrix.M32 + vector.W * matrix.M42);
		const auto num3 = (vector.X * matrix.M13 + vector.Y * matrix.M23 + vector.Z * matrix.M33 + vector.W * matrix.M43);
		const auto num4 = (vector.X * matrix.M14 + vector.Y * matrix.M24 + vector.Z * matrix.M34 + vector.W * matrix.M44);
		Vector4 vector4;
		vector4.X = num1;
		vector4.Y = num2;
		vector4.Z = num3;
		vector4.W = num4;
		return vector4;
	}

	struct _Matrix {
		static constexpr Matrix Identity = Matrix(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	};

	constexpr const Matrix& Xna::Matrix::Identity() noexcept {
		return _Matrix::Identity;
	}

	constexpr void Vector2::Transform(
		std::span<const Vector2> sourceArray,
		const Matrix& matrix,
		std::span<Vector2> destinationArray) noexcept
	{
		for (std::size_t index = 0; index < sourceArray.size(); ++index)
		{
			float x = sourceArray[index].X;
			float y = sourceArray[index].Y;

			destinationArray[index].X = (x * matrix.M11) + (y * matrix.M21) + matrix.M41;
			destinationArray[index].Y = (x * matrix.M12) + (y * matrix.M22) + matrix.M42;
		}
	}

	constexpr void Vector2::Transform(
		std::span<const Vector2> sourceArray,
		std::size_t sourceIndex,
		Matrix const& matrix,
		std::span<Vector2> destinationArray,
		std::size_t destinationIndex,
		std::size_t length) noexcept
	{
		assert(sourceArray.data() != nullptr && "sourceArray cannot be null");
		assert(destinationArray.data() != nullptr && "destinationArray cannot be null");
		assert(sourceArray.size() >= sourceIndex + length && "Not enough source size");
		assert(destinationArray.size() >= destinationIndex + length && "Not enough target size");

		for (; length > 0; --length)
		{
			float x = sourceArray[sourceIndex].X;
			float y = sourceArray[sourceIndex].Y;

			destinationArray[destinationIndex].X = x * matrix.M11 + y * matrix.M21 + matrix.M41;
			destinationArray[destinationIndex].Y = x * matrix.M12 + y * matrix.M22 + matrix.M42;

			++sourceIndex;
			++destinationIndex;
		}
	}

	constexpr void Vector2::TransformNormal(
		std::span<const Vector2> sourceArray,
		const Matrix& matrix,
		std::span<Vector2> destinationArray) noexcept
	{
		assert(sourceArray.data() != nullptr && "sourceArray cannot be null");
		assert(destinationArray.data() != nullptr && "destinationArray cannot be null");
		assert(destinationArray.size() >= sourceArray.size() && "Not enough target size");

		for (std::size_t index = 0; index < sourceArray.size(); ++index)
		{
			float x = sourceArray[index].X;
			float y = sourceArray[index].Y;

			// Transformação de Normais ignora translação (M41, M42)
			destinationArray[index].X = (x * matrix.M11) + (y * matrix.M21);
			destinationArray[index].Y = (x * matrix.M12) + (y * matrix.M22);
		}
	}

	constexpr void Vector2::TransformNormal(
		std::span<const Vector2> sourceArray,
		std::size_t sourceIndex,
		const Matrix& matrix,
		std::span<Vector2> destinationArray,
		std::size_t destinationIndex,
		std::size_t length) noexcept
	{
		assert(sourceArray.data() != nullptr && "sourceArray cannot be null");
		assert(destinationArray.data() != nullptr && "destinationArray cannot be null");
		assert(sourceArray.size() >= sourceIndex + length && "Not enough source size");
		assert(destinationArray.size() >= destinationIndex + length && "Not enough target size");

		for (; length > 0; --length)
		{
			float x = sourceArray[sourceIndex].X;
			float y = sourceArray[sourceIndex].Y;

			// Transformação de Normais (sem translação) calculada em float puro
			destinationArray[destinationIndex].X = (x * matrix.M11) + (y * matrix.M21);
			destinationArray[destinationIndex].Y = (x * matrix.M12) + (y * matrix.M22);

			++sourceIndex;
			++destinationIndex;
		}
	}

	constexpr void Vector3::Transform(
		std::span<const Vector3> sourceArray,
		const Matrix& matrix,
		std::span<Vector3> destinationArray) noexcept
	{
		assert(sourceArray.data() != nullptr && "sourceArray cannot be null");
		assert(destinationArray.data() != nullptr && "destinationArray cannot be null");
		assert(destinationArray.size() >= sourceArray.size() && "Not enough target size");

		for (std::size_t index = 0; index < sourceArray.size(); ++index)
		{
			float x = sourceArray[index].X;
			float y = sourceArray[index].Y;
			float z = sourceArray[index].Z;

			// Transformação 3D com translação calculada diretamente em float puro
			destinationArray[index].X = (x * matrix.M11) + (y * matrix.M21) + (z * matrix.M31) + matrix.M41;
			destinationArray[index].Y = (x * matrix.M12) + (y * matrix.M22) + (z * matrix.M32) + matrix.M42;
			destinationArray[index].Z = (x * matrix.M13) + (y * matrix.M23) + (z * matrix.M33) + matrix.M43;
		}
	}

	constexpr void Vector3::Transform(
		std::span<const Vector3> sourceArray,
		std::size_t sourceIndex,
		const Matrix& matrix,
		std::span<Vector3> destinationArray,
		std::size_t destinationIndex,
		std::size_t length) noexcept
	{
		// Validações com assert e std::size_t para garantir o limite do buffer
		assert(sourceArray.data() != nullptr && "sourceArray cannot be null");
		assert(destinationArray.data() != nullptr && "destinationArray cannot be null");
		assert(sourceArray.size() >= sourceIndex + length && "Not enough source size");
		assert(destinationArray.size() >= destinationIndex + length && "Not enough target size");

		for (; length > 0; --length)
		{
			float x = sourceArray[sourceIndex].X;
			float y = sourceArray[sourceIndex].Y;
			float z = sourceArray[sourceIndex].Z;

			// Transformação 3D com translação e matemática puramente em float
			destinationArray[destinationIndex].X = (x * matrix.M11) + (y * matrix.M21) + (z * matrix.M31) + matrix.M41;
			destinationArray[destinationIndex].Y = (x * matrix.M12) + (y * matrix.M22) + (z * matrix.M32) + matrix.M42;
			destinationArray[destinationIndex].Z = (x * matrix.M13) + (y * matrix.M23) + (z * matrix.M33) + matrix.M43;

			++sourceIndex;
			++destinationIndex;
		}
	}

	constexpr void Vector3::TransformNormal(
		std::span<const Vector3> sourceArray,
		const Matrix& matrix,
		std::span<Vector3> destinationArray) noexcept
	{
		assert(sourceArray.data() != nullptr && "sourceArray cannot be null");
		assert(destinationArray.data() != nullptr && "destinationArray cannot be null");
		assert(destinationArray.size() >= sourceArray.size() && "Not enough target size");

		for (std::size_t index = 0; index < sourceArray.size(); ++index)
		{
			float x = sourceArray[index].X;
			float y = sourceArray[index].Y;
			float z = sourceArray[index].Z;

			// Transformação de normais em 3D (ignora a translação M41, M42, M43) 
			// Realizada diretamente em ponto flutuante de precisão simples (float)
			destinationArray[index].X = (x * matrix.M11) + (y * matrix.M21) + (z * matrix.M31);
			destinationArray[index].Y = (x * matrix.M12) + (y * matrix.M22) + (z * matrix.M32);
			destinationArray[index].Z = (x * matrix.M13) + (y * matrix.M23) + (z * matrix.M33);
		}
	}

	constexpr void Vector3::TransformNormal(
		std::span<const Vector3> sourceArray,
		std::size_t sourceIndex,
		const Matrix& matrix,
		std::span<Vector3> destinationArray,
		std::size_t destinationIndex,
		std::size_t length) noexcept
	{
		// Validações de nulidade e de limites utilizando asserts e std::size_t
		assert(sourceArray.data() != nullptr && "sourceArray cannot be null");
		assert(destinationArray.data() != nullptr && "destinationArray cannot be null");
		assert(sourceArray.size() >= sourceIndex + length && "Not enough source size");
		assert(destinationArray.size() >= destinationIndex + length && "Not enough target size");

		for (; length > 0; --length)
		{
			float x = sourceArray[sourceIndex].X;
			float y = sourceArray[sourceIndex].Y;
			float z = sourceArray[sourceIndex].Z;

			// Transformação de normais 3D (ignora componentes de translação)
			// Cálculos realizados diretamente em float
			destinationArray[destinationIndex].X = (x * matrix.M11) + (y * matrix.M21) + (z * matrix.M31);
			destinationArray[destinationIndex].Y = (x * matrix.M12) + (y * matrix.M22) + (z * matrix.M32);
			destinationArray[destinationIndex].Z = (x * matrix.M13) + (y * matrix.M23) + (z * matrix.M33);

			++sourceIndex;
			++destinationIndex;
		}
	}

	constexpr void Vector4::Transform(
		std::span<const Vector4> sourceArray,
		const Matrix& matrix,
		std::span<Vector4> destinationArray) noexcept
	{
		assert(sourceArray.data() != nullptr && "sourceArray cannot be null");
		assert(destinationArray.data() != nullptr && "destinationArray cannot be null");
		assert(destinationArray.size() >= sourceArray.size() && "Not enough target size");

		for (std::size_t index = 0; index < sourceArray.size(); ++index)
		{
			float x = sourceArray[index].X;
			float y = sourceArray[index].Y;
			float z = sourceArray[index].Z;
			float w = sourceArray[index].W;

			// Multiplicação de matriz 4x4 por vetor 4D realizada puramente em float
			destinationArray[index].X = (x * matrix.M11) + (y * matrix.M21) + (z * matrix.M31) + (w * matrix.M41);
			destinationArray[index].Y = (x * matrix.M12) + (y * matrix.M22) + (z * matrix.M32) + (w * matrix.M42);
			destinationArray[index].Z = (x * matrix.M13) + (y * matrix.M23) + (z * matrix.M33) + (w * matrix.M43);
			destinationArray[index].W = (x * matrix.M14) + (y * matrix.M24) + (z * matrix.M34) + (w * matrix.M44);
		}
	}

	constexpr void Vector4::Transform(
		std::span<const Vector4> sourceArray,
		std::size_t sourceIndex,
		const Matrix& matrix,
		std::span<Vector4> destinationArray,
		std::size_t destinationIndex,
		std::size_t length) noexcept
	{
		assert(sourceArray.data() != nullptr && "sourceArray cannot be null");
		assert(destinationArray.data() != nullptr && "destinationArray cannot be null");
		assert(sourceArray.size() >= sourceIndex + length && "Not enough source size");
		assert(destinationArray.size() >= destinationIndex + length && "Not enough target size");

		// Processa apenas o segmento ('length') especificado
		for (; length > 0; --length)
		{
			float x = sourceArray[sourceIndex].X;
			float y = sourceArray[sourceIndex].Y;
			float z = sourceArray[sourceIndex].Z;
			float w = sourceArray[sourceIndex].W;

			// Multiplicação de matriz 4x4 por vetor 4D realizada puramente em float
			destinationArray[destinationIndex].X = (x * matrix.M11) + (y * matrix.M21) + (z * matrix.M31) + (w * matrix.M41);
			destinationArray[destinationIndex].Y = (x * matrix.M12) + (y * matrix.M22) + (z * matrix.M32) + (w * matrix.M42);
			destinationArray[destinationIndex].Z = (x * matrix.M13) + (y * matrix.M23) + (z * matrix.M33) + (w * matrix.M43);
			destinationArray[destinationIndex].W = (x * matrix.M14) + (y * matrix.M24) + (z * matrix.M34) + (w * matrix.M44);

			++sourceIndex;
			++destinationIndex;
		}
	}

	constexpr Vector3 Vector3::Transform(Vector3 const& value, Quaternion const& rotation) noexcept
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

	constexpr Vector4 Vector4::Transform(Vector2 const& value, Quaternion const& rotation) noexcept
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

	constexpr Vector4 Vector4::Transform(Vector3 const& value, Quaternion const& rotation) noexcept {
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

	constexpr Matrix Matrix::CreateFromQuaternion(Quaternion const& quaternion) noexcept {
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

	constexpr Matrix Matrix::Transform(Matrix const& value, Quaternion const& rotation) noexcept {
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

	constexpr Matrix Matrix::CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept {
		const auto result1 = Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);
		const auto result2 = Matrix::CreateFromQuaternion(result1);
		return result2;
	}

	constexpr void Vector2::Transform(
		std::span<const Vector2> sourceArray,
		const Quaternion& rotation,
		std::span<Vector2> destinationArray) noexcept
	{
		assert(sourceArray.data() != nullptr && "sourceArray cannot be null");
		assert(destinationArray.data() != nullptr && "destinationArray cannot be null");
		assert(destinationArray.size() >= sourceArray.size() && "Not enough target size");

		// Pré-cálculo da matriz de rotação a partir do Quaternion
		float num1 = rotation.X + rotation.X;
		float num2 = rotation.Y + rotation.Y;
		float num3 = rotation.Z + rotation.Z;
		float num4 = rotation.W * num3;
		float num5 = rotation.X * num1;
		float num6 = rotation.X * num2;
		float num7 = rotation.Y * num2;
		float num8 = rotation.Z * num3;

		// Componentes equivalentes aos elementos da matriz de rotação 2D
		float num9 = 1.0f - num7 - num8;
		float num10 = num6 - num4;
		float num11 = num6 + num4;
		float num12 = 1.0f - num5 - num8;

		for (std::size_t index = 0; index < sourceArray.size(); ++index)
		{
			float x = sourceArray[index].X;
			float y = sourceArray[index].Y;

			// Aplicação da rotação nos eixos X e Y calculada diretamente em float
			destinationArray[index].X = (x * num9) + (y * num10);
			destinationArray[index].Y = (x * num11) + (y * num12);
		}
	}

	constexpr void Vector2::Transform(
		std::span<const Vector2> sourceArray,
		std::size_t sourceIndex,
		const Quaternion& rotation,
		std::span<Vector2> destinationArray,
		std::size_t destinationIndex,
		std::size_t length) noexcept
	{
		assert(sourceArray.data() != nullptr && "sourceArray cannot be null");
		assert(destinationArray.data() != nullptr && "destinationArray cannot be null");
		assert(sourceArray.size() >= sourceIndex + length && "Not enough source size");
		assert(destinationArray.size() >= destinationIndex + length && "Not enough target size");

		// Pré-cálculo da matriz de rotação a partir do Quaternion
		float num1 = rotation.X + rotation.X;
		float num2 = rotation.Y + rotation.Y;
		float num3 = rotation.Z + rotation.Z;
		float num4 = rotation.W * num3;
		float num5 = rotation.X * num1;
		float num6 = rotation.X * num2;
		float num7 = rotation.Y * num2;
		float num8 = rotation.Z * num3;

		// Componentes equivalentes aos elementos da matriz de rotação 2D
		float num9 = 1.0f - num7 - num8;
		float num10 = num6 - num4;
		float num11 = num6 + num4;
		float num12 = 1.0f - num5 - num8;

		// Processa apenas o segmento ('length') especificado
		for (; length > 0; --length)
		{
			float x = sourceArray[sourceIndex].X;
			float y = sourceArray[sourceIndex].Y;

			// Aplicação da rotação calculada diretamente em float
			destinationArray[destinationIndex].X = (x * num9) + (y * num10);
			destinationArray[destinationIndex].Y = (x * num11) + (y * num12);

			++sourceIndex;
			++destinationIndex;
		}
	}

	constexpr void Vector3::Transform(
		std::span<const Vector3> sourceArray,
		const Quaternion& rotation,
		std::span<Vector3> destinationArray) noexcept
	{
		// Validações de segurança com assert
		assert(sourceArray.data() != nullptr && "sourceArray cannot be null");
		assert(destinationArray.data() != nullptr && "destinationArray cannot be null");
		assert(destinationArray.size() >= sourceArray.size() && "Not enough target size");

		// Pré-cálculo da matriz de rotação 3x3 a partir do Quaternion
		float num1 = rotation.X + rotation.X;
		float num2 = rotation.Y + rotation.Y;
		float num3 = rotation.Z + rotation.Z;
		float num4 = rotation.W * num1;
		float num5 = rotation.W * num2;
		float num6 = rotation.W * num3;
		float num7 = rotation.X * num1;
		float num8 = rotation.X * num2;
		float num9 = rotation.X * num3;
		float num10 = rotation.Y * num2;
		float num11 = rotation.Y * num3;
		float num12 = rotation.Z * num3;

		// Componentes finais equivalentes aos elementos da matriz de rotação 3D
		float num13 = 1.0f - num10 - num12;
		float num14 = num8 - num6;
		float num15 = num9 + num5;
		float num16 = num8 + num6;
		float num17 = 1.0f - num7 - num12;
		float num18 = num11 - num4;
		float num19 = num9 - num5;
		float num20 = num11 + num4;
		float num21 = 1.0f - num7 - num10;

		for (std::size_t index = 0; index < sourceArray.size(); ++index)
		{
			float x = sourceArray[index].X;
			float y = sourceArray[index].Y;
			float z = sourceArray[index].Z;

			// Aplicação da rotação nos três eixos calculada diretamente em float
			destinationArray[index].X = (x * num13) + (y * num14) + (z * num15);
			destinationArray[index].Y = (x * num16) + (y * num17) + (z * num18);
			destinationArray[index].Z = (x * num19) + (y * num20) + (z * num21);
		}
	}

	constexpr void Vector3::Transform(
		std::span<const Vector3> sourceArray,
		std::size_t sourceIndex,
		const Quaternion& rotation,
		std::span<Vector3> destinationArray,
		std::size_t destinationIndex,
		std::size_t length) noexcept
	{
		assert(sourceArray.data() != nullptr && "sourceArray cannot be null");
		assert(destinationArray.data() != nullptr && "destinationArray cannot be null");
		assert(sourceArray.size() >= sourceIndex + length && "Not enough source size");
		assert(destinationArray.size() >= destinationIndex + length && "Not enough target size");

		// Pré-cálculo da matriz de rotação 3x3 a partir do Quaternion
		float num1 = rotation.X + rotation.X;
		float num2 = rotation.Y + rotation.Y;
		float num3 = rotation.Z + rotation.Z;
		float num4 = rotation.W * num1;
		float num5 = rotation.W * num2;
		float num6 = rotation.W * num3;
		float num7 = rotation.X * num1;
		float num8 = rotation.X * num2;
		float num9 = rotation.X * num3;
		float num10 = rotation.Y * num2;
		float num11 = rotation.Y * num3;
		float num12 = rotation.Z * num3;

		// Componentes finais equivalentes aos elementos da matriz de rotação 3D
		float num13 = 1.0f - num10 - num12;
		float num14 = num8 - num6;
		float num15 = num9 + num5;
		float num16 = num8 + num6;
		float num17 = 1.0f - num7 - num12;
		float num18 = num11 - num4;
		float num19 = num9 - num5;
		float num20 = num11 + num4;
		float num21 = 1.0f - num7 - num10;

		// Processa apenas o segmento ('length') especificado
		for (; length > 0; --length)
		{
			float x = sourceArray[sourceIndex].X;
			float y = sourceArray[sourceIndex].Y;
			float z = sourceArray[sourceIndex].Z;

			// Aplicação da rotação calculada diretamente em float puro
			destinationArray[destinationIndex].X = (x * num13) + (y * num14) + (z * num15);
			destinationArray[destinationIndex].Y = (x * num16) + (y * num17) + (z * num18);
			destinationArray[destinationIndex].Z = (x * num19) + (y * num20) + (z * num21);

			++sourceIndex;
			++destinationIndex;
		}
	}

	constexpr void Vector4::Transform(
		std::span<const Vector4> sourceArray,
		const Quaternion& rotation,
		std::span<Vector4> destinationArray) noexcept
	{
		// Validações de limites e nulidade usando assert
		assert(sourceArray.data() != nullptr && "sourceArray cannot be null");
		assert(destinationArray.data() != nullptr && "destinationArray cannot be null");
		assert(destinationArray.size() >= sourceArray.size() && "Not enough target size");

		// Pré-cálculo da matriz de rotação 3x3 a partir do Quaternion
		float num1 = rotation.X + rotation.X;
		float num2 = rotation.Y + rotation.Y;
		float num3 = rotation.Z + rotation.Z;
		float num4 = rotation.W * num1;
		float num5 = rotation.W * num2;
		float num6 = rotation.W * num3;
		float num7 = rotation.X * num1;
		float num8 = rotation.X * num2;
		float num9 = rotation.X * num3;
		float num10 = rotation.Y * num2;
		float num11 = rotation.Y * num3;
		float num12 = rotation.Z * num3;

		// Componentes finais equivalentes aos elementos da matriz de rotação 3D
		float num13 = 1.0f - num10 - num12;
		float num14 = num8 - num6;
		float num15 = num9 + num5;
		float num16 = num8 + num6;
		float num17 = 1.0f - num7 - num12;
		float num18 = num11 - num4;
		float num19 = num9 - num5;
		float num20 = num11 + num4;
		float num21 = 1.0f - num7 - num10;

		for (std::size_t index = 0; index < sourceArray.size(); ++index)
		{
			float x = sourceArray[index].X;
			float y = sourceArray[index].Y;
			float z = sourceArray[index].Z;

			// Rotação dos eixos X, Y e Z calculada puramente em float
			destinationArray[index].X = (x * num13) + (y * num14) + (z * num15);
			destinationArray[index].Y = (x * num16) + (y * num17) + (z * num18);
			destinationArray[index].Z = (x * num19) + (y * num20) + (z * num21);

			// Em uma rotação baseada em quaternions, o eixo W permanece inalterado
			destinationArray[index].W = sourceArray[index].W;
		}
	}

	constexpr void Vector4::Transform(
		std::span<const Vector4> sourceArray,
		std::size_t sourceIndex,
		const Quaternion& rotation,
		std::span<Vector4> destinationArray,
		std::size_t destinationIndex,
		std::size_t length) noexcept
	{
		// Validações de limites e nulidade usando assert e std::size_t
		assert(sourceArray.data() != nullptr && "sourceArray cannot be null");
		assert(destinationArray.data() != nullptr && "destinationArray cannot be null");
		assert(sourceArray.size() >= sourceIndex + length && "Not enough source size");
		assert(destinationArray.size() >= destinationIndex + length && "Not enough target size");

		// Pré-cálculo da matriz de rotação 3x3 a partir do Quaternion
		float num1 = rotation.X + rotation.X;
		float num2 = rotation.Y + rotation.Y;
		float num3 = rotation.Z + rotation.Z;
		float num4 = rotation.W * num1;
		float num5 = rotation.W * num2;
		float num6 = rotation.W * num3;
		float num7 = rotation.X * num1;
		float num8 = rotation.X * num2;
		float num9 = rotation.X * num3;
		float num10 = rotation.Y * num2;
		float num11 = rotation.Y * num3;
		float num12 = rotation.Z * num3;

		// Componentes finais equivalentes aos elementos da matriz de rotação 3D
		float num13 = 1.0f - num10 - num12;
		float num14 = num8 - num6;
		float num15 = num9 + num5;
		float num16 = num8 + num6;
		float num17 = 1.0f - num7 - num12;
		float num18 = num11 - num4;
		float num19 = num9 - num5;
		float num20 = num11 + num4;
		float num21 = 1.0f - num7 - num10;

		// Processa apenas o segmento ('length') especificado
		for (; length > 0; --length)
		{
			float x = sourceArray[sourceIndex].X;
			float y = sourceArray[sourceIndex].Y;
			float z = sourceArray[sourceIndex].Z;
			float w = sourceArray[sourceIndex].W;

			// Rotação dos eixos X, Y e Z calculada puramente em float
			destinationArray[destinationIndex].X = (x * num13) + (y * num14) + (z * num15);
			destinationArray[destinationIndex].Y = (x * num16) + (y * num17) + (z * num18);
			destinationArray[destinationIndex].Z = (x * num19) + (y * num20) + (z * num21);

			// Em uma rotação, o componente W permanece inalterado
			destinationArray[destinationIndex].W = w;

			++sourceIndex;
			++destinationIndex;
		}
	}
}