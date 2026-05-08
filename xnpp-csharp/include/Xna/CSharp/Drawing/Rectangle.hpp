#ifndef CSHARP_DRAWING_RECTANGLE_HPP
#define CSHARP_DRAWING_RECTANGLE_HPP

#include "Point.hpp"
#include "Size.hpp"
#include <algorithm>

namespace Xna::CSharp {
	struct Rectangle {
		constexpr Rectangle() = default;

		constexpr Rectangle(Point const& point, Size const& size)
			: X(point.X), Y(point.Y), Width(size.Width), Height(size.Height) {
		}

		constexpr Rectangle(int32_t x, int32_t y, int32_t width, int32_t height)
			: X(x), Y(y), Width(width), Height(height) {
		}

		static constexpr Rectangle FromLTRB(int32_t left, int32_t top, int32_t right, int32_t bottom) {
			return Rectangle(left, top, (right - left), (bottom - top));
		}

		constexpr Point Location() const {
			return Point(X, Y);
		}

		constexpr void Location(Point const& value) {
			X = value.X;
			Y = value.Y;
		}

		constexpr CSharp::Size Size() const {
			return CSharp::Size(Width, Height);
		}

		constexpr void Size(Point const& value) {
			Width = value.X;
			Height = value.Y;
		}

		constexpr bool operator==(Rectangle const& other) const {
			return X == other.X
				&& Y == other.Y
				&& Width == other.Width
				&& Height == other.Height;
		}

		constexpr int32_t Left() const { return X; }
		constexpr int32_t Top() const { return Y; }
		constexpr int32_t Right() const { return X + Width; }
		constexpr int32_t Bottom() const { return Y + Height; }

		constexpr bool IsEmpty() const { return X == 0 && Y == 00 && Height == 0 && Width == 0; }

		constexpr bool Contains(int32_t x, int32_t y) const {
			return X <= x && x < X + Width && Y <= y && y < Y + Height;
		}

		constexpr bool Contains(Point const& pt) const {
			return Contains(pt.X, pt.Y);
		}

		constexpr void Inflate(int32_t width, int32_t height) {
			X -= width;
			Y -= height;

			Width += 2 * width;
			Height += 2 * height;
		}

		constexpr void Inflate(CSharp::Size const& size) {
			Inflate(size.Width, size.Height);
		}

		static constexpr Rectangle Inflate(Rectangle const& rect, int32_t x, int32_t y) {
			auto r = rect;
			r.Inflate(x, y);
			return r;
		}

		constexpr void Intersect(Rectangle const& rect) {
			auto result = Intersect(rect, *this);

			X = result.X;
			Y = result.Y;
			Width = result.Width;
			Height = result.Height;
		}

		constexpr static Rectangle Intersect(Rectangle const& a, Rectangle const& b) {
			const auto x1 = (std::max)(a.X, b.X);
			const auto x2 = (std::min)(a.X + a.Width, b.X + b.Width);
			const auto y1 = (std::max)(a.Y, b.Y);
			const auto y2 = (std::min)(a.Y + a.Height, b.Y + b.Height);

			if (x2 >= x1 && y2 >= y1)
			{
				return Rectangle(x1, y1, x2 - x1, y2 - y1);
			}

			return {};
		}

		constexpr bool IntersectsWith(Rectangle const& rect) const {
			return (rect.X < X + Width) && (X < rect.X + rect.Width) &&
				(rect.Y < Y + Height) && (Y < rect.Y + rect.Height);
		}

		constexpr static Rectangle Union(Rectangle const& a, Rectangle const& b) {
			const auto x1 = (std::min)(a.X, b.X);
			const auto x2 = (std::max)(a.X + a.Width, b.X + b.Width);
			const auto y1 = (std::min)(a.Y, b.Y);
			const auto y2 = (std::max)(a.Y + a.Height, b.Y + b.Height);

			return Rectangle(x1, y1, x2 - x1, y2 - y1);
		}

		constexpr void Offset(Point const& pos) {
			Offset(pos.X, pos.Y);
		}

		constexpr void Offset(int32_t x, int32_t y) {
			X = x;
			Y = y;
		}
	
		int32_t X{ 0 };
		int32_t Y{ 0 };
		int32_t Width{ 0 };
		int32_t Height{ 0 };
	};
}

#endif