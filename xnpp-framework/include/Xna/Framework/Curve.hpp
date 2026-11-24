#pragma once

#include "MathHelper.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <vector>

namespace Xna {
	//////////////////////////////////////////////////////////////////
	// ENUMS														//
	//////////////////////////////////////////////////////////////////

	//Specifies different tangent types to be calculated for CurveKey points in a Curve. 
	enum class CurveTangent {
		//A Flat tangent always has a value equal to zero
		Flat,
		//A Linear tangent at a CurveKey is equal to the difference between its Value and the Value of the preceding or succeeding CurveKey.
		Linear,
		//A Smooth tangent smooths the inflection between a TangentIn and TangentOut by taking into account the values of both neighbors of the CurveKey.
		Smooth,
	};

	//Defines how the value of a Curve will be determined for positions before the first point on the Curve or after the last point on the Curve. 
	enum class CurveLoopType {
		//The Curve will evaluate to its first key for positions before the first point in the Curve and to the last key for positions after the last point.
		Constant,
		//Positions specified past the ends of the curve will wrap around to the opposite side of the Curve.
		Cycle,
		//Positions specified past the ends of the curve will wrap around to the opposite side of the Curve. The value will be offset by the difference between the values of the first and last CurveKey multiplied by the number of times the position wraps around. If the position is before the first point in the Curve, the difference will be subtracted from its value; otherwise, the difference will be added. 
		CycleOffset,
		//Positions specified past the ends of the Curve act as an offset from the same side of the Curve toward the opposite side.
		Oscillate,
		//Linear interpolation will be performed to determine the value.
		Linear,
	};

	//Defines the continuity of CurveKeys on a Curve. 
	enum class CurveContinuity {
		//Interpolation can be used between this CurveKey and the next.
		Smooth,
		//Interpolation cannot be used between this CurveKey and the next. Specifying a position between the two points returns this point.
		Step,
	};

	//////////////////////////////////////////////////////////////////
	// CURVE KEY													//
	//////////////////////////////////////////////////////////////////

	//Represents a point in a multi-point curve.
	struct CurveKey final {
		//Initializes a new instance of CurveKey.
		constexpr CurveKey() noexcept = default;

		//Initializes a new instance of CurveKey.
		constexpr CurveKey(CurveKey const&) noexcept = default;

		//Initializes a new instance of CurveKey.
		constexpr CurveKey(float position, float value, float tangentIn, float tangentOut, CurveContinuity continuity) noexcept :
			Position(position), Value(value), TangentOut(tangentOut), TangentIn(tangentIn), Continuity(continuity) {}

		//Position of the CurveKey in the curve.
		float Position{ 0.0f };
		//Describes the value of this point.
		float Value{ 0.0f };
		//Describes the tangent when leaving this point to the next point in the curve.
		float TangentOut{ 0.0f };
		//Describes the tangent when approaching this point from the previous point in the curve
		float TangentIn{ 0.0f };
		//Describes whether the segment between this point and the next point in the curve is discrete or continuous.
		CurveContinuity Continuity{ CurveContinuity::Smooth };

		[[nodiscard]] constexpr bool operator==(const CurveKey& other) const noexcept = default;

		[[nodiscard]] constexpr bool operator<(CurveKey const& other) const noexcept {
			return Position < other.Position;
		}
	};

	//Contains the CurveKeys making up a Curve. 
	struct CurveKeyCollection final {
		//Initializes a new instance of CurveKey.
		constexpr CurveKeyCollection() noexcept = default;

		//Determines the index of a CurveKey in the CurveKeyCollection.
		[[nodiscard]] constexpr int IndexOf(CurveKey const& item) const noexcept {
			if (keys.empty())
				return -1;

			auto it = std::find(keys.begin(), keys.end(), item);

			if (it == keys.end())
				return -1;

			auto value = std::distance(keys.begin(), it);

			return static_cast<int>(value);
		}

		//Removes the CurveKey at the specified index.
		constexpr void RemoveAt(size_t index) noexcept {
			if (keys.empty())
				return;

			if (index >= keys.size())
				index = keys.size() - 1;

			keys.erase(keys.begin() + index);
			isCachedAvailable = false;
		}

		//Adds a CurveKey to the CurveKeyCollection.
		constexpr void Add(CurveKey const& item) {
			auto it = std::lower_bound(
				keys.begin(),
				keys.end(),
				item,
				[](CurveKey const& a, CurveKey const& b) {
					return a.Position < b.Position;
				});

			while (it != keys.end() && (*it).Position == item.Position) {
				++it;
			}

			keys.insert(it, item);

			isCachedAvailable = false;
		}

		//Removes all CurveKeys from the CurveKeyCollection.
		constexpr void Clear() noexcept {
			keys.clear();
			timeRange = 0.0f;
			invTimeRange = 0.0f;
			isCachedAvailable = false;
		}

