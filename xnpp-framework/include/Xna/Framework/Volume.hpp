#ifndef XNA_FRAMEWORK_PLANE_HPP
#define XNA_FRAMEWORK_PLANE_HPP

#include "MathHelper.hpp"
#include "MathTypes.hpp"
#include <optional>
#include <cstddef>
#include <vector>
#include <span>
#include <limits>

namespace Xna {
	struct Plane;
	struct BoundingFrustum;
	struct BoundingBox;
	struct BoundingSphere;
	struct Ray;

	//////////////////////////////////////////////////////////////////
	// ENUMS														//
	//////////////////////////////////////////////////////////////////

	//Indicates the extent to which bounding volumes intersect or contain one another. 
	enum class ContainmentType {
		//Indicates there is no overlap between the bounding volumes.
		Disjoint,
		//Indicates that one bounding volume completely contains the other.
		Contains,
		//Indicates that the bounding volumes partially overlap.
		Intersects,
	};

	//Describes the intersection between a plane and a bounding volume.
	enum class PlaneIntersectionType {
		//There is no intersection, and the bounding volume is in the positive half-space of the Plane.
		Front,
		//There is no intersection, and the bounding volume is in the negative half - space of the Plane.
		Back,
		//The Plane is intersected.
		Intersecting,
	};


	//////////////////////////////////////////////////////////////////
	// PLANE														//
	//////////////////////////////////////////////////////////////////

	//Defines a plane. 
	struct Plane final {
		//The normal vector of the Plane.
		Vector3 Normal{ 0 };
		//The distance of the Plane along its normal from the origin. 
		float D{ 0 };

		//Creates an instance of Plane.
		constexpr Plane() noexcept = default;

		//Creates an instance of Plane.
		constexpr Plane(float a, float b, float c, float d) noexcept : Normal({ a,b,c }), D(d) {}

		//Creates an instance of Plane.
		constexpr Plane(Vector3 normal, float d) noexcept : Normal(normal), D(d) {}

		//Creates an instance of Plane.
		constexpr Plane(Vector4 value) noexcept : Normal({ value.X, value.Y, value.Z }), D(value.W) {}

		//Creates an instance of Plane.
		constexpr Plane(Vector3 point1, Vector3 point2, Vector3 point3) noexcept {
			const auto num1 = point2.X - point1.X;
			const auto num2 = point2.Y - point1.Y;
			const auto num3 = point2.Z - point1.Z;
			const auto num4 = point3.X - point1.X;
			const auto num5 = point3.Y - point1.Y;
			const auto num6 = point3.Z - point1.Z;
			const auto num7 = (num2 * num6 - num3 * num5);
			const auto num8 = (num3 * num4 - num1 * num6);
			const auto num9 = (num1 * num5 - num2 * num4);
			const auto num10 = 1.0f / MathHelper::Sqrt(num7 * num7 + num8 * num8 + num9 * num9);
			Normal.X = num7 * num10;
			Normal.Y = num8 * num10;
			Normal.Z = num9 * num10;
			D = -(Normal.X * point1.X + Normal.Y * point1.Y + Normal.Z * point1.Z);
		}

		//Changes the coefficients of the Normal vector of a Plane to make it of unit length. 
		constexpr void Normalize() noexcept {
			const auto d = (Normal.X * Normal.X + Normal.Y * Normal.Y + Normal.Z * Normal.Z);

			if (MathHelper::Abs(d - 1.0f) < 1.1920928955078125E-07)
				return;

			const auto num = 1.0f / MathHelper::Sqrt(d);

			Normal.X *= num;
			Normal.Y *= num;
			Normal.Z *= num;
			D *= num;
		}

		//Changes the coefficients of the Normal vector of a Plane to make it of unit length. 
		[[nodiscard]] static constexpr Plane Normalize(Plane value) noexcept {
			value.Normalize();
			return value;
		}

