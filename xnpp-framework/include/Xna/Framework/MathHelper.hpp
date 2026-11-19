#pragma once

#include <algorithm>
#include <cmath>
#include <concepts>
#include <limits>
#include <type_traits>

namespace Xna {
	// Defines a concept that strictly accepts float or double
	template<typename T>
	concept FloatOrDouble = std::same_as<T, float> || std::same_as<T, double>;

	// Contains commonly used precalculated values and math utilities.
	struct MathHelper final {
		MathHelper() = delete;
		MathHelper(const MathHelper&) = delete;
		MathHelper(MathHelper&&) = delete;
		MathHelper& operator=(const MathHelper&) = delete;
		MathHelper& operator=(MathHelper&&) = delete;

		// Represents the mathematical constant e.
		static constexpr double E = 2.7182818284590451;
		// Represents the value of pi.
		static constexpr double PI = 3.1415926535897931;

		// Converts degrees to radians.
		template <FloatOrDouble T>
		[[nodiscard]] static constexpr T ToRadians(T degrees) noexcept {
			return degrees * static_cast<T>(PI / 180.0);
		}

		// Converts radians to degrees.
		template <FloatOrDouble T>
		[[nodiscard]] static constexpr T ToDegrees(T radians) noexcept {
			return radians * static_cast<T>(180.0 / PI);
		}

		// Calculates the absolute value of the difference of two values.
		template <FloatOrDouble T>
		[[nodiscard]] static constexpr T Distance(T value1, T value2) noexcept {
			return Abs(value1 - value2);
		}

		// Returns the lesser of two values.
		template <FloatOrDouble T>
		[[nodiscard]] static constexpr T Min(T value1, T value2) noexcept {
			return (std::min)(value1, value2);
		}

		// Returns the greater of two values.
		template <FloatOrDouble T>
		[[nodiscard]] static constexpr T Max(T value1, T value2) noexcept {
			return (std::max)(value1, value2);
		}

		// Restricts a value to be within a specified range.
		template <typename T>
		[[nodiscard]] static constexpr T Clamp(T value, T min, T max) noexcept {
			value = value > max ? max : value;
			value = value < min ? min : value;
			return value;
		}

		// Linearly interpolates between two values.
		template <typename T1, FloatOrDouble T2>
		[[nodiscard]] static constexpr T2 Lerp(T1 value1, T1 value2, T2 amount) noexcept {
			return std::lerp(static_cast<T2>(value1), static_cast<T2>(value2), amount);
		}

		// Returns the Cartesian coordinate for one axis of a point defined by a triangle and two barycentric coordinates.
		template <typename T>
		[[nodiscard]] static constexpr T Barycentric(T value1, T value2, T value3, T amount1, T amount2) noexcept {
			return value1 + amount1 * (value2 - value1) + amount2 * (value3 - value1);
		}

		// Interpolates between two values using a cubic equation.
		template <typename T>
		[[nodiscard]] static constexpr T SmoothStep(T value1, T value2, T amount) noexcept {
			const auto num = Clamp(amount, T(0.0), T(1.0));
			return Lerp(value1, value2, (num * num * (T(3.0) - T(2.0) * num)));
		}

		// Performs a Catmull-Rom interpolation using the specified positions.
		template <typename T>
		[[nodiscard]] static constexpr T CatmullRom(T value1, T value2, T value3, T value4, T amount) noexcept {
			const auto num1 = amount * amount;
			const auto num2 = amount * num1;
			return (T(0.5) * (T(2.0) * value2 + (-value1 + value3) * amount +
				(T(2.0) * value1 - T(5.0) * value2 + T(4.0) * value3 - value4) * num1 +
				(-value1 + T(3.0) * value2 - T(3.0) * value3 + value4) * num2));
		}

		// Performs a Hermite spline interpolation.
		template <typename T>
		[[nodiscard]] static constexpr T Hermite(T value1, T tangent1, T value2, T tangent2, T amount) noexcept {
			const auto num1 = amount;
			const auto num2 = num1 * num1;
			const auto num3 = num1 * num2;
			const auto num4 = (T(2.0) * num3 - T(3.0) * num2 + T(1.0));
			const auto num5 = (T(-2.0) * num3 + T(3.0) * num2);
			const auto num6 = num3 - T(2.0) * num2 + num1;
			const auto num7 = num3 - num2;
			return value1 * num4 + value2 * num5 + tangent1 * num6 + tangent2 * num7;
		}

		template<FloatOrDouble T>
		[[nodiscard]] static constexpr T Sqrt(T x) noexcept {
			if (std::is_constant_evaluated())
				return internalSqrt(x);
			else
				return std::sqrt(x);
		}

		template<FloatOrDouble T>
		[[nodiscard]] static constexpr T Abs(T x) noexcept {
			if (std::is_constant_evaluated())
				return internalAbs(x);
			else
				return std::abs(x);
		}

		template<FloatOrDouble T>
		[[nodiscard]] static constexpr T Sin(T x) noexcept {
			if (std::is_constant_evaluated())
				return internalSin(x);
			else
				return std::sin(x);
		}

