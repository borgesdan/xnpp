#ifndef XNA_FRAMEWORK_BOUNDINGSPHERE_HPP
#define XNA_FRAMEWORK_BOUNDINGSPHERE_HPP

#include "BoundingBox.hpp"
#include "Shared.hpp"

namespace Xna {
	//Defines a sphere. 
	struct BoundingSphere final {
		//The center point of the sphere.
		Vector3 Center{};
		//The radius of the sphere.
		float Radius{ 0 };
		//Creates a new instance of BoundingSphere.
		constexpr BoundingSphere() = default;
		//Creates a new instance of BoundingSphere.
		constexpr BoundingSphere(Vector3 const& center, float radius) :
			Center(center), Radius(radius < 0 ? 0 : radius) {
		}

		constexpr bool operator==(BoundingSphere const& other) const {
			return Center == other.Center && Radius == other.Radius;
		}

		constexpr operator std::optional<BoundingSphere>() const {
			return std::make_optional<BoundingSphere>(Center, Radius);
		}

		//Creates a BoundingSphere that contains the two specified BoundingSphere instances. 
		static constexpr BoundingSphere CreateMerged(BoundingSphere const& original, BoundingSphere const& additional);
		//Creates the smallest BoundingSphere that can contain a specified BoundingBox. 
		static constexpr BoundingSphere CreateFromBoundingBox(BoundingBox const& box);
		//Creates a BoundingSphere that can contain a specified list of points.
		static constexpr BoundingSphere CreateFromPoints(std::vector<Vector3> const& points) {
			return CreateFromPoints(points.data(), points.size());
		}
		//Creates a BoundingSphere that can contain a specified list of points.
		static constexpr BoundingSphere CreateFromPoints(Vector3 const* points, size_t size);

		//Creates the smallest BoundingSphere that can contain a specified BoundingFrustum. 
		static constexpr BoundingSphere CreateFromFrustum(BoundingFrustum const& points);

		//Checks whether the current BoundingSphere intersects another bounding volume.
		constexpr bool Intersects(BoundingBox const& box) const;
		//Checks whether the current BoundingSphere intersects another bounding volume.
		bool Intersects(BoundingFrustum& frustum) const;
		//Checks whether the current BoundingSphere intersects another bounding volume.
		constexpr PlaneIntersectionType Intersects(Plane const& plane) const;
		//Checks whether the current BoundingSphere intersects another bounding volume.
		constexpr std::optional<float> Intersects(Ray const& ray) const;
		//Checks whether the current BoundingSphere intersects another bounding volume.
		constexpr bool Intersects(BoundingSphere const& sphere) const;

		//Checks whether the current BoundingSphere contains a specified bounding volume.
		constexpr ContainmentType Contains(BoundingBox const& box) const;
		//Checks whether the current BoundingSphere contains a specified bounding volume.
		constexpr ContainmentType Contains(BoundingFrustum& frustum) const;
		//Checks whether the current BoundingSphere contains a specified bounding volume.
		constexpr ContainmentType Contains(Vector3 const& point) const;
		//Checks whether the current BoundingSphere contains a specified bounding volume.
		constexpr ContainmentType Contains(BoundingSphere const& sphere) const;

		//Translates and scales the BoundingSphere using a given Matrix.
		constexpr BoundingSphere Transform(Matrix const& matrix) const;

	private:
		constexpr void SupportMapping(Vector3 const& v, Vector3& result) const;
	};

	constexpr BoundingBox BoundingBox::CreateFromSphere(BoundingSphere const& sphere) {
		BoundingBox fromSphere;
		fromSphere.Min.X = sphere.Center.X - sphere.Radius;
		fromSphere.Min.Y = sphere.Center.Y - sphere.Radius;
		fromSphere.Min.Z = sphere.Center.Z - sphere.Radius;
		fromSphere.Max.X = sphere.Center.X + sphere.Radius;
		fromSphere.Max.Y = sphere.Center.Y + sphere.Radius;
		fromSphere.Max.Z = sphere.Center.Z + sphere.Radius;
		return fromSphere;
	}

