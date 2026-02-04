#ifndef XNA_FRAMEWORK_GRAPHICS_VIEWPORT_HPP
#define XNA_FRAMEWORK_GRAPHICS_VIEWPORT_HPP

#include "../Rectangle.hpp"
#include "../Vector3.hpp"
#include "../Matrix.hpp"
#include <optional>

namespace Xna {
	//Defines the window dimensions of a render-target surface onto which a 3D volume projects. 
	struct Viewport final {
		//Gets or sets the pixel coordinate of the upper-left corner of the viewport on the render-target surface.
		float X{ 0 };
		//Gets or sets the pixel coordinate of the upper-left corner of the viewport on the render-target surface.
		float Y{ 0 };
		//Gets or sets the width dimension of the viewport on the render-target surface, in pixels.
		float Width{ 0 };
		//Gets or sets the height dimension of the viewport on the render-target surface, in pixels.
		float Height{ 0 };
		//Gets or sets the minimum depth of the clip volume.
		float MinDepth{ 0 };
		//Gets or sets the maximum depth of the clip volume.
		float MaxDepth{ 1.0F };

		//Initializes a new instance of Viewport.
		constexpr Viewport() = default;

		//Initializes a new instance of Viewport.
		constexpr Viewport(float X, float Y, float Width, float Height, float MinDepth, float MaxDepth)
			: X(X), Y(Y), Width(Width), Height(Height), MinDepth(MinDepth), MaxDepth(MaxDepth) {
		}

		//Initializes a new instance of Viewport.
		constexpr Viewport(Rectangle const& bounds) 
			: X(static_cast<float>(bounds.X)), Y(static_cast<float>(bounds.Y)), Width(static_cast<float>(bounds.Width)), Height(static_cast<float>(bounds.Height)) {
		}

		constexpr bool operator==(const Viewport& other) const {
			return X == other.X
				&& Y == other.Y
				&& Width == other.Width
				&& Height == other.Height
				&& MinDepth == other.MinDepth
				&& MaxDepth == other.MaxDepth;
		}

		constexpr operator std::optional<Viewport>() const {
			return std::make_optional<Viewport>(*this);
		}

		//Gets or sets the size of this resource.
		constexpr Rectangle Bounds() const {
			return {
				static_cast<int32_t>(X),
				static_cast<int32_t>(Y),
				static_cast<int32_t>(Width),
				static_cast<int32_t>(Height),
			};
		}

		//Gets or sets the size of this resource.
		constexpr void Bounds(Rectangle const& value) {
			X = static_cast<float>(value.X);
			Y = static_cast<float>(value.Y);
			Width = static_cast<float>(value.Width);
			Height = static_cast<float>(value.Height);
		}

		//Projects a 3D vector from object space into screen space.
		constexpr Vector3 Project(Vector3 const& source, Matrix const& projection, Matrix const& view, Matrix const& world) const {
			const auto matrix = Matrix::Multiply(Matrix::Multiply(world, view), projection);
			auto vector3 = Vector3::Transform(source, matrix);

			const auto a = (source.X * matrix.M14 + source.Y * matrix.M24 + source.Z * matrix.M34) + matrix.M44;
			
			if (!Viewport::WithinEpsilon(a, 1.0f))
				vector3 = vector3 / a;

			vector3.X = ((vector3.X + 1.0f) * 0.5f) * Width + X;
			vector3.Y = ((-vector3.Y + 1.0f) * 0.5f) * Height + Y;
			vector3.Z = vector3.Z * (MaxDepth - MinDepth) + MinDepth;
			return vector3;
		}

		//Converts a screen space point into a corresponding point in world space.
		constexpr Vector3 Unproject(Vector3 source, Matrix projection, Matrix view, Matrix world) const {
			const auto matrix = Matrix::Invert(Matrix::Multiply(Matrix::Multiply(world, view), projection));
			source.X = ((source.X - X) / Width * 2.0f - 1.0f);
			source.Y = -((source.Y - Y) / Height * 2.0f - 1.0f);
			source.Z = ((source.Z - MinDepth) / (MaxDepth - MinDepth));
			auto vector3 = Vector3::Transform(source, matrix);
			
			const auto a = (source.X * matrix.M14 + source.Y * matrix.M24 + source.Z * matrix.M34) + matrix.M44;
			
			if (!Viewport::WithinEpsilon(a, 1.0f))
				vector3 = vector3 / a;

			return vector3;
		}

	private:
		static constexpr bool WithinEpsilon(float a, float b) {
			const auto num = a - b;
			return -1.4012984643248171E-45 <= static_cast<double>(num) 
				&& static_cast<double>(num) <= 1.4012984643248171E-45;
		}
	};
}

#endif