#ifndef XNA_FRAMEWORK_MATRIX_HPP
#define XNA_FRAMEWORK_MATRIX_HPP

#include "Vector4.hpp"

namespace Xna {
	struct Plane;

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
		constexpr Matrix() = default;

		//Initializes a new instance of Matrix.
		constexpr Matrix(
			float M11, float M12, float M13, float M14,
			float M21, float M22, float M23, float M24,
			float M31, float M32, float M33, float M34,
			float M41, float M42, float M43, float M44) :
			M11(M11), M12(M12), M13(M13), M14(M14),
			M21(M21), M22(M22), M23(M23), M24(M24),
			M31(M31), M32(M32), M33(M33), M34(M34),
			M41(M41), M42(M42), M43(M43), M44(M44) {
		}		

		//Returns an instance of the identity matrix.
		constexpr static Matrix Identity() {
			return Matrix(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
		}

		//Gets and sets the up vector of the Matrix.
		constexpr Vector3 Up() const noexcept { return Vector3(M21, M22, M23); }
		//Gets and sets the down vector of the Matrix.
		constexpr Vector3 Down() const noexcept { return Vector3(-M21, -M22, -M23); }
		//Gets and sets the right vector of the Matrix.
		constexpr Vector3 Right() const noexcept { return Vector3(M11, M12, M13); }
		//Gets and sets the left vector of the Matrix.
		constexpr Vector3 Left() const noexcept { return Vector3(-M11, -M12, -M13); }
		//Gets and sets the forward vector of the Matrix.
		constexpr Vector3 Forward() const noexcept { return Vector3(-M31, -M32, -M33); }
		//Gets and sets the backward vector of the Matrix.
		constexpr Vector3 Backward() const noexcept { return Vector3(M31, M32, M33); }
		//Gets and sets the translation vector of the Matrix.
		constexpr Vector3 Translation() const noexcept { return Vector3(M41, M42, M43); }

		//Gets and sets the up vector of the Matrix.
		constexpr void Up(Vector3 const& value) noexcept {
			M21 = value.X;
			M22 = value.Y;
			M23 = value.Z;
		}

		//Gets and sets the down vector of the Matrix.
		constexpr void Down(Vector3 const& value) noexcept {
			M21 = -value.X;
			M22 = -value.Y;
			M23 = -value.Z;
		}

		//Gets and sets the right vector of the Matrix.
		constexpr void Right(Vector3 const& value) noexcept {
			M11 = value.X;
			M12 = value.Y;
			M13 = value.Z;
		}

		//Gets and sets the left vector of the Matrix.
		constexpr void Left(Vector3 const& value) noexcept {
			M11 = -value.X;
			M12 = -value.Y;
			M13 = -value.Z;
		}

		//Gets and sets the forward vector of the Matrix.
		constexpr void Forward(Vector3 const& value) noexcept {
			M31 = -value.X;
			M32 = -value.Y;
			M33 = -value.Z;
		}

		//Gets and sets the backward vector of the Matrix.
		constexpr void Backward(Vector3 const& value) noexcept {
			M31 = value.X;
			M32 = value.Y;
			M33 = value.Z;
		}

		//Gets and sets the translation vector of the Matrix.
		constexpr void Translation(Vector3 const& value) noexcept {
			M41 = value.X;
			M42 = value.Y;
			M43 = value.Z;
		}

		//Creates a spherical billboard that rotates around a specified object position.
		static constexpr Matrix CreateBillboard(Vector3 const& objectPosition, Vector3 const& cameraPosition, Vector3 const& cameraUpVector, std::optional<Vector3> const& cameraForwardVector) {
			Vector3 result1;
			result1.X = objectPosition.X - cameraPosition.X;
			result1.Y = objectPosition.Y - cameraPosition.Y;
			result1.Z = objectPosition.Z - cameraPosition.Z;
			const auto d = result1.LengthSquared();

			if (static_cast<double>(d) < 9.9999997473787516E-05)
				result1 = cameraForwardVector ? -cameraForwardVector.value() : Vector3::Forward();
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
		static constexpr Matrix CreateConstrainedBillboard(
			Vector3 const& objectPosition,
			Vector3 const& cameraPosition,
			Vector3 const& rotateAxis,
			std::optional<Vector3> const& cameraForwardVector,
			std::optional<Vector3> const& objectForwardVector) {
			Vector3 result1;
			result1.X = objectPosition.X - cameraPosition.X;
			result1.Y = objectPosition.Y - cameraPosition.Y;
			result1.Z = objectPosition.Z - cameraPosition.Z;
			const auto d = result1.LengthSquared();
			if (static_cast<double>(d) < 9.9999997473787516E-05)
				result1 = cameraForwardVector ? -cameraForwardVector.value() : Vector3::Forward();
			else
				result1 = Vector3::Multiply(result1, 1.0f / std::sqrt(d));

			const auto vector2 = rotateAxis;
			float result2 = Vector3::Dot(rotateAxis, result1);
			Vector3 result3;
			Vector3 result4;
			if (MathHelper::Extensions::Abs(result2) > 0.998254656791687) {
				if (objectForwardVector)
				{
					result3 = objectForwardVector.value();
					result2 = Vector3::Dot(rotateAxis, result3);

					if (MathHelper::Extensions::Abs(result2) > 0.998254656791687)
						result3 = MathHelper::Extensions::Abs((rotateAxis.X * Vector3::Forward().X + rotateAxis.Y * Vector3::Forward().Y + rotateAxis.Z * Vector3::Forward().Z)) > 0.998254656791687 ? Vector3::Right() : Vector3::Forward();
				}
				else
					result3 = MathHelper::Extensions::Abs((rotateAxis.X * Vector3::Forward().X + rotateAxis.Y * Vector3::Forward().Y + rotateAxis.Z * Vector3::Forward().Z)) > 0.998254656791687 ? Vector3::Right() : Vector3::Forward();

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
		static constexpr Matrix CreateTranslation(Vector3 const& position) {
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
		static constexpr Matrix CreateTranslation(float xPosition, float yPosition, float zPosition) {
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
		static constexpr Matrix CreateScale(float xScale, float yScale, float zScale) {
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
		static constexpr Matrix CreateScale(Vector3 const& scales) {
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
		static constexpr Matrix CreateScale(float scale) {
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
		static Matrix CreateRotationX(float radians);
		//Returns a y-axis rotation matrix.
		static Matrix CreateRotationY(float radians);
		//Returns an z-axis rotation matrix.
		static Matrix CreateRotationZ(float radians);
		//Creates a new Matrix that rotates around an arbitrary vector.
		static Matrix CreateFromAxisAngle(Vector3 const& axis, float angle);
		//Builds a perspective projection matrix based on a field of view.
		static Matrix CreatePerspectiveFieldOfView(
			float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance);
		//Builds a perspective projection matrix.
		static constexpr Matrix CreatePerspective(
			float width, float height, float nearPlaneDistance, float farPlaneDistance) {
			if (nearPlaneDistance <= 0.0 || farPlaneDistance <= 0.0 || nearPlaneDistance >= farPlaneDistance) {
				return Matrix();
			}

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
		static constexpr Matrix CreatePerspectiveOffCenter(
			float left, float right, float bottom, float top,
			float nearPlaneDistance, float farPlaneDistance) {
			if (nearPlaneDistance <= 0.0 || farPlaneDistance <= 0.0 || nearPlaneDistance >= farPlaneDistance) {
				return Matrix();
			}

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
		static constexpr Matrix CreateOrthographic(float width, float height, float zNearPlane, float zFarPlane) {
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
		static constexpr Matrix CreateOrthographicOffCenter(
			float left, float right, float bottom, float top, float zNearPlane, float zFarPlane) {
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
		static constexpr Matrix CreateLookAt(
			Vector3 const& cameraPosition, Vector3 const& cameraTarget, Vector3 const& cameraUpVector) {
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
		static constexpr Matrix CreateWorld(
			Vector3 const& position, Vector3 const& forward, Vector3 const& up) {
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
		static constexpr Matrix CreateFromQuaternion(Quaternion const& quaternion);
		//Creates a new rotation matrix from a specified yaw, pitch, and roll.
		static constexpr Matrix CreateFromYawPitchRoll(float yaw, float pitch, float roll);
		static constexpr Matrix CreateShadow(Vector3 const& lightDirection, Plane const& plane);
		static constexpr Matrix CreateReflection(Plane value);
		
		//Transforms a Matrix by applying a Quaternion rotation.
		static constexpr Matrix Transform(Matrix const& value, Quaternion const& rotation);
		
		//Transposes the rows and columns of a matrix.
		static constexpr Matrix Transpose(Matrix const& matrix) {
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
		constexpr float Determinant() const {
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
		static constexpr Matrix Invert(Matrix const& matrix) {
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
		static constexpr Matrix Lerp(Matrix const& matrix1, Matrix const& matrix2, float amount) {
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
		static constexpr Matrix Negate(Matrix const& matrix) {
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
		static constexpr Matrix Add(Matrix const& matrix1, Matrix const& matrix2) {
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
		static constexpr Matrix Subtract(Matrix const& matrix1, Matrix const& matrix2) {
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
		static constexpr Matrix Multiply(Matrix const& matrix1, Matrix const& matrix2) {
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
		static constexpr Matrix Multiply(Matrix const& matrix1, float scaleFactor) {
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
		static constexpr Matrix Divide(Matrix const& matrix1, Matrix const& matrix2) {
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
		static constexpr Matrix Divide(Matrix const& matrix1, float divider) {
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

		constexpr Matrix operator-() const {
			return Matrix::Negate(*this);
		}

		friend constexpr Matrix operator+(Matrix const& matrix1, Matrix const& matrix2) {
			return Matrix::Add(matrix1, matrix2);
		}

		friend constexpr Matrix operator-(Matrix const& matrix1, Matrix const& matrix2) {
			return Matrix::Subtract(matrix1, matrix2);
		}

		friend constexpr Matrix operator*(Matrix const& matrix1, Matrix const& matrix2) {
			return Matrix::Multiply(matrix1, matrix2);
		}

		friend constexpr Matrix operator*(Matrix const& matrix, float factor) {
			return Matrix::Multiply(matrix, factor);
		}

		friend constexpr Matrix operator*(float factor, Matrix const& matrix) {
			return Matrix::Multiply(matrix, factor);
		}

		friend constexpr Matrix operator/(Matrix const& matrix1, Matrix const& matrix2) {
			return Matrix::Divide(matrix1, matrix2);
		}

		friend constexpr Matrix operator/(Matrix const& matrix, float divider) {
			return Matrix::Divide(matrix, divider);
		}

		constexpr bool operator==(const Matrix& other) const {
			return M11 == other.M11 && M12 == other.M12 && M13 == other.M13 && M14 == other.M14
				&& M21 == other.M21 && M22 == other.M22 && M23 == other.M23 && M24 == other.M24
				&& M31 == other.M31 && M32 == other.M32 && M33 == other.M33 && M34 == other.M34
				&& M41 == other.M41 && M42 == other.M42 && M43 == other.M43 && M44 == other.M44;
		}

		constexpr operator std::optional<Matrix>() const {
			return std::make_optional<Matrix>(
				M11, M12, M13, M14,
				M21, M22, M23, M24,
				M31, M32, M33, M34,
				M41, M42, M43, M44);
		}
	};

	constexpr Vector2 Vector2::Transform(Vector2 const& position, Matrix const& matrix) {
		const auto posx = (position.X * matrix.M11 + position.Y * matrix.M21) + matrix.M41;
		const auto posy = (position.X * matrix.M12 + position.Y * matrix.M22) + matrix.M42;

		return{ posx, posy };
	}

	constexpr Vector2 Vector2::TransformNormal(Vector2 const& normal, Matrix const& matrix) {
		const auto posx = normal.X * matrix.M11 + normal.Y * matrix.M21;
		const auto posy = normal.X * matrix.M12 + normal.Y * matrix.M22;
		return { posx, posy };
	}

	constexpr Vector3 Vector3::Transform(Vector3 const& position, Matrix const& matrix) {
		const auto num1 = (position.X * matrix.M11 + position.Y * matrix.M21 + position.Z * matrix.M31) + matrix.M41;
		const auto num2 = (position.X * matrix.M12 + position.Y * matrix.M22 + position.Z * matrix.M32) + matrix.M42;
		const auto num3 = (position.X * matrix.M13 + position.Y * matrix.M23 + position.Z * matrix.M33) + matrix.M43;
		Vector3 vector3;
		vector3.X = num1;
		vector3.Y = num2;
		vector3.Z = num3;
		return vector3;
	}

	constexpr Vector3 Vector3::TransformNormal(Vector3 const& normal, Matrix const& matrix)
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

	constexpr Vector4 Vector4::Transform(Vector2 const& position, Matrix const& matrix)
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

	constexpr Vector4 Vector4::Transform(Vector3 const& position, Matrix const& matrix)
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

	constexpr Vector4 Vector4::Transform(Vector4 const& vector, Matrix const& matrix) {
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
}

#endif