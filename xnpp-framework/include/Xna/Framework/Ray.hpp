#ifndef XNA_FRAMEWORK_RAY_HPP
#define XNA_FRAMEWORK_RAY_HPP

#include "BoundingSphere.hpp"
#include <optional>
#include "MathHelper.hpp"

namespace Xna {
	//Defines a ray.
	struct Ray final {
		//Specifies the starting point of the Ray.
		Vector3 Position{};
		//Unit vector specifying the direction the Ray is pointing.
		Vector3 Direction{};
		//Creates a new instance of Ray.
		constexpr Ray() = default;
		//Creates a new instance of Ray.
		constexpr Ray(Vector3 const& position, Vector3 const& direction) :
			Position(position), Direction(direction) {
		}

		constexpr bool operator==(Ray const& other) const {
			return Position == other.Position && Direction == other.Direction;
		}

		constexpr operator std::optional<Ray>() const {
			return std::make_optional<Ray>(Position, Direction);
		}

		//Checks whether the Ray intersects a specified plane or bounding volume. 
		constexpr std::optional<float> Intersects(BoundingBox const& box) const {
			return box.Intersects(*this);
		}

		//Checks whether the Ray intersects a specified plane or bounding volume. 
		constexpr std::optional<float> Intersects(BoundingFrustum const& frustum) const {
			return frustum.Intersects(*this);
		}

		//Checks whether the Ray intersects a specified plane or bounding volume. 
		constexpr std::optional<float> Intersects(Plane const& plane) const {
			return plane.Intersects(*this);
		}

		//Checks whether the Ray intersects a specified plane or bounding volume. 
		constexpr std::optional<float> Intersects(BoundingSphere const& sphere) const {
			return sphere.Intersects(*this);
		}
	};

	constexpr Ray BoundingFrustum::ComputeIntersectionLine(Plane const& p1, Plane const& p2) {
		Ray intersectionLine{};
		intersectionLine.Direction = Vector3::Cross(p1.Normal, p2.Normal);
		const auto num = intersectionLine.Direction.LengthSquared();
		intersectionLine.Position = Vector3::Cross(-p1.D * p2.Normal + p2.D * p1.Normal, intersectionLine.Direction) / num;
		return intersectionLine;
	}

	constexpr Vector3 BoundingFrustum::ComputeIntersection(Plane const& plane, Ray const& ray) {
		const auto num = (-plane.D - Vector3::Dot(plane.Normal, ray.Position)) / Vector3::Dot(plane.Normal, ray.Direction);
		return ray.Position + ray.Direction * num;
	}

	constexpr void BoundingFrustum::Matrix(Xna::Matrix const& value) {
		matrix = value;
		planes[2].Normal.X = -value.M14 - value.M11;
		planes[2].Normal.Y = -value.M24 - value.M21;
		planes[2].Normal.Z = -value.M34 - value.M31;
		planes[2].D = -value.M44 - value.M41;
		planes[3].Normal.X = -value.M14 + value.M11;
		planes[3].Normal.Y = -value.M24 + value.M21;
		planes[3].Normal.Z = -value.M34 + value.M31;
		planes[3].D = -value.M44 + value.M41;
		planes[4].Normal.X = -value.M14 + value.M12;
		planes[4].Normal.Y = -value.M24 + value.M22;
		planes[4].Normal.Z = -value.M34 + value.M32;
		planes[4].D = -value.M44 + value.M42;
		planes[5].Normal.X = -value.M14 - value.M12;
		planes[5].Normal.Y = -value.M24 - value.M22;
		planes[5].Normal.Z = -value.M34 - value.M32;
		planes[5].D = -value.M44 - value.M42;
		planes[0].Normal.X = -value.M13;
		planes[0].Normal.Y = -value.M23;
		planes[0].Normal.Z = -value.M33;
		planes[0].D = -value.M43;
		planes[1].Normal.X = -value.M14 + value.M13;
		planes[1].Normal.Y = -value.M24 + value.M23;
		planes[1].Normal.Z = -value.M34 + value.M33;
		planes[1].D = -value.M44 + value.M43;

		for (size_t index = 0; index < PlaneCount; ++index)
		{
			const auto num = planes[index].Normal.Length();
			planes[index].Normal = planes[index].Normal / num;
			planes[index].D /= num;
		}

		Ray intersectionLine1 = BoundingFrustum::ComputeIntersectionLine(planes[0], planes[2]);
		corners[0] = BoundingFrustum::ComputeIntersection(planes[4], intersectionLine1);
		corners[3] = BoundingFrustum::ComputeIntersection(planes[5], intersectionLine1);
		Ray intersectionLine2 = BoundingFrustum::ComputeIntersectionLine(planes[3], planes[0]);
		corners[1] = BoundingFrustum::ComputeIntersection(planes[4], intersectionLine2);
		corners[2] = BoundingFrustum::ComputeIntersection(planes[5], intersectionLine2);
		intersectionLine2 = BoundingFrustum::ComputeIntersectionLine(planes[2], planes[1]);
		corners[4] = BoundingFrustum::ComputeIntersection(planes[4], intersectionLine2);
		corners[7] = BoundingFrustum::ComputeIntersection(planes[5], intersectionLine2);
		intersectionLine2 = BoundingFrustum::ComputeIntersectionLine(planes[1], planes[3]);
		corners[5] = BoundingFrustum::ComputeIntersection(planes[4], intersectionLine2);
		corners[6] = BoundingFrustum::ComputeIntersection(planes[5], intersectionLine2);
	}