		//Transforms a normalized Plane by a Matrix or Quaternion.
		[[nodiscard]] static constexpr Plane Transform(Plane plane, Matrix const& matrix) noexcept {
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

		//Transforms a normalized Plane by a Matrix or Quaternion.
		[[nodiscard]] static constexpr Plane Transform(Plane plane, Quaternion const& rotation) noexcept {
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

		//Calculates the dot product of a specified Vector4 and this Plane.
		[[nodiscard]] constexpr float Dot(Vector4 value) const noexcept {
			return (Normal.X * value.X + Normal.Y * value.Y + Normal.Z * value.Z + D * value.W);
		}

		//Returns the dot product of a specified Vector3 and the Normal vector of this Plane plus the D constant value of the Plane.
		[[nodiscard]] constexpr float DotCoordinate(Vector3 value) const noexcept {
			return (Normal.X * value.X + Normal.Y * value.Y + Normal.Z * value.Z) + D;
		}
		//Returns the dot product of a specified Vector3 and the Normal vector of this Plane. 
		[[nodiscard]] constexpr float DotNormal(Vector3 value) const noexcept {
			return (Normal.X * value.X + Normal.Y * value.Y + Normal.Z * value.Z);
		}

		[[nodiscard]] static constexpr Matrix CreateShadow(Vector3 lightDirection, Plane plane) noexcept {
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

		[[nodiscard]] static constexpr Matrix CreateReflection(Plane value) noexcept {
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

		//Checks whether a Plane intersects a bounding volume.
		[[nodiscard]] constexpr PlaneIntersectionType Intersects(BoundingBox const& box) const noexcept;
		//Checks whether a Plane intersects a bounding volume.
		[[nodiscard]] constexpr PlaneIntersectionType Intersects(BoundingFrustum const& frustum) const noexcept;
		//Checks whether a Plane intersects a bounding volume.
		[[nodiscard]] constexpr PlaneIntersectionType Intersects(BoundingSphere const& sphere) const noexcept;
		//Checks whether a Plane intersects a bounding volume.
		[[nodiscard]] constexpr std::optional<float> Intersects(Ray const& ray) const noexcept;

		[[nodiscard]] constexpr bool operator==(const Plane& other) const noexcept = default;
	};

	//////////////////////////////////////////////////////////////////
	// BOUNDING FRUSTUM												//
	//////////////////////////////////////////////////////////////////

	//Defines a frustum and helps determine whether forms intersect with it. 
	struct BoundingFrustum final {
		struct InternalRay final {
			Vector3 Position{};
			Vector3 Direction{};
		};

		//Specifies the total number of corners (8) in the BoundingFrustum.
		static constexpr int CornerCount = 8;

		//Creates a new instance of BoundingFrustum.
		constexpr BoundingFrustum() noexcept = default;
		//Creates a new instance of BoundingFrustum.
		constexpr BoundingFrustum(Xna::Matrix const& matrix) noexcept {
			Matrix(matrix);
		}

		//Gets the near plane of the BoundingFrustum.
		[[nodiscard]] constexpr Plane Near() const noexcept { return planes[0]; }
		//Gets the far plane of the BoundingFrustum.
		[[nodiscard]] constexpr Plane Far() const noexcept { return planes[1]; }
		//Gets the left plane of the BoundingFrustum.
		[[nodiscard]] constexpr Plane Left() const noexcept { return planes[2]; }
		//Gets the right plane of the BoundingFrustum.
		[[nodiscard]] constexpr Plane Right() const noexcept { return planes[3]; }
		//Gets the top plane of the BoundingFrustum.
		[[nodiscard]] constexpr Plane Top() const noexcept { return planes[4]; }
		//Gets the bottom plane of the BoundingFrustum.
		[[nodiscard]] constexpr Plane Bottom() const noexcept { return planes[5]; }

		//Gets an array of points that make up the corners of the BoundingFrustum.
		constexpr void GetCorners(std::span<Vector3, CornerCount> destination) const {
			for (size_t i = 0; i < CornerCount; ++i)
				destination[i] = corners[i];
		}

		//Gets or sets the Matrix that describes this bounding frustum.
		[[nodiscard]] constexpr const Xna::Matrix& Matrix() const noexcept { return matrix; }
		
		//Gets or sets the Matrix that describes this bounding frustum.
		constexpr void Matrix(Xna::Matrix const& value) noexcept {
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
		
		[[nodiscard]] constexpr PlaneIntersectionType Intersects(Plane plane) const noexcept {
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

		[[nodiscard]] constexpr bool Intersects(BoundingFrustum const& frustum) const noexcept {
			return Contains(frustum) != ContainmentType::Disjoint;
		}
		
		[[nodiscard]] constexpr ContainmentType Contains(BoundingFrustum const& frustum) const noexcept {
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
		
		[[nodiscard]] constexpr ContainmentType Contains(Vector3 point) const noexcept{
			for (size_t i = 0; i < PlaneCount; ++i) {
				const auto& plane = planes[i];

				if ((static_cast<double>(plane.Normal.X * point.X + plane.Normal.Y * point.Y + plane.Normal.Z * point.Z) + plane.D) > 9.9999997473787516E-06)
					return ContainmentType::Disjoint;
			}

			return ContainmentType::Contains;
		}
		
		[[nodiscard]] constexpr ContainmentType Contains(BoundingSphere box) const noexcept;
		[[nodiscard]] constexpr ContainmentType Contains(BoundingBox const& box) const noexcept;
		[[nodiscard]] constexpr std::optional<float> Intersects(Ray const& ray) const noexcept;
		[[nodiscard]] constexpr bool Intersects(BoundingBox const& box) const noexcept;
		[[nodiscard]] constexpr bool Intersects(BoundingSphere box) const noexcept;

		[[nodiscard]] constexpr bool operator==(BoundingFrustum const& other) const noexcept = default;

		[[nodiscard]] constexpr Vector3 operator[](int index) const noexcept{			
			if (index < 0)
				index = 0;
			else if(index > CornerCount)
				index = CornerCount - 1;

			return corners[static_cast<size_t>(index)];
		}		

	private:
		static constexpr InternalRay ComputeIntersectionLine(Plane p1, Plane p2) noexcept {
			InternalRay intersectionLine{};
			intersectionLine.Direction = Vector3::Cross(p1.Normal, p2.Normal);
			const auto num = intersectionLine.Direction.LengthSquared();
			intersectionLine.Position = Vector3::Cross(-p1.D * p2.Normal + p2.D * p1.Normal, intersectionLine.Direction) / num;
			return intersectionLine;
		}

		static constexpr Vector3 ComputeIntersection(Plane plane, InternalRay const& ray) noexcept {
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

	//////////////////////////////////////////////////////////////////
	// BOUNDING BOX													//
	//////////////////////////////////////////////////////////////////

	//Defines an axis-aligned box-shaped 3D volume. 
	struct BoundingBox final {
		//Specifies the total number of corners (8) in the BoundingBox.
		static constexpr int CornerCount = 8;

		//The maximum point the BoundingBox contains.
		Vector3 Min{};
		//The minimum point the BoundingBox contains. 
		Vector3 Max{};

		//Creates a new instance of BoundingBox.
		constexpr BoundingBox() noexcept = default;
		//Creates a new instance of BoundingBox.
		constexpr BoundingBox(Vector3 min, Vector3 max) noexcept : Min(min), Max(max) {}

		[[nodiscard]] constexpr bool operator==(BoundingBox const& other) const noexcept = default;

		//Gets an array of points that make up the corners of the BoundingBox.
		constexpr void GetCorners(std::span<Vector3, CornerCount> corners) const noexcept {
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

		//Creates the smallest BoundingBox that contains the two specified BoundingBox instances. 
		[[nodiscard]] static constexpr BoundingBox CreateMerged(BoundingBox const& original, BoundingBox const& additional) noexcept {
			BoundingBox merged;
			merged.Min = Vector3::Min(original.Min, additional.Min);
			merged.Max = Vector3::Max(original.Max, additional.Max);
			return merged;
		}		

		//Creates the smallest BoundingBox that will contain a group of points.
		[[nodiscard]] static constexpr BoundingBox CreateFromPoints(std::span<const Vector3> points, size_t size) noexcept {
			constexpr float floatMinVal = (std::numeric_limits<float>::min)();
			constexpr float floatMaxVal = (std::numeric_limits<float>::max)();
			
			Vector3 result1 = Vector3(floatMaxVal);
			Vector3 result2 = Vector3(floatMinVal);

			for (size_t i = 0; i < size; ++i) {
				const auto& point = points[i];
				result1 = Vector3::Min(result1, point);
				result2 = Vector3::Max(result2, point);
			}

			return BoundingBox(result1, result2);
		}

		//Checks whether the current BoundingBox intersects with another bounding volume.
		[[nodiscard]] constexpr bool Intersects(BoundingBox const& box) const noexcept {
			return Max.X >= box.Min.X
				&& Min.X <= box.Max.X
				&& Max.Y >= box.Min.Y
				&& Min.Y <= box.Max.Y
				&& Max.Z >= box.Min.Z
				&& Min.Z <= box.Max.Z;
		}

		//Checks whether the current BoundingBox intersects with another bounding volume.
		[[nodiscard]] bool Intersects(BoundingFrustum const& frustum) const noexcept {
			return frustum.Intersects(*this);
		}

		//Checks whether the current BoundingBox intersects with another bounding volume.
		[[nodiscard]] constexpr PlaneIntersectionType Intersects(Plane plane) const noexcept {
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

		//Tests whether the BoundingBox overlaps another bounding volume.
		[[nodiscard]] constexpr ContainmentType Contains(BoundingBox const& box) const noexcept {
			if (Max.X < box.Min.X || Min.X > box.Max.X || Max.Y < box.Min.Y || Min.Y > box.Max.Y || Max.Z < box.Min.Z || Min.Z > box.Max.Z)
				return ContainmentType::Disjoint;
			return Min.X > box.Min.X || box.Max.X > Max.X || Min.Y > box.Min.Y || box.Max.Y > Max.Y || Min.Z > box.Min.Z || box.Max.Z > Max.Z
				? ContainmentType::Intersects
				: ContainmentType::Contains;
		}

		//Tests whether the BoundingBox overlaps another bounding volume.
		[[nodiscard]] constexpr ContainmentType Contains(BoundingFrustum const& frustum) const noexcept {
			if (!frustum.Intersects(*this))
				return ContainmentType::Disjoint;

			for (size_t i = 0; i < frustum.CornerCount; ++i) {
				const auto corner = frustum[i];

				if (Contains(corner) == ContainmentType::Disjoint)
					return ContainmentType::Intersects;
			}
			return ContainmentType::Contains;
		}

		//Tests whether the BoundingBox overlaps another bounding volume.
		[[nodiscard]] constexpr ContainmentType Contains(Vector3 point) const noexcept {
			return Min.X > point.X || point.X > Max.X || Min.Y > point.Y || point.Y > Max.Y || Min.Z > point.Z || point.Z > Max.Z
				? ContainmentType::Disjoint
				: ContainmentType::Contains;
		}

		//Creates the smallest BoundingBox that will contain the specified BoundingSphere. 
		[[nodiscard]] static constexpr BoundingBox CreateFromSphere(BoundingSphere sphere) noexcept;

		//Checks whether the current BoundingBox intersects with another bounding volume.
		[[nodiscard]] constexpr std::optional<float> Intersects(Ray const& ray) const noexcept;
		//Checks whether the current BoundingBox intersects with another bounding volume.
		[[nodiscard]] constexpr bool Intersects(BoundingSphere const& sphere) const noexcept;

		//Tests whether the BoundingBox overlaps another bounding volume.
		[[nodiscard]] constexpr ContainmentType Contains(BoundingSphere const& sphere) const noexcept;

	private:
		constexpr void SupportMapping(Vector3 v, Vector3& result) const noexcept {
			result.X = v.X >= 0.0 ? Max.X : Min.X;
			result.Y = v.Y >= 0.0 ? Max.Y : Min.Y;
			result.Z = v.Z >= 0.0 ? Max.Z : Min.Z;
		}
	};

	//////////////////////////////////////////////////////////////////
	// BOUNDING SPHERE												//
	//////////////////////////////////////////////////////////////////

	//Defines a sphere. 
	struct BoundingSphere final {
		//The center point of the sphere.
		Vector3 Center{};
		//The radius of the sphere.
		float Radius{ 0 };

		//Creates a new instance of BoundingSphere.
		constexpr BoundingSphere() noexcept = default;
		//Creates a new instance of BoundingSphere.
		constexpr BoundingSphere(Vector3 center, float radius) noexcept : Center(center), Radius(radius < 0 ? 0 : radius) {}

		[[nodiscard]] constexpr bool operator==(BoundingSphere const& other) const noexcept = default;		

		//Creates a BoundingSphere that contains the two specified BoundingSphere instances. 
		[[nodiscard]] static constexpr BoundingSphere CreateMerged(BoundingSphere const& original, BoundingSphere const& additional) noexcept {
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

		//Creates the smallest BoundingSphere that can contain a specified BoundingBox. 
		[[nodiscard]] static constexpr BoundingSphere CreateFromBoundingBox(BoundingBox const& box) noexcept {
			BoundingSphere fromBoundingBox;
			fromBoundingBox.Center = Vector3::Lerp(box.Min, box.Max, 0.5f);
			const auto result = Vector3::Distance(box.Min, box.Max);
			fromBoundingBox.Radius = result * 0.5f;
			return fromBoundingBox;
		}		

		//Creates a BoundingSphere that can contain a specified list of points.
		[[nodiscard]] static constexpr BoundingSphere CreateFromPoints(std::span<const Vector3> points, size_t size) noexcept{
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

		//Creates the smallest BoundingSphere that can contain a specified BoundingFrustum. 
		[[nodiscard]] static constexpr BoundingSphere CreateFromFrustum(BoundingFrustum const& frustum) noexcept {
			return BoundingSphere::CreateFromPoints(frustum.corners, frustum.CornerCount);
		}

		//Checks whether the current BoundingSphere intersects another bounding volume.
		[[nodiscard]] constexpr bool Intersects(BoundingBox const& box) const noexcept {
			Vector3 result1 = Vector3::Clamp(Center, box.Min, box.Max);
			float result2 = Vector3::DistanceSquared(Center, result1);
			return result2 <= Radius * Radius;
		}

		//Checks whether the current BoundingSphere intersects another bounding volume.
		[[nodiscard]] bool Intersects(BoundingFrustum& frustum) const noexcept {
			return frustum.Intersects(*this);
		}

		//Checks whether the current BoundingSphere intersects another bounding volume.
		[[nodiscard]] constexpr PlaneIntersectionType Intersects(Plane plane) const noexcept {
			const auto num = (Center.X * plane.Normal.X + Center.Y * plane.Normal.Y + Center.Z * plane.Normal.Z) + plane.D;

			if (num > Radius)
				return PlaneIntersectionType::Front;

			return num < -Radius ? PlaneIntersectionType::Back : PlaneIntersectionType::Intersecting;
		}
		
		//Checks whether the current BoundingSphere intersects another bounding volume.
		[[nodiscard]] constexpr bool Intersects(BoundingSphere const& sphere) const noexcept {
			const auto result = Vector3::DistanceSquared(Center, sphere.Center);
			const auto radius1 = Radius;
			const auto radius2 = sphere.Radius;
			return radius1 * radius1 + 2.0F * radius1 * radius2 + radius2 * radius2 > result;
		}

		//Checks whether the current BoundingSphere contains a specified bounding volume.
		[[nodiscard]] constexpr ContainmentType Contains(BoundingBox const& box) const noexcept {
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

		//Checks whether the current BoundingSphere contains a specified bounding volume.
		[[nodiscard]] constexpr ContainmentType Contains(BoundingFrustum& frustum) const noexcept {
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

		//Checks whether the current BoundingSphere contains a specified bounding volume.
		[[nodiscard]] constexpr ContainmentType Contains(Vector3 point) const noexcept {
			return Vector3::DistanceSquared(point, Center) >= Radius * Radius
				? ContainmentType::Disjoint
				: ContainmentType::Contains;
		}

		//Checks whether the current BoundingSphere contains a specified bounding volume.
		[[nodiscard]] constexpr ContainmentType Contains(BoundingSphere const& sphere) const noexcept {
			const auto result = Vector3::Distance(Center, sphere.Center);
			const auto radius1 = Radius;
			const auto radius2 = sphere.Radius;
			if (radius1 + radius2 < result)
				return ContainmentType::Disjoint;
			return radius1 - radius2 < result
				? ContainmentType::Intersects
				: ContainmentType::Contains;
		}

		//Translates and scales the BoundingSphere using a given Matrix.
		[[nodiscard]] constexpr BoundingSphere Transform(Matrix const& matrix) const noexcept {
			BoundingSphere boundingSphere;
			boundingSphere.Center = Vector3::Transform(Center, matrix);
			float d = MathHelper::Max((matrix.M11 * matrix.M11 + matrix.M12 * matrix.M12 + matrix.M13 * matrix.M13), MathHelper::Max((matrix.M21 * matrix.M21 + matrix.M22 * matrix.M22 + matrix.M23 * matrix.M23), (matrix.M31 * matrix.M31 + matrix.M32 * matrix.M32 + matrix.M33 * matrix.M33)));
			boundingSphere.Radius = Radius * MathHelper::Sqrt(d);
			return boundingSphere;
		}

		//Checks whether the current BoundingSphere intersects another bounding volume.
		[[nodiscard]] constexpr std::optional<float> Intersects(Ray const& ray) const noexcept;

	private:
		constexpr void SupportMapping(Vector3 v, Vector3& result) const noexcept {
			const auto num = Radius / v.Length();
			result.X = Center.X + v.X * num;
			result.Y = Center.Y + v.Y * num;
			result.Z = Center.Z + v.Z * num;
		}
	};

	//////////////////////////////////////////////////////////////////
	// RAY															//
	//////////////////////////////////////////////////////////////////

	struct Ray final {
		//Specifies the starting point of the Ray.
		Vector3 Position{};
		//Unit vector specifying the direction the Ray is pointing.
		Vector3 Direction{};
		//Creates a new instance of Ray.
		constexpr Ray() = default;
		
		//Creates a new instance of Ray.
		constexpr Ray(Vector3 position, Vector3 direction) noexcept : Position(position), Direction(direction) {}

		constexpr bool operator==(Ray const& other) const noexcept = default;		

		//Checks whether the Ray intersects a specified plane or bounding volume. 
		[[nodiscard]] constexpr std::optional<float> Intersects(BoundingBox const& box) const noexcept {
			return box.Intersects(*this);
		}

		//Checks whether the Ray intersects a specified plane or bounding volume. 
		[[nodiscard]] constexpr std::optional<float> Intersects(BoundingFrustum const& frustum) const noexcept {
			return frustum.Intersects(*this);
		}

		//Checks whether the Ray intersects a specified plane or bounding volume. 
		[[nodiscard]] constexpr std::optional<float> Intersects(Plane plane) const noexcept {
			return plane.Intersects(*this);
		}

		//Checks whether the Ray intersects a specified plane or bounding volume. 
		[[nodiscard]] constexpr std::optional<float> Intersects(BoundingSphere const& sphere) const noexcept {
			return sphere.Intersects(*this);
		}
	};

	//////////////////////////////////////////////////////////////////
	// IMPLS														//
	//////////////////////////////////////////////////////////////////

	constexpr PlaneIntersectionType Plane::Intersects(BoundingFrustum const& frustum) const noexcept {
		return frustum.Intersects(*this);
	}

	constexpr PlaneIntersectionType Plane::Intersects(BoundingBox const& box) const noexcept {
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

	constexpr bool BoundingFrustum::Intersects(BoundingBox const& box) const noexcept {
		const auto containment = Contains(box);
		return containment != ContainmentType::Disjoint;
	}

	constexpr ContainmentType BoundingFrustum::Contains(BoundingBox const& box) const noexcept {
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

	constexpr PlaneIntersectionType Plane::Intersects(BoundingSphere const& sphere) const noexcept {
		const auto num = (sphere.Center.X * Normal.X + sphere.Center.Y * Normal.Y + sphere.Center.Z * Normal.Z) + D;

		if (num > sphere.Radius)
			return PlaneIntersectionType::Front;

		return num < -sphere.Radius
			? PlaneIntersectionType::Back : PlaneIntersectionType::Intersecting;
	}

	constexpr ContainmentType BoundingFrustum::Contains(BoundingSphere sphere) const noexcept {
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

	constexpr bool BoundingFrustum::Intersects(BoundingSphere sphere) const noexcept {
		const auto containment = Contains(sphere);
		return containment != ContainmentType::Disjoint;
	}

	constexpr BoundingBox BoundingBox::CreateFromSphere(BoundingSphere sphere) noexcept {
		BoundingBox fromSphere;
		fromSphere.Min.X = sphere.Center.X - sphere.Radius;
		fromSphere.Min.Y = sphere.Center.Y - sphere.Radius;
		fromSphere.Min.Z = sphere.Center.Z - sphere.Radius;
		fromSphere.Max.X = sphere.Center.X + sphere.Radius;
		fromSphere.Max.Y = sphere.Center.Y + sphere.Radius;
		fromSphere.Max.Z = sphere.Center.Z + sphere.Radius;
		return fromSphere;
	}

	constexpr bool BoundingBox::Intersects(BoundingSphere const& sphere) const noexcept {
		const auto result1 = Vector3::Clamp(sphere.Center, Min, Max);
		const auto result2 = Vector3::DistanceSquared(sphere.Center, result1);
		return result2 <= sphere.Radius * sphere.Radius;
	}

	constexpr ContainmentType BoundingBox::Contains(BoundingSphere const& sphere) const noexcept {
		Vector3 result1 = Vector3::Clamp(sphere.Center, Min, Max);
		float result2 = Vector3::DistanceSquared(sphere.Center, result1);
		float radius = sphere.Radius;

		if (result2 > radius * radius)
			return ContainmentType::Disjoint;

		return Min.X + radius > sphere.Center.X || sphere.Center.X > Max.X - radius || Max.X - Min.X <= radius || Min.Y + radius > sphere.Center.Y || sphere.Center.Y > Max.Y - radius || Max.Y - Min.Y <= radius || Min.Z + radius > sphere.Center.Z || sphere.Center.Z > Max.Z - radius || Max.X - Min.X <= radius
			? ContainmentType::Intersects
			: ContainmentType::Contains;
	}
}

#endif