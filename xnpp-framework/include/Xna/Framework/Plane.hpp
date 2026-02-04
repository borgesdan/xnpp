#ifndef XNA_FRAMEWORK_PLANE_HPP
#define XNA_FRAMEWORK_PLANE_HPP

#include "Shared.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Matrix.hpp"
#include "Quaternion.hpp"
#include <optional>
#include "Xna/Framework/MathHelper.hpp"

namespace Xna {
	struct Plane;
	struct BoundingFrustum;
	struct BoundingBox;
	struct BoundingSphere;
	struct Ray;

	//Defines a plane. 
	struct Plane final {
		//The normal vector of the Plane.
		Vector3 Normal{ 0 };
		//The distance of the Plane along its normal from the origin. 
		float D{ 0 };

		//Creates an instance of Plane.
		constexpr Plane() = default;

		//Creates an instance of Plane.
		constexpr Plane(float a, float b, float c, float d) :
			Normal({ a,b,c }), D(d) {
		}

		//Creates an instance of Plane.
		constexpr Plane(Vector3 const& normal, float d) :
			Normal(normal), D(d) {
		}

		//Creates an instance of Plane.
		constexpr Plane(Vector4 const& value) :
			Normal({ value.X, value.Y, value.Z }), D(value.W) {
		}

		//Creates an instance of Plane.
		constexpr Plane(Vector3 const& point1, Vector3 const& point2, Vector3 const& point3) {
			const auto num1 = point2.X - point1.X;
			const auto num2 = point2.Y - point1.Y;
			const auto num3 = point2.Z - point1.Z;
			const auto num4 = point3.X - point1.X;
			const auto num5 = point3.Y - point1.Y;
			const auto num6 = point3.Z - point1.Z;
			const auto num7 = (num2 * num6 - num3 * num5);
			const auto num8 = (num3 * num4 - num1 * num6);
			const auto num9 = (num1 * num5 - num2 * num4);
			const auto num10 = 1.0f / MathHelper::Extensions::Sqrt(num7 * num7 + num8 * num8 + num9 * num9);
			Normal.X = num7 * num10;
			Normal.Y = num8 * num10;
			Normal.Z = num9 * num10;
			D = -(Normal.X * point1.X + Normal.Y * point1.Y + Normal.Z * point1.Z);
		}

		//Changes the coefficients of the Normal vector of a Plane to make it of unit length. 
		constexpr void Normalize() {
			const auto d = (Normal.X * Normal.X + Normal.Y * Normal.Y + Normal.Z * Normal.Z);

			if (MathHelper::Extensions::Abs(d - 1.0f) < 1.1920928955078125E-07)
				return;

			const auto num = 1.0f / MathHelper::Extensions::Sqrt(d);

			Normal.X *= num;
			Normal.Y *= num;
			Normal.Z *= num;
			D *= num;
		}

		//Changes the coefficients of the Normal vector of a Plane to make it of unit length. 
		static constexpr Plane Normalize(Plane const& value) {
			auto p = value;
			p.Normalize();
			return p;
		}

		//Transforms a normalized Plane by a Matrix or Quaternion.
		static constexpr Plane Transform(Plane const& plane, Matrix const& matrix);
		//Transforms a normalized Plane by a Matrix or Quaternion.
		static constexpr Plane Transform(Plane const& plane, Quaternion const& rotation);

		//Calculates the dot product of a specified Vector4 and this Plane.
		constexpr float Dot(Vector4 const& value) const;
		//Returns the dot product of a specified Vector3 and the Normal vector of this Plane plus the D constant value of the Plane.
		constexpr float DotCoordinate(Vector3 const& value) const;
		//Returns the dot product of a specified Vector3 and the Normal vector of this Plane. 
		constexpr float DotNormal(Vector3 const& value) const;

		//Checks whether a Plane intersects a bounding volume.
		constexpr PlaneIntersectionType Intersects(BoundingBox const& box) const;
		//Checks whether a Plane intersects a bounding volume.
		constexpr PlaneIntersectionType Intersects(BoundingFrustum const& frustum) const;
		//Checks whether a Plane intersects a bounding volume.
		constexpr PlaneIntersectionType Intersects(BoundingSphere const& sphere) const;
		//Checks whether a Plane intersects a bounding volume.
		constexpr std::optional<float> Intersects(Ray const& ray) const;

		constexpr bool operator==(Plane const& other) const {
			return Normal == other.Normal && D == other.D;
		}