		//Determines whether the CurveKeyCollection contains a specific CurveKey.
		[[nodiscard]] constexpr bool Contains(CurveKey const& item) const noexcept {
			if (keys.empty())
				return false;

			return std::find(keys.begin(), keys.end(), item) != keys.end();
		}

		//Gets the number of elements contained in the CurveKeyCollection.
		[[nodiscard]] constexpr size_t Count() const noexcept { return keys.size(); }

		//Removes the first occurrence of a specific CurveKey from the CurveKeyCollection.
		constexpr bool Remove(CurveKey const& item) noexcept {
			if (keys.empty())
				return false;

			auto it = std::find(keys.begin(), keys.end(), item);

			if (it == keys.end())
				return false;

			keys.erase(it);
			isCachedAvailable = false;
			return true;
		}

		//Gets or sets the element at the specified index.
		[[nodiscard]] constexpr CurveKey* operator[](size_t index) noexcept {
			if (keys.empty())
				return nullptr;

			if (index >= keys.size())
				index = keys.size() - 1;

			return &keys[index];
		}

		//Gets the element at the specified index.
		[[nodiscard]] constexpr const CurveKey* operator[](size_t index) const noexcept {
			if (keys.empty())
				return nullptr;

			if (index >= keys.size())
				index = keys.size() - 1;

			return &keys[index];
		}

		[[nodiscard]] constexpr bool IsCachedAvailable() const noexcept { return isCachedAvailable; }
		[[nodiscard]] constexpr float TimeRange() const noexcept { return timeRange; }
		[[nodiscard]] constexpr float InvTimeRange() const noexcept { return invTimeRange; }

		constexpr void ComputeCacheValues() const noexcept {
			timeRange = 0.0f;
			invTimeRange = 0.0f;

			if (!keys.empty()) {
				timeRange = keys[keys.size() - 1].Position - keys[0].Position;

				if (timeRange > std::numeric_limits<float>::denorm_min())
					invTimeRange = 1.0f / timeRange;
			}

			isCachedAvailable = true;
		}

	private:
		std::vector<CurveKey> keys;

		mutable float timeRange{ 0.0f };
		mutable float invTimeRange{ 0.0f };
		mutable bool isCachedAvailable{ true };
	};

	//////////////////////////////////////////////////////////////////
	// CURVE														//
	//////////////////////////////////////////////////////////////////

	//Stores an arbitrary collection of 2D CurveKey points, and provides methods for evaluating features of the curve they define.
	struct Curve final {
		//Specifies how to handle weighting values that are less than the first control point in the curve.
		CurveLoopType PreLoop{ CurveLoopType::Constant };
		//Specifies how to handle weighting values that are greater than the last control point in the curve.
		CurveLoopType PostLoop{ CurveLoopType::Constant };
		//The points that make up the curve.
		CurveKeyCollection Keys{};

		//Initializes a new instance of Curve.
		constexpr Curve() noexcept = default;

		//Gets a value indicating whether the curve is constant.
		[[nodiscard]] constexpr bool IsConstant() const noexcept { return Keys.Count() <= 1; }

		//Computes all tangents for all CurveKeys in the Curve. 
		constexpr void ComputeTangents(CurveTangent tangentType) noexcept {
			ComputeTangents(tangentType, tangentType);
		}

		//Computes all tangents for all CurveKeys in the Curve. 
		constexpr void ComputeTangents(CurveTangent tangentInType, CurveTangent tangentOutType) noexcept {
			for (size_t keyIndex = 0; keyIndex < Keys.Count(); ++keyIndex)
				ComputeTangent(keyIndex, tangentInType, tangentOutType);
		}

		//Finds the value at a position on the Curve.
		[[nodiscard]] constexpr float Evaluate(float position) const noexcept {
			if (Keys.Count() == 0)
				return 0.0f;
			if (Keys.Count() == 1)
				return Keys[0]->Value;

			const auto* key1 = Keys[0];
			const auto* key2 = Keys[Keys.Count() - 1];
			float t = position;
			float num1 = 0.0f;

			if (t < key1->Position) {
				if (PreLoop == CurveLoopType::Constant)
					return key1->Value;

				if (PreLoop == CurveLoopType::Linear)
					return key1->Value - key1->TangentIn * (key1->Position - t);

				if (!Keys.IsCachedAvailable())
					Keys.ComputeCacheValues();

				float num2 = CalcCycle(t);
				float num3 = t - (key1->Position + num2 * Keys.TimeRange());

				if (PreLoop == CurveLoopType::Cycle) {
					t = key1->Position + num3;
				}
				else if (PreLoop == CurveLoopType::CycleOffset) {
					t = key1->Position + num3;
					num1 = (key2->Value - key1->Value) * num2;
				}
				else {
					t = (static_cast<int32_t>(num2) & 1) != 0 ? key2->Position - num3 : key1->Position + num3;
				}
			}
			else if (key2->Position < t) {
				if (PostLoop == CurveLoopType::Constant)
					return key2->Value;

				if (PostLoop == CurveLoopType::Linear)
					return key2->Value - key2->TangentOut * (key2->Position - t);

				if (!Keys.IsCachedAvailable())
					Keys.ComputeCacheValues();

				float num4 = CalcCycle(t);
				float num5 = t - (key1->Position + num4 * Keys.TimeRange());

				if (PostLoop == CurveLoopType::Cycle) {
					t = key1->Position + num5;
				}
				else if (PostLoop == CurveLoopType::CycleOffset) {
					t = key1->Position + num5;
					num1 = (key2->Value - key1->Value) * num4;
				}
				else {
					t = (static_cast<int32_t>(num4) & 1) != 0 ? key2->Position - num5 : key1->Position + num5;
				}
			}

			CurveKey k0{};
			CurveKey k1{};
			float segment = FindSegment(t, k0, k1);
			return num1 + Curve::Hermite(k0, k1, segment);
		}

	private:
		constexpr void ComputeTangent(size_t keyIndex, CurveTangent tangentType) noexcept {
			ComputeTangent(keyIndex, tangentType, tangentType);
		}

		constexpr void ComputeTangent(size_t keyIndex, CurveTangent tangentInType, CurveTangent tangentOutType) noexcept {
			if (Keys.Count() <= keyIndex)
				return;

			auto* key = Keys[keyIndex];
			auto num1 = key->Position;
			auto num2 = key->Position;
			auto num3 = key->Position;
			auto num5 = key->Value;
			auto num6 = key->Value;
			auto num7 = key->Value;

			if (keyIndex > 0) {
				num3 = Keys[keyIndex - 1]->Position;
				num7 = Keys[keyIndex - 1]->Value;
			}
			if (keyIndex + 1 < Keys.Count()) {
				num1 = Keys[keyIndex + 1]->Position;
				num5 = Keys[keyIndex + 1]->Value;
			}

			constexpr float eps = std::numeric_limits<float>::epsilon();

			switch (tangentInType) {
			case CurveTangent::Linear: {
				key->TangentIn = num6 - num7;
				break;
			}
			case CurveTangent::Smooth: {
				const auto num8 = num1 - num3;
				const auto num9 = num5 - num7;
				if (MathHelper::Abs(num9) >= eps && num8 > eps)
					key->TangentIn = num9 * MathHelper::Abs(num3 - num2) / num8;
				else
					key->TangentIn = 0.0f;
				break;
			}
			default: {
				key->TangentIn = 0.0f;
				break;
			}
			}

			switch (tangentOutType) {
			case CurveTangent::Linear: {
				key->TangentOut = num5 - num6;
				break;
			}
			case CurveTangent::Smooth: {
				const auto num10 = num1 - num3;
				const auto num11 = num5 - num7;
				if (MathHelper::Abs(num11) >= eps && num10 > eps)
					key->TangentOut = num11 * MathHelper::Abs(num1 - num2) / num10;
				else
					key->TangentOut = 0.0f;
				break;
			}
			default: {
				key->TangentOut = 0.0f;
				break;
			}
			}
		}

		[[nodiscard]] constexpr float CalcCycle(float t) const noexcept {
			const auto num = (t - Keys[0]->Position) * Keys.InvTimeRange();
			auto cycle = static_cast<float>(static_cast<int32_t>(num));

			if (num < 0.0f)
				--cycle;

			return cycle;
		}

		[[nodiscard]] constexpr float FindSegment(float t, CurveKey& k0, CurveKey& k1) const noexcept {
			float segment = t;
			k0 = *Keys[0];

			for (size_t index = 1; index < Keys.Count(); ++index) {
				k1 = *Keys[index];
				if (k1.Position >= t) {
					const auto position1 = k0.Position;
					const auto position2 = k1.Position;
					const auto num2 = position2 - position1;
					segment = 0.0f;
					if (num2 > 1e-10f)
						segment = (t - position1) / num2;
					break;
				}
				k0 = k1;
			}
			return segment;
		}

		[[nodiscard]] static constexpr float Hermite(CurveKey const& k0, CurveKey const& k1, float t) noexcept {
			if (k0.Continuity == CurveContinuity::Step)
				return t >= 1.0f ? k1.Value : k0.Value;

			const auto num1 = t * t;
			const auto num2 = num1 * t;
			const auto internalValue1 = k0.Value;
			const auto internalValue2 = k1.Value;
			const auto tangentOut = k0.TangentOut;
			const auto tangentIn = k1.TangentIn;
			return (internalValue1 * (2.0f * num2 - 3.0f * num1 + 1.0f)
				+ internalValue2 * (-2.0f * num2 + 3.0f * num1)
				+ tangentOut * (num2 - 2.0f * num1 + t) + tangentIn * (num2 - num1));
		}
	};
}
