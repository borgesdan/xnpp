#ifndef XNA_FRAMEWORK_RECTANGLE_HPP
#define XNA_FRAMEWORK_RECTANGLE_HPP

#include "Point.hpp"

namespace Xna {
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
		constexpr Rectangle(const int32_t& X, const int32_t& Y, const int32_t& Width, const int32_t& Height) :
			X(X), Y(Y), Width(Width), Height(Height) {
		}

		constexpr bool operator==(const Rectangle& other) const {
			return Height == other.Height && Width == other.Width && X == other.X && Y == other.Y;
		}

		constexpr operator std::optional<Rectangle>() const {
			return std::make_optional<Rectangle>(X, Y, Width, Height);
		}

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
}

#endif