		constexpr operator std::optional<Plane>() const {
			return std::make_optional<Plane>(Normal, D);
		}
	};


	constexpr Plane Plane::Transform(Plane const& plane, Matrix const& matrix) {
		Matrix result = Matrix::Invert(matrix);
		const auto x = plane.Normal.X;
		const auto y = plane.Normal.Y;
		const auto z = plane.Normal.Z;
		const auto d = plane.D;
		Plane plane1;
		plane1.Normal.X = (x * result.M11 + y * result.M12 + z * result.M13 + d * result.M14);
		plane1.Normal.Y = (x * result.M21 + y * result.M22 + z * result.M23 + d * result.M24);
		plane1.Normal.Z = (x * result.M31 + y * result.M32 + z * result.M33 + d * result.M34);
		plane1.D = (x * result.M41 + y * result.M42 + z * result.M43 + d * result.M44);
		return plane1;
	}

	constexpr Plane Plane::Transform(Plane const& plane, Quaternion const& rotation) {
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
		const auto x = plane.Normal.X;
		const auto y = plane.Normal.Y;
		const auto z = plane.Normal.Z;
		Plane plane1;
		plane1.Normal.X = (x * num13 + y * num14 + z * num15);
		plane1.Normal.Y = (x * num16 + y * num17 + z * num18);
		plane1.Normal.Z = (x * num19 + y * num20 + z * num21);
		plane1.D = plane.D;
		return plane1;
	}

	constexpr float Plane::Dot(Vector4 const& value) const {
		return (Normal.X * value.X + Normal.Y * value.Y + Normal.Z * value.Z + D * value.W);
	}

	constexpr float Plane::DotCoordinate(Vector3 const& value) const {
		return (Normal.X * value.X + Normal.Y * value.Y + Normal.Z * value.Z) + D;
	}

	constexpr float Plane::DotNormal(Vector3 const& value) const {
		return (Normal.X * value.X + Normal.Y * value.Y + Normal.Z * value.Z);
	}

	constexpr Matrix Matrix::CreateShadow(Vector3 const& lightDirection, Plane const& plane) {
		const auto result = Plane::Normalize(plane);
		const auto num1 = (result.Normal.X * lightDirection.X + result.Normal.Y * lightDirection.Y + result.Normal.Z * lightDirection.Z);
		const auto num2 = -result.Normal.X;
		const auto num3 = -result.Normal.Y;
		const auto num4 = -result.Normal.Z;
		const auto num5 = -result.D;
		Matrix shadow;
		shadow.M11 = num2 * lightDirection.X + num1;
		shadow.M21 = num3 * lightDirection.X;
		shadow.M31 = num4 * lightDirection.X;
		shadow.M41 = num5 * lightDirection.X;
		shadow.M12 = num2 * lightDirection.Y;
		shadow.M22 = num3 * lightDirection.Y + num1;
		shadow.M32 = num4 * lightDirection.Y;
		shadow.M42 = num5 * lightDirection.Y;
		shadow.M13 = num2 * lightDirection.Z;
		shadow.M23 = num3 * lightDirection.Z;
		shadow.M33 = num4 * lightDirection.Z + num1;
		shadow.M43 = num5 * lightDirection.Z;
		shadow.M14 = 0.0f;
		shadow.M24 = 0.0f;
		shadow.M34 = 0.0f;
		shadow.M44 = num1;
		return shadow;
	}

	constexpr Matrix Matrix::CreateReflection(Plane value) {		
		value.Normalize();
		const auto x = value.Normal.X;
		const auto y = value.Normal.Y;
		const auto z = value.Normal.Z;
		const auto num1 = -2.0f * x;
		const auto num2 = -2.0f * y;
		const auto num3 = -2.0f * z;
		Matrix reflection;
		reflection.M11 = (num1 * x + 1.0f);
		reflection.M12 = num2 * x;
		reflection.M13 = num3 * x;
		reflection.M14 = 0.0f;
		reflection.M21 = num1 * y;
		reflection.M22 = (num2 * y + 1.0f);
		reflection.M23 = num3 * y;
		reflection.M24 = 0.0f;
		reflection.M31 = num1 * z;
		reflection.M32 = num2 * z;
		reflection.M33 = (num3 * z + 1.0f);
		reflection.M34 = 0.0f;
		reflection.M41 = num1 * value.D;
		reflection.M42 = num2 * value.D;
		reflection.M43 = num3 * value.D;
		reflection.M44 = 1.0f;
		return reflection;
	}
}

#endif