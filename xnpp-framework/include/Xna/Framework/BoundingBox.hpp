#ifndef XNA_FRAMEWORK_BOUNDINGBOX_HPP
#define XNA_FRAMEWORK_BOUNDINGBOX_HPP

#include "BoundingFrustum.hpp"

namespace Xna {
	//Defines an axis-aligned box-shaped 3D volume. 
	struct BoundingBox final {
		//Specifies the total number of corners (8) in the BoundingBox.
		inline static constexpr int CornerCount = 8;

		//The maximum point the BoundingBox contains.
		Vector3 Min{};
		//The minimum point the BoundingBox contains. 
		Vector3 Max{};

		//Creates a new instance of BoundingBox.
		constexpr BoundingBox() = default;
		//Creates a new instance of BoundingBox.
		constexpr BoundingBox(Vector3 const& min, Vector3 const& max) :
			Min(min), Max(max) {
		}

		constexpr bool operator==(BoundingBox const& other) const {
			return Min == other.Min && Max == other.Max;
		}

		constexpr operator std::optional<BoundingBox>() const {
			return std::make_optional<BoundingBox>(Min, Max);
		}

		//Gets an array of points that make up the corners of the BoundingBox.
		constexpr void GetCorners(std::vector<Vector3>& corners) const;

		//Creates the smallest BoundingBox that contains the two specified BoundingBox instances. 
		static constexpr BoundingBox CreateMerged(BoundingBox const& original, BoundingBox const& additional);
		//Creates the smallest BoundingBox that will contain the specified BoundingSphere. 
		static constexpr BoundingBox CreateFromSphere(BoundingSphere const& sphere);
		//Creates the smallest BoundingBox that will contain a group of points.
		static constexpr BoundingBox CreateFromPoints(std::vector<Vector3> const& points) {
			return CreateFromPoints(points.data(), points.size());
		}
		//Creates the smallest BoundingBox that will contain a group of points.
		static constexpr BoundingBox CreateFromPoints(Vector3 const* points, size_t size);

		//Checks whether the current BoundingBox intersects with another bounding volume.
		constexpr bool Intersects(BoundingBox const& box) const;
		//Checks whether the current BoundingBox intersects with another bounding volume.
		bool Intersects(BoundingFrustum& frustum) const;
		//Checks whether the current BoundingBox intersects with another bounding volume.
		constexpr PlaneIntersectionType Intersects(Plane const& plane) const;
		//Checks whether the current BoundingBox intersects with another bounding volume.
		constexpr std::optional<float> Intersects(Ray const& ray) const;
		//Checks whether the current BoundingBox intersects with another bounding volume.
		constexpr bool Intersects(BoundingSphere const& sphere) const;

		//Tests whether the BoundingBox overlaps another bounding volume.
		constexpr ContainmentType Contains(BoundingBox const& box) const;
		//Tests whether the BoundingBox overlaps another bounding volume.
		constexpr ContainmentType Contains(BoundingFrustum& frustum) const;
		//Tests whether the BoundingBox overlaps another bounding volume.
		constexpr ContainmentType Contains(Vector3 const& point) const;
		//Tests whether the BoundingBox overlaps another bounding volume.
		constexpr ContainmentType Contains(BoundingSphere const& sphere) const;

	private:
		constexpr void SupportMapping(Vector3 const& v, Vector3& result) const;

	private:
		static constexpr float FLOAT_MIN_VALUE = (std::numeric_limits<float>::min)();
		static constexpr float FLOAT_MAX_VALUE = (std::numeric_limits<float>::max)();
	};

	constexpr ContainmentType BoundingFrustum::Contains(BoundingBox const& box) const {
		bool flag = false;
		for (size_t i = 0; i < PlaneCount; ++i)
		{
			const auto& plane = planes[i];

			switch (box.Intersects(plane))
			{
			case PlaneIntersectionType::Front:
				return ContainmentType::Disjoint;
			case PlaneIntersectionType::Intersecting:
				flag = true;
				break;
			}
		}
		return !flag ? ContainmentType::Contains : ContainmentType::Intersects;
	}	

	constexpr void BoundingBox::GetCorners(std::vector<Vector3>& corners) const {
		if (corners.size() < 8)
			corners.resize(CornerCount);

		corners[0].X = Min.X;
		corners[0].Y = Max.Y;
		corners[0].Z = Max.Z;
		corners[1].X = Max.X;
		corners[1].Y = Max.Y;
		corners[1].Z = Max.Z;
		corners[2].X = Max.X;
		corners[2].Y = Min.Y;
		corners[2].Z = Max.Z;
		corners[3].X = Min.X;
		corners[3].Y = Min.Y;
		corners[3].Z = Max.Z;
		corners[4].X = Min.X;
		corners[4].Y = Max.Y;
		corners[4].Z = Min.Z;
		corners[5].X = Max.X;
		corners[5].Y = Max.Y;
		corners[5].Z = Min.Z;
		corners[6].X = Max.X;
		corners[6].Y = Min.Y;
		corners[6].Z = Min.Z;
		corners[7].X = Min.X;
		corners[7].Y = Min.Y;
		corners[7].Z = Min.Z;
	}

	constexpr BoundingBox BoundingBox::CreateMerged(BoundingBox const& original, BoundingBox const& additional) {
		BoundingBox merged;
		merged.Min = Vector3::Min(original.Min, additional.Min);
		merged.Max = Vector3::Max(original.Max, additional.Max);
		return merged;
	}