	constexpr bool BoundingBox::Intersects(BoundingSphere const& sphere) const {
		const auto result1 = Vector3::Clamp(sphere.Center, Min, Max);
		const auto result2 = Vector3::DistanceSquared(sphere.Center, result1);
		return result2 <= sphere.Radius * sphere.Radius;
	}

	constexpr ContainmentType BoundingBox::Contains(BoundingSphere const& sphere) const {
		Vector3 result1 = Vector3::Clamp(sphere.Center, Min, Max);
		float result2 = Vector3::DistanceSquared(sphere.Center, result1);
		float radius = sphere.Radius;

		if (result2 > radius * radius)
			return ContainmentType::Disjoint;

		return Min.X + radius > sphere.Center.X || sphere.Center.X > Max.X - radius || Max.X - Min.X <= radius || Min.Y + radius > sphere.Center.Y || sphere.Center.Y > Max.Y - radius || Max.Y - Min.Y <= radius || Min.Z + radius > sphere.Center.Z || sphere.Center.Z > Max.Z - radius || Max.X - Min.X <= radius
			? ContainmentType::Intersects
			: ContainmentType::Contains;
	}

	constexpr bool BoundingSphere::Intersects(BoundingBox const& box) const {
		Vector3 result1 = Vector3::Clamp(Center, box.Min, box.Max);
		float result2 = Vector3::DistanceSquared(Center, result1);
		return result2 <= Radius * Radius;
	}

	inline bool BoundingSphere::Intersects(BoundingFrustum& frustum) const {
		return frustum.Intersects(*this);
	}

	constexpr PlaneIntersectionType BoundingSphere::Intersects(Plane const& plane) const {
		const auto num = (Center.X * plane.Normal.X + Center.Y * plane.Normal.Y + Center.Z * plane.Normal.Z) + plane.D;

		if (num > Radius)
			return PlaneIntersectionType::Front;

		return num < -Radius ? PlaneIntersectionType::Back : PlaneIntersectionType::Intersecting;
	}

	constexpr bool BoundingSphere::Intersects(BoundingSphere const& sphere) const {
		const auto result = Vector3::DistanceSquared(Center, sphere.Center);
		const auto radius1 = Radius;
		const auto radius2 = sphere.Radius;
		return radius1 * radius1 + 2.0F * radius1 * radius2 + radius2 * radius2 > result;
	}

	constexpr PlaneIntersectionType Plane::Intersects(BoundingSphere const& sphere) const {
		const auto num = (sphere.Center.X * Normal.X + sphere.Center.Y * Normal.Y + sphere.Center.Z * Normal.Z) + D;

		if (num > sphere.Radius)
			return PlaneIntersectionType::Front;

		return num < -sphere.Radius
			? PlaneIntersectionType::Back : PlaneIntersectionType::Intersecting;
	}

	constexpr ContainmentType BoundingFrustum::Contains(BoundingSphere const& sphere) const {
		auto center = sphere.Center;
		auto radius = sphere.Radius;
		auto num1 = 0;
		for (size_t i = 0; i < PlaneCount; ++i)
		{
			const auto& plane = planes[i];

			auto num2 = (plane.Normal.X * center.X + plane.Normal.Y * center.Y + plane.Normal.Z * center.Z) + plane.D;

			if (num2 > radius)
				return ContainmentType::Disjoint;

			if (num2 < -radius)
				++num1;
		}

		return num1 != 6 ? ContainmentType::Intersects : ContainmentType::Contains;
	}

	constexpr bool BoundingFrustum::Intersects(BoundingSphere const& sphere) {
		const auto containment = Contains(sphere);
		return containment != ContainmentType::Disjoint;
	}

	constexpr BoundingSphere BoundingSphere::CreateFromBoundingBox(BoundingBox const& box) {
		BoundingSphere fromBoundingBox;
		fromBoundingBox.Center = Vector3::Lerp(box.Min, box.Max, 0.5f);
		const auto result = Vector3::Distance(box.Min, box.Max);
		fromBoundingBox.Radius = result * 0.5f;
		return fromBoundingBox;
	}

