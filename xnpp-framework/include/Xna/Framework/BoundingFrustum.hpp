#ifndef XNA_FRAMEWORK_BOUNDINGFRUSTUM_HPP
#define XNA_FRAMEWORK_BOUNDINGFRUSTUM_HPP

#include "Plane.hpp"
#include "Matrix.hpp"
#include <optional>
#include <cstddef>

namespace Xna {
	//Defines a frustum and helps determine whether forms intersect with it. 
	//[!] To enable constexpr functionality for BoundingFrustum methods, part of the implementation is based on MonoGame, which avoids the GJK structure used in the original XNA code.
	struct BoundingFrustum final {
		struct InternalRay final {
			Vector3 Position{};
			Vector3 Direction{};
		};

		//Specifies the total number of corners (8) in the BoundingFrustum.
		static constexpr int CornerCount = 8;

		//Creates a new instance of BoundingFrustum.
		constexpr BoundingFrustum() = default;
		//Creates a new instance of BoundingFrustum.
		constexpr BoundingFrustum(Xna::Matrix const& matrix) {
			Matrix(matrix);
		}

		//Gets the near plane of the BoundingFrustum.
		constexpr Plane Near() const { return planes[0]; }
		//Gets the far plane of the BoundingFrustum.
		constexpr Plane Far() const { return planes[1]; }
		//Gets the left plane of the BoundingFrustum.
		constexpr Plane Left() const { return planes[2]; }
		//Gets the right plane of the BoundingFrustum.
		constexpr Plane Right() const { return planes[3]; }
		//Gets the top plane of the BoundingFrustum.
		constexpr Plane Top() const { return planes[4]; }
		//Gets the bottom plane of the BoundingFrustum.
		constexpr Plane Bottom() const { return planes[5]; }		

		//Gets an array of points that make up the corners of the BoundingFrustum.
		constexpr void GetCorners(std::vector<Vector3>& destination) const;
		//Gets or sets the Matrix that describes this bounding frustum.
		constexpr Xna::Matrix Matrix() const { return matrix; }
		//Gets or sets the Matrix that describes this bounding frustum.
		constexpr void Matrix(Xna::Matrix const& value) {
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

			auto intersectionLine1 = BoundingFrustum::ComputeIntersectionLine(planes[0], planes[2]);
			corners[0] = BoundingFrustum::ComputeIntersection(planes[4], intersectionLine1);
			corners[3] = BoundingFrustum::ComputeIntersection(planes[5], intersectionLine1);
			auto intersectionLine2 = BoundingFrustum::ComputeIntersectionLine(planes[3], planes[0]);
			corners[1] = BoundingFrustum::ComputeIntersection(planes[4], intersectionLine2);
			corners[2] = BoundingFrustum::ComputeIntersection(planes[5], intersectionLine2);
			intersectionLine2 = BoundingFrustum::ComputeIntersectionLine(planes[2], planes[1]);
			corners[4] = BoundingFrustum::ComputeIntersection(planes[4], intersectionLine2);
			corners[7] = BoundingFrustum::ComputeIntersection(planes[5], intersectionLine2);
			intersectionLine2 = BoundingFrustum::ComputeIntersectionLine(planes[1], planes[3]);
			corners[5] = BoundingFrustum::ComputeIntersection(planes[4], intersectionLine2);
			corners[6] = BoundingFrustum::ComputeIntersection(planes[5], intersectionLine2);
		}
		//Gets or sets the Matrix that describes this bounding frustum.
		constexpr bool Intersects(BoundingBox const& box);
		//Gets or sets the Matrix that describes this bounding frustum.
		constexpr bool Intersects(BoundingSphere const& box);
		//Gets or sets the Matrix that describes this bounding frustum.
		constexpr PlaneIntersectionType Intersects(Plane const& plane) const;
		//Gets or sets the Matrix that describes this bounding frustum.
		constexpr bool Intersects(BoundingFrustum const& frustum);
		//Gets or sets the Matrix that describes this bounding frustum.
		constexpr std::optional<float> Intersects(Ray const& ray) const;
		//Checks whether the current BoundingFrustum contains a specified bounding volume. 
		constexpr ContainmentType Contains(BoundingBox const& box) const;
		//Checks whether the current BoundingFrustum contains a specified bounding volume. 
		constexpr ContainmentType Contains(BoundingFrustum const& box);
		//Checks whether the current BoundingFrustum contains a specified bounding volume. 
		constexpr ContainmentType Contains(Vector3 const& point) const;
		//Checks whether the current BoundingFrustum contains a specified bounding volume. 
		constexpr ContainmentType Contains(BoundingSphere const& box) const;		