	constexpr BoundingBox BoundingBox::CreateFromPoints(Vector3 const* points, size_t size) {
		Vector3 result1 = Vector3(FLOAT_MAX_VALUE);
		Vector3 result2 = Vector3(FLOAT_MIN_VALUE);

		for (size_t i = 0; i < size; ++i) {
			const auto& point = points[i];
			result1 = Vector3::Min(result1, point);
			result2 = Vector3::Max(result2, point);
		}

		return BoundingBox(result1, result2);
	}

	constexpr bool BoundingBox::Intersects(BoundingBox const& box) const {
		return Max.X >= box.Min.X
			&& Min.X <= box.Max.X
			&& Max.Y >= box.Min.Y
			&& Min.Y <= box.Max.Y
			&& Max.Z >= box.Min.Z
			&& Min.Z <= box.Max.Z;
	}

	inline bool BoundingBox::Intersects(BoundingFrustum& frustum) const {
		return frustum.Intersects(*this);
	}

	constexpr PlaneIntersectionType BoundingBox::Intersects(Plane const& plane) const {
		Vector3 vector3_1;
		vector3_1.X = plane.Normal.X >= 0.0 ? Min.X : Max.X;
		vector3_1.Y = plane.Normal.Y >= 0.0 ? Min.Y : Max.Y;
		vector3_1.Z = plane.Normal.Z >= 0.0 ? Min.Z : Max.Z;
		Vector3 vector3_2;
		vector3_2.X = plane.Normal.X >= 0.0 ? Max.X : Min.X;
		vector3_2.Y = plane.Normal.Y >= 0.0 ? Max.Y : Min.Y;
		vector3_2.Z = plane.Normal.Z >= 0.0 ? Max.Z : Min.Z;
		if (plane.Normal.X * vector3_1.X + plane.Normal.Y * vector3_1.Y + plane.Normal.Z * vector3_1.Z + plane.D > 0.0)
			return PlaneIntersectionType::Front;

		return plane.Normal.X * vector3_2.X + plane.Normal.Y * vector3_2.Y + plane.Normal.Z * vector3_2.Z + plane.D < 0.0
			? PlaneIntersectionType::Back
			: PlaneIntersectionType::Intersecting;
	}

	constexpr ContainmentType BoundingBox::Contains(BoundingBox const& box) const {
		if (Max.X < box.Min.X || Min.X > box.Max.X || Max.Y < box.Min.Y || Min.Y > box.Max.Y || Max.Z < box.Min.Z || Min.Z > box.Max.Z)
			return ContainmentType::Disjoint;
		return Min.X > box.Min.X || box.Max.X > Max.X || Min.Y > box.Min.Y || box.Max.Y > Max.Y || Min.Z > box.Min.Z || box.Max.Z > Max.Z
			? ContainmentType::Intersects
			: ContainmentType::Contains;
	}

	constexpr ContainmentType BoundingBox::Contains(Vector3 const& point) const {
		return Min.X > point.X || point.X > Max.X || Min.Y > point.Y || point.Y > Max.Y || Min.Z > point.Z || point.Z > Max.Z
			? ContainmentType::Disjoint
			: ContainmentType::Contains;
	}

	constexpr void BoundingBox::SupportMapping(Vector3 const& v, Vector3& result) const {
		result.X = v.X >= 0.0 ? Max.X : Min.X;
		result.Y = v.Y >= 0.0 ? Max.Y : Min.Y;
		result.Z = v.Z >= 0.0 ? Max.Z : Min.Z;
	}

	constexpr PlaneIntersectionType Plane::Intersects(BoundingBox const& box) const {
		Vector3 vector3_1;
		vector3_1.X = Normal.X >= 0.0f ? box.Min.X : box.Max.X;
		vector3_1.Y = Normal.Y >= 0.0f ? box.Min.Y : box.Max.Y;
		vector3_1.Z = Normal.Z >= 0.0f ? box.Min.Z : box.Max.Z;
		Vector3 vector3_2;
		vector3_2.X = Normal.X >= 0.0f ? box.Max.X : box.Min.X;
		vector3_2.Y = Normal.Y >= 0.0f ? box.Max.Y : box.Min.Y;
		vector3_2.Z = Normal.Z >= 0.0f ? box.Max.Z : box.Min.Z;

		if (Normal.X * vector3_1.X + Normal.Y * vector3_1.Y + Normal.Z * vector3_1.Z + D > 0.0F)
			return PlaneIntersectionType::Front;

		return Normal.X * vector3_2.X + Normal.Y * vector3_2.Y + Normal.Z * vector3_2.Z + D < 0.0F
			? PlaneIntersectionType::Back : PlaneIntersectionType::Intersecting;
	}

	constexpr bool BoundingFrustum::Intersects(BoundingBox const& box) {
		const auto containment = Contains(box);
		return containment != ContainmentType::Disjoint;
	}

	constexpr ContainmentType BoundingBox::Contains(BoundingFrustum& frustum) const {
		if (!frustum.Intersects(*this))
			return ContainmentType::Disjoint;

		for (size_t i = 0; i < frustum.CornerCount; ++i) {
			const auto corner = frustum[i];

			if (Contains(corner) == ContainmentType::Disjoint)
				return ContainmentType::Intersects;
		}
		return ContainmentType::Contains;
	}	
}

#endif