	constexpr BoundingSphere BoundingSphere::CreateFromPoints(Vector3 const* points, size_t size) {
		Vector3 current;
		auto vector3_1 = current = points[0];
		auto vector3_2 = current;
		auto vector3_3 = current;
		auto vector3_4 = current;
		auto vector3_5 = current;
		auto vector3_6 = current;

		for (size_t i = 0; i < size; ++i) {
			const auto& point = points[i];

			if (point.X < vector3_6.X)
				vector3_6 = point;
			if (point.X > vector3_5.X)
				vector3_5 = point;
			if (point.Y < vector3_4.Y)
				vector3_4 = point;
			if (point.Y > vector3_3.Y)
				vector3_3 = point;
			if (point.Z < vector3_2.Z)
				vector3_2 = point;
			if (point.Z > vector3_1.Z)
				vector3_1 = point;
		}

		auto result1 = Vector3::Distance(vector3_5, vector3_6);
		auto result2 = Vector3::Distance(vector3_3, vector3_4);
		auto result3 = Vector3::Distance(vector3_1, vector3_2);
		Vector3 result4;
		float num1 = 0;

		if (result1 > result2)
		{
			if (result1 > result3)
			{
				result4 = Vector3::Lerp(vector3_5, vector3_6, 0.5f);
				num1 = result1 * 0.5f;
			}
			else
			{
				result4 = Vector3::Lerp(vector3_1, vector3_2, 0.5f);
				num1 = result3 * 0.5f;
			}
		}
		else if (result2 > result3)
		{
			result4 = Vector3::Lerp(vector3_3, vector3_4, 0.5f);
			num1 = result2 * 0.5f;
		}
		else
		{
			result4 = Vector3::Lerp(vector3_1, vector3_2, 0.5f);
			num1 = result3 * 0.5f;
		}

		for (size_t i = 0; i < size; ++i) {
			const auto& point = points[i];

			Vector3 vector3_7;
			vector3_7.X = point.X - result4.X;
			vector3_7.Y = point.Y - result4.Y;
			vector3_7.Z = point.Z - result4.Z;
			float num2 = vector3_7.Length();

			if (num2 > num1) {
				num1 = (num1 + num2) * 0.5F;
				result4 += (1.0F - num1 / num2) * vector3_7;
			}
		}

		BoundingSphere fromPoints;
		fromPoints.Center = result4;
		fromPoints.Radius = num1;
		return fromPoints;
	}

	constexpr BoundingSphere BoundingSphere::CreateFromFrustum(BoundingFrustum const& frustum) {
		return BoundingSphere::CreateFromPoints(frustum.corners, frustum.CornerCount);
	}

	constexpr ContainmentType BoundingSphere::Contains(BoundingFrustum& frustum) const {
		if (!frustum.Intersects(*this))
			return ContainmentType::Disjoint;

		float num = Radius * Radius;
		for (size_t i = 0; i < BoundingFrustum::CornerCount; ++i)
		{
			const auto corner = frustum[i];

			Vector3 vector3;
			vector3.X = corner.X - Center.X;
			vector3.Y = corner.Y - Center.Y;
			vector3.Z = corner.Z - Center.Z;

			if (vector3.LengthSquared() > num)
				return ContainmentType::Intersects;
		}

		return ContainmentType::Contains;
	}

	constexpr ContainmentType BoundingSphere::Contains(BoundingSphere const& sphere) const {
		const auto result = Vector3::Distance(Center, sphere.Center);
		const auto radius1 = Radius;
		const auto radius2 = sphere.Radius;
		if (radius1 + radius2 < result)
			return ContainmentType::Disjoint;
		return radius1 - radius2 < result
			? ContainmentType::Intersects
			: ContainmentType::Contains;
	}

	constexpr void BoundingSphere::SupportMapping(Vector3 const& v, Vector3& result) const {
		const auto num = Radius / v.Length();
		result.X = Center.X + v.X * num;
		result.Y = Center.Y + v.Y * num;
		result.Z = Center.Z + v.Z * num;
	}