		constexpr bool operator==(BoundingFrustum const& other) const {
			return matrix == other.matrix;
		}

		constexpr Vector3 operator[](size_t index) const {
			if (index >= CornerCount)
				index = CornerCount - 1; //throw exception

			return corners[index];
		}

		constexpr operator std::optional<BoundingFrustum>() const {
			return std::make_optional<BoundingFrustum>(matrix);
		}

	private:
		static constexpr InternalRay ComputeIntersectionLine(Plane const& p1, Plane const& p2) {
			InternalRay intersectionLine{};
			intersectionLine.Direction = Vector3::Cross(p1.Normal, p2.Normal);
			const auto num = intersectionLine.Direction.LengthSquared();
			intersectionLine.Position = Vector3::Cross(-p1.D * p2.Normal + p2.D * p1.Normal, intersectionLine.Direction) / num;
			return intersectionLine;
		}

		static constexpr Vector3 ComputeIntersection(Plane const& plane, InternalRay const& ray) {
			const auto num = (-plane.D - Vector3::Dot(plane.Normal, ray.Position)) / Vector3::Dot(plane.Normal, ray.Direction);
			return ray.Position + ray.Direction * num;
		}

	private:		
		Vector3 corners[8];
		Plane planes[6];
		Xna::Matrix matrix{ Xna::Matrix::Identity() };
		static constexpr int PlaneCount = 6;

		friend struct BoundingSphere;
	};

	constexpr void BoundingFrustum::GetCorners(std::vector<Vector3>& destination) const {
		if (destination.size() < CornerCount)
			destination.resize(CornerCount);

		for (size_t i = 0; i < CornerCount; ++i)
			destination[i] = corners[i];
	}	

	constexpr PlaneIntersectionType BoundingFrustum::Intersects(Plane const& plane) const {
		int num = 0;
		for (size_t index = 0; index < CornerCount; ++index)
		{
			auto result = Vector3::Dot(corners[index], plane.Normal);

			if (result + plane.D > 0.0F)
				num |= 1;
			else
				num |= 2;

			if (num == 3)
				return PlaneIntersectionType::Intersecting;
		}
		return num != 1 ? PlaneIntersectionType::Back : PlaneIntersectionType::Front;
	}

	constexpr ContainmentType BoundingFrustum::Contains(Vector3 const& point) const {
		for (size_t i = 0; i < PlaneCount; ++i) {
			const auto& plane = planes[i];

			if ((static_cast<double>(plane.Normal.X * point.X + plane.Normal.Y * point.Y + plane.Normal.Z * point.Z) + plane.D) > 9.9999997473787516E-06)
				return ContainmentType::Disjoint;
		}

		return ContainmentType::Contains;
	}	

	constexpr PlaneIntersectionType Plane::Intersects(BoundingFrustum const& frustum) const {
		return frustum.Intersects(*this);
	}

	constexpr bool BoundingFrustum::Intersects(BoundingFrustum const& frustum) {
		return Contains(frustum) != ContainmentType::Disjoint;
	}

	constexpr ContainmentType BoundingFrustum::Contains(BoundingFrustum const& frustum) {
		if (*this == frustum)
			return ContainmentType::Contains;

		auto intersects = false;
		for (size_t i = 0; i < PlaneCount; ++i)
		{
			PlaneIntersectionType planeIntersectionType = frustum.Intersects(planes[i]);
			switch (planeIntersectionType)
			{
			case PlaneIntersectionType::Front:
				return ContainmentType::Disjoint;
			case PlaneIntersectionType::Intersecting:
				intersects = true;
				break;
			}
		}
		return intersects ? ContainmentType::Intersects : ContainmentType::Contains;
	}

}

#endif