	constexpr std::optional<float> BoundingFrustum::Intersects(Ray const& ray) const {
		const auto ctype = Contains(ray.Position);

		switch (ctype)
		{
		case ContainmentType::Disjoint:
			return {};
		case ContainmentType::Contains:
			return 0.0f;
		case ContainmentType::Intersects:
			return {}; //throw new NotImplementedException();
		default:
			return {}; //throw new ArgumentOutOfRangeException();
		}
	}

	constexpr std::optional<float> BoundingSphere::Intersects(Ray const& ray) const {
		const auto num1 = Center.X - ray.Position.X;
		const auto num2 = Center.Y - ray.Position.Y;
		const auto num3 = Center.Z - ray.Position.Z;
		const auto num4 = (num1 * num1 + num2 * num2 + num3 * num3);
		const auto num5 = Radius * Radius;

		if (num4 <= num5)
			return 0.0f;

		const auto num6 = (num1 * ray.Direction.X + num2 * ray.Direction.Y + num3 * ray.Direction.Z);

		if (num6 < 0.0)
			return {};

		const auto num7 = num4 - num6 * num6;

		if (num7 > num5)
			return {};

		const auto num8 = MathHelper::Extensions::Sqrt(num5 - num7);

		return num6 - num8;
	}

	constexpr std::optional<float> BoundingBox::Intersects(Ray const& ray) const {
		float num1 = 0.0f;
		float num2 = FLOAT_MAX_VALUE;
		
		if (MathHelper::Extensions::Abs(ray.Direction.X) < 9.9999999747524271E-07)
		{
			if (ray.Position.X < Min.X || ray.Position.X > Max.X)
				return {};
		}
		else
		{
			const auto num3 = 1.0f / ray.Direction.X;
			auto num4 = (Min.X - ray.Position.X) * num3;
			auto num5 = (Max.X - ray.Position.X) * num3;
			if (num4 > num5)
			{
				float num6 = num4;
				num4 = num5;
				num5 = num6;
			}
			num1 = MathHelper::Max(num4, num1);
			num2 = MathHelper::Min(num5, num2);
			if (num1 > num2)
				return {};
		}
		if (MathHelper::Extensions::Abs(ray.Direction.Y) < 9.9999999747524271E-07)
		{
			if (ray.Position.Y < Min.Y || ray.Position.Y > Max.Y)
				return {};
		}
		else
		{
			const auto num7 = 1.0f / ray.Direction.Y;
			auto num8 = (Min.Y - ray.Position.Y) * num7;
			auto num9 = (Max.Y - ray.Position.Y) * num7;
			if (num8 > num9)
			{
				float num10 = num8;
				num8 = num9;
				num9 = num10;
			}
			num1 = MathHelper::Max(num8, num1);
			num2 = MathHelper::Min(num9, num2);
			if (num1 > num2)
				return {};
		}
		if (MathHelper::Extensions::Abs(ray.Direction.Z) < 9.9999999747524271E-07)
		{
			if (ray.Position.Z < Min.Z || ray.Position.Z > Max.Z)
				return {};
		}
		else
		{
			const auto num11 = 1.0f / ray.Direction.Z;
			auto num12 = (Min.Z - ray.Position.Z) * num11;
			auto num13 = (Max.Z - ray.Position.Z) * num11;
			if (num12 > num13)
			{
				float num14 = num12;
				num12 = num13;
				num13 = num14;
			}
			num1 = MathHelper::Max(num12, num1);
			const auto num15 = MathHelper::Min(num13, num2);
			if (num1 > num15)
				return {};
		}
		return num1;
	}

	//Checks whether a Plane intersects a bounding volume.
	constexpr std::optional<float> Plane::Intersects(Ray const& ray) const {
		const auto num1 = (Normal.X * ray.Direction.X + Normal.Y * ray.Direction.Y + Normal.Z * ray.Direction.Z);

		if (MathHelper::Extensions::Abs(num1) < 9.9999997473787516E-06)
			return {};

		const auto num2 = (Normal.X * ray.Position.X + Normal.Y * ray.Position.Y + Normal.Z * ray.Position.Z);
		auto num3 = (-D - num2) / num1;

		if (num3 < 0.0)
		{
			if (num3 < -9.9999997473787516E-06)
				return {};
			num3 = 0.0f;
		}

		return num3;
	}
}

#endif