		template<FloatOrDouble T>
		[[nodiscard]] static constexpr T Cos(T x) noexcept {
			if (std::is_constant_evaluated())
				return internalCos(x);
			else
				return std::cos(x);
		}

		template<FloatOrDouble T>
		[[nodiscard]] static constexpr T Tan(T x) noexcept {
			if (std::is_constant_evaluated())
				return internalTan(x);
			else
				return std::tan(x);
		}

		template<FloatOrDouble T>
		[[nodiscard]] static constexpr T Acos(T x) noexcept {
			if (std::is_constant_evaluated())
				return internalAcos(x);
			else
				return std::acos(x);
		}

	private:
		//Replaces an angle-reduction `while` loop with an O(1) reduction, to avoid hitting the compiler's iteration limit for large angles.
		template<typename T>
		static constexpr T reduceAngle(T x) noexcept {
			constexpr T pi = static_cast<T>(PI);
			constexpr T twoPi = static_cast<T>(PI * 2.0);

			x -= static_cast<long long>(x / twoPi) * twoPi;
			if (x > pi)
				x -= twoPi;
			else if (x < -pi)
				x += twoPi;

			return x;
		}

		//It seeks to perform scale normalization, ensuring maximum precision for both float and double types.
		template<typename T>
		static constexpr T internalSqrt(T x) noexcept {
			if (x <= T(0) || x != x)
				return T(0);

			T scale = T(1);
			while (x > T(1)) {
				x *= T(0.25);
				scale *= T(2);
			}
			while (x < T(0.25)) {
				x *= T(4);
				scale *= T(0.5);
			}

			T result = x;
			for (size_t i = 0; i < 10; ++i)
				result = (result + x / result) * T(0.5);

			return result * scale;
		}

		template<typename T>
		static constexpr T internalAbs(T x) noexcept {
			if (x != x)
				return T(0);

			return x < T(0) ? -x : x;
		}

		template<typename T>
		static constexpr T internalSin(T x) noexcept {
			if (x != x)
				return T(0);

			x = reduceAngle(x);

			T term = x;
			T sum = x;
			const T x2 = x * x;

			// Série de Taylor: sin(x) = x - x^3/3! + x^5/5! - ...
			for (size_t i = 1; i <= 10; ++i) {
				term *= -x2 / static_cast<T>((2 * i) * (2 * i + 1));
				sum += term;
			}

			return sum;
		}

		template<typename T>
		static constexpr T internalCos(T x) noexcept {
			if (x != x)
				return T(1.0);

			x = reduceAngle(x);

			T term = T(1.0);
			T sum = T(1.0);
			const T x2 = x * x;

			// Série de Taylor: cos(x) = 1 - x^2/2! + x^4/4! - ...
			for (size_t i = 1; i <= 10; ++i) {
				term *= -x2 / static_cast<T>((2 * i - 1) * (2 * i));
				sum += term;
			}

			return sum;
		}

		template<typename T>
		static constexpr void internalSinCos(T x, T& sinOut, T& cosOut) noexcept {
			if (x != x) {
				sinOut = T(0);
				cosOut = T(1);
				return;
			}

			x = reduceAngle(x);

			const T x2 = x * x;
			T sinTerm = x;
			T cosTerm = T(1);
			T sinSum = x;
			T cosSum = T(1);

			for (size_t i = 1; i <= 10; ++i) {
				sinTerm *= -x2 / static_cast<T>((2 * i) * (2 * i + 1));
				sinSum += sinTerm;

				cosTerm *= -x2 / static_cast<T>((2 * i - 1) * (2 * i));
				cosSum += cosTerm;
			}

			sinOut = sinSum;
			cosOut = cosSum;
		}

		template<typename T>
		static constexpr T internalTan(T x) noexcept {
			T s{}, c{};
			internalSinCos(x, s, c);

			constexpr T epsilon = T(1e-12);

			if (c > -epsilon && c < epsilon)
				return (s >= T(0))
				? std::numeric_limits<T>::infinity()
				: -std::numeric_limits<T>::infinity();

			return s / c;
		}

		template<typename T>
		static constexpr T internalAsinSeries(T y) noexcept {
			const T y2 = y * y;
			T term = y;
			T sum = y;

			for (size_t i = 1; i <= 20; ++i) {
				const auto num = static_cast<T>((2 * i - 1) * (2 * i - 1));
				const auto den = static_cast<T>((2 * i) * (2 * i + 1));
				term *= y2 * (num / den);
				sum += term;
			}

			return sum;
		}

		template<typename T>
		static constexpr T internalAcos(T x) noexcept {
			if (x != x || x >= T(1))
				return T(0);
			if (x <= T(-1))
				return static_cast<T>(PI);

			const bool isNegative = x < T(0);
			const T ax = isNegative ? -x : x;
			constexpr T halfPi = static_cast<T>(PI * 0.5);

			T result{};
			if (ax > T(0.5)) {
				const T y = internalSqrt((T(1) - ax) * T(0.5));
				result = T(2) * internalAsinSeries(y);
			}
			else {
				result = halfPi - internalAsinSeries(ax);
			}

			return isNegative ? (static_cast<T>(PI) - result) : result;
		}
	};
}