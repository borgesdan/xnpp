#ifndef XNA_FRAMEWORK_MATHHELPER_HPP
#define XNA_FRAMEWORK_MATHHELPER_HPP

#include <limits>
#include <cmath>
#include <algorithm>

namespace Xna {
	//Contains commonly used precalculated values. 
	struct MathHelper final {
		MathHelper() = delete;
		MathHelper(MathHelper&) = delete;
		MathHelper(MathHelper&&) = delete;

		//Represents the mathematical constant e.
		static constexpr double E = 2.7182818284590451;
		//Represents the value of pi.
		static constexpr double PI = 3.1415926535897931;

		//Converts degrees to radians.
		static constexpr float ToRadians(float degrees) { return degrees * (static_cast<float>(PI) / 180.0f); }
		//Converts radians to degrees.
		static constexpr float ToDegrees(float radians) { return radians * 57.2957764F; }
		//Calculates the absolute value of the difference of two values
		static float Distance(float value1, float value2) { return std::abs(value1 - value2); }
		//Returns the lesser of two values.
		static constexpr float Min(float value1, float value2) { return (std::min)(value1, value2); }
		//Returns the greater of two values.
		static constexpr float Max(float value1, float value2) { return (std::max)(value1, value2); }

		//Restricts a value to be within a specified range.
		template <typename T>
		static constexpr T Clamp(T value, T min, T max) {
			value = value > max ? max : value;
			value = value < min ? min : value;
			return value;
		}

		//Linearly interpolates between two values.
		static constexpr float Lerp(float value1, float value2, float amount) {
			return value1 + (value2 - value1) * amount;
		}

		//Returns the Cartesian coordinate for one axis of a point that is defined by a given triangle and two normalized barycentric (areal) coordinates.
		static constexpr float Barycentric(float value1, float value2, float value3, float amount1, float amount2) {
			return value1 + amount1 * (value2 - value1) + amount2 * (value3 - value1);
		}

		//Interpolates between two values using a cubic equation.
		static constexpr float SmoothStep(float value1, float value2, float amount) {
			const auto num = Clamp(amount, 0.0f, 1.0f);
			return Lerp(value1, value2, (num * num * (3.0F - 2.0F * num)));
		}

		//Performs a Catmull-Rom interpolation using the specified positions.
		static constexpr float CatmullRom(float value1, float value2, float value3, float value4, float amount) {
			const auto num1 = amount * amount;
			const auto num2 = amount * num1;
			return (0.5F * (2.0F * value2 + (-value1 + value3) * amount + (2.0F * value1 - 5.0F * value2 + 4.0F * value3 - value4) * num1 + (-value1 + 3.0F * value2 - 3.0F * value3 + value4) * num2));
		}

		//Performs a Hermite spline interpolation.
		static constexpr float Hermite(float value1, float tangent1, float value2, float tangent2, float amount) {
			const auto num1 = amount;
			const auto num2 = num1 * num1;
			const auto num3 = num1 * num2;
			const auto num4 = (2.0F * num3 - 3.0F * num2 + 1.0F);
			const auto num5 = (-2.0F * num3 + 3.0F * num2);
			const auto num6 = num3 - 2.0f * num2 + num1;
			const auto num7 = num3 - num2;
			return value1 * num4 + value2 * num5 + tangent1 * num6 + tangent2 * num7;
		}

		struct Extensions {
			template<typename T>
			static constexpr T Sqrt(T x) {
				if (std::is_constant_evaluated())
					return internalSqrt(x);
				else
					return std::sqrt(x);
			}

			template<typename T>
			static constexpr T Abs(T x) {
				if (std::is_constant_evaluated())
					return internalAbs(x);
				else
					return std::abs(x);
			}


			static constexpr float Sin(float x) {
				if (std::is_constant_evaluated())
					return internalSin(x);
				else
					return std::sin(x);
			}

			static constexpr double Sin(double x) {
				if (std::is_constant_evaluated())
					return internalSin(x);
				else
					return std::sin(x);
			}

			static constexpr float Cos(float x) {
				if (std::is_constant_evaluated())
					return internalCos(x);
				else
					return std::cos(x);
			}

			static constexpr double Cos(double x) {
				if (std::is_constant_evaluated())
					return internalCos(x);
				else
					return std::cos(x);
			}

		private:
			template<typename T> static constexpr T internalSqrt(T x) {
				if (x <= T(0))
					return T(0);

				if (x != x) // NaN check
					return T(0);

				T result = x;

				for (size_t i = 0; i < 25; ++i)
					result = (result + x / result) / T(2);

				return result;
			}

			template<typename T> static constexpr T internalAbs(T x) {
				if (x != x) // NaN check
					return T(0);

				return x < T(0) ? -x : x;
			}

			template <typename T> static constexpr T internalSin(T x) {
				// contrato forte
				if (x != x) // NaN check
					return T(0);

				const auto pi = static_cast<T>(PI);

				while (x > pi) x -= pi * 2;
				while (x < -pi) x += pi * 2;

				T term = x;        // primeiro termo
				T sum = x;
				T x2 = x * x;

				// Série de Taylor: sin(x) = x - x^3/3! + x^5/5! - ...
				for (size_t i = 1; i <= 10; ++i)
				{
					term *= -x2 / ((2 * i) * (2 * i + 1));
					sum += term;
				}

				return sum;
			}

			template <typename T> static constexpr T internalCos(T x)
			{
				// contrato forte: evita NaN em constexpr
				if (x != x)
					return 1.0;

				const auto pi = static_cast<T>(PI);

				while (x > pi) x -= pi * 2;
				while (x < -pi) x += pi * 2;


				T term = 1.0;  // primeiro termo
				T sum = 1.0;
				T x2 = x * x;

				// Série de Taylor:
				// cos(x) = 1 - x^2/2! + x^4/4! - ...
				for (size_t i = 1; i <= 10; ++i)
				{
					term *= -x2 / ((2 * i - 1) * (2 * i));
					sum += term;
				}

				return sum;
			}
		};
	};
}

#endif