	constexpr BoundingSphere BoundingSphere::Transform(Matrix const& matrix) const {
		BoundingSphere boundingSphere;
		boundingSphere.Center = Vector3::Transform(Center, matrix);
		float d = MathHelper::Max((matrix.M11 * matrix.M11 + matrix.M12 * matrix.M12 + matrix.M13 * matrix.M13), MathHelper::Max((matrix.M21 * matrix.M21 + matrix.M22 * matrix.M22 + matrix.M23 * matrix.M23), (matrix.M31 * matrix.M31 + matrix.M32 * matrix.M32 + matrix.M33 * matrix.M33)));
		boundingSphere.Radius = Radius * MathHelper::Extensions::Sqrt(d);
		return boundingSphere;
	}

	constexpr ContainmentType BoundingSphere::Contains(BoundingBox const& box) const {
		if (!box.Intersects(*this))
			return ContainmentType::Disjoint;
		float num = Radius * Radius;
		Vector3 vector3;
		vector3.X = Center.X - box.Min.X;
		vector3.Y = Center.Y - box.Max.Y;
		vector3.Z = Center.Z - box.Max.Z;
		if (vector3.LengthSquared() > num)
			return ContainmentType::Intersects;
		vector3.X = Center.X - box.Max.X;
		vector3.Y = Center.Y - box.Max.Y;
		vector3.Z = Center.Z - box.Max.Z;
		if (vector3.LengthSquared() > num)
			return ContainmentType::Intersects;
		vector3.X = Center.X - box.Max.X;
		vector3.Y = Center.Y - box.Min.Y;
		vector3.Z = Center.Z - box.Max.Z;
		if (vector3.LengthSquared() > num)
			return ContainmentType::Intersects;
		vector3.X = Center.X - box.Min.X;
		vector3.Y = Center.Y - box.Min.Y;
		vector3.Z = Center.Z - box.Max.Z;
		if (vector3.LengthSquared() > num)
			return ContainmentType::Intersects;
		vector3.X = Center.X - box.Min.X;
		vector3.Y = Center.Y - box.Max.Y;
		vector3.Z = Center.Z - box.Min.Z;
		if (vector3.LengthSquared() > num)
			return ContainmentType::Intersects;
		vector3.X = Center.X - box.Max.X;
		vector3.Y = Center.Y - box.Max.Y;
		vector3.Z = Center.Z - box.Min.Z;
		if (vector3.LengthSquared() > num)
			return ContainmentType::Intersects;
		vector3.X = Center.X - box.Max.X;
		vector3.Y = Center.Y - box.Min.Y;
		vector3.Z = Center.Z - box.Min.Z;
		if (vector3.LengthSquared() > num)
			return ContainmentType::Intersects;
		vector3.X = Center.X - box.Min.X;
		vector3.Y = Center.Y - box.Min.Y;
		vector3.Z = Center.Z - box.Min.Z;
		return vector3.LengthSquared() > num
			? ContainmentType::Intersects
			: ContainmentType::Contains;
	}

	constexpr ContainmentType BoundingSphere::Contains(Vector3 const& point) const {
		return Vector3::DistanceSquared(point, Center) >= Radius * Radius
			? ContainmentType::Disjoint
			: ContainmentType::Contains;
	}

	constexpr BoundingSphere BoundingSphere::CreateMerged(BoundingSphere const& original, BoundingSphere const& additional) {
		Vector3 result = Vector3::Subtract(additional.Center, original.Center);
		float num1 = result.Length();
		float radius1 = original.Radius;
		float radius2 = additional.Radius;

		if (radius1 + radius2 >= num1) {
			if (radius1 - radius2 >= num1)
				return original;
			if (radius2 - radius1 >= num1)
				return additional;
		}

		Vector3 vector3 = result * (1.0f / num1);
		const auto num2 = MathHelper::Min(-radius1, num1 - radius2);
		const auto num3 = (MathHelper::Max(radius1, num1 + radius2) - num2) * 0.5F;
		BoundingSphere merged;
		merged.Center = original.Center + vector3 * (num3 + num2);
		merged.Radius = num3;
		return merged;
	}
}

#endif