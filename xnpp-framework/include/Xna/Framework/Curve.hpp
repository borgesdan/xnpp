#ifndef XNA_FRAMEWORK_CURVE_HPP
#define XNA_FRAMEWORK_CURVE_HPP

#include <vector>
#include <algorithm>
#include "Shared.hpp"
#include "CurveKeyCollection.hpp"

namespace Xna {
	//Stores an arbitrary collection of 2D CurveKey points, and provides methods for evaluating features of the curve they define.
	struct Curve final {
		//Specifies how to handle weighting values that are less than the first control point in the curve.
		CurveLoopType PreLoop{ CurveLoopType::Constant };
		//Specifies how to handle weighting values that are greater than the last control point in the curve.
		CurveLoopType PostLoop{ CurveLoopType::Constant };
		//The points that make up the curve.
		CurveKeyCollection Keys{};

		//Initializes a new instance of Curve.
		constexpr Curve() = default;

		//Gets a value indicating whether the curve is constant.
		constexpr bool IsConstant() const { return Keys.Count() <= 1; }

		//Computes all tangents for all CurveKeys in the Curve. 
		constexpr void ComputeTangents(CurveTangent tangentType) {
			ComputeTangents(tangentType, tangentType);
		}

		//Computes all tangents for all CurveKeys in the Curve. 
		constexpr void ComputeTangents(CurveTangent tangentInType, CurveTangent tangentOutType) {
			for (size_t keyIndex = 0; keyIndex < Keys.Count(); ++keyIndex)
				ComputeTangent(keyIndex, tangentInType, tangentOutType);
		}

		//Finds the value at a position on the Curve.
		constexpr float Evaluate(float position) {
			if (Keys.Count() == 0)
				return 0.0f;
			if (Keys.Count() == 1)
				return Keys[0].Value;

			auto& key1 = Keys[0];
			auto& key2 = Keys[Keys.Count() - 1];
			float t = position;
			float num1 = 0.0f;

			if (t < key1.Position)
			{
				if (PreLoop == CurveLoopType::Constant)
					return key1.Value;

				if (PreLoop == CurveLoopType::Linear)
					return key1.Value - key1.TangentIn * (key1.Position - t);

				if (!Keys.IsCachedAvailable)
					Keys.ComputeCacheValues();

				float num2 = CalcCycle(t);
				float num3 = t - (key1.Position + num2 * Keys.TimeRange);

				if (PreLoop == CurveLoopType::Cycle)
					t = key1.Position + num3;

				else if (PreLoop == CurveLoopType::CycleOffset) {
					t = key1.Position + num3;
					num1 = (key2.Value - key1.Value) * num2;
				}
				else {
					t = (static_cast<int32_t>(num2) & 1) != 0 ? key2.Position - num3 : key1.Position + num3;
				}
			}
			else if (key2.Position < t)
			{
				if (PostLoop == CurveLoopType::Constant)
					return key2.Value;

				if (PostLoop == CurveLoopType::Linear)
					return key2.Value - key2.TangentOut * (key2.Position - t);

				if (!Keys.IsCachedAvailable)
					Keys.ComputeCacheValues();

				float num4 = CalcCycle(t);
				float num5 = t - (key1.Position + num4 * Keys.TimeRange);

				if (PostLoop == CurveLoopType::Cycle)
					t = key1.Position + num5;

				else if (PostLoop == CurveLoopType::CycleOffset)
				{
					t = key1.Position + num5;
					num1 = (key2.Value - key1.Value) * num4;
				}
				else {
					t = (static_cast<int32_t>(num4) & 1) != 0 ? key2.Position - num5 : key1.Position + num5;
				}
			}

			CurveKey k0;
			CurveKey k1;
			float segment = FindSegment(t, k0, k1);
			return num1 + Curve::Hermite(k0, k1, segment);
		}

	private:
		constexpr void ComputeTangent(size_t keyIndex, CurveTangent tangentType) {
			ComputeTangent(keyIndex, tangentType, tangentType);
		}

		constexpr void ComputeTangent(size_t keyIndex, CurveTangent tangentInType, CurveTangent tangentOutType) {
			if (Keys.Count() <= keyIndex)
				return;

			auto& key = Keys[keyIndex];
			auto num1 = key.Position;
			auto num2 = key.Position;
			auto num3 = key.Position;
			auto num5 = key.Value;
			auto num6 = key.Value;
			auto num7 = key.Value;

			if (keyIndex > 0)
			{
				num3 = Keys[keyIndex - 1].Position;
				num7 = Keys[keyIndex - 1].Value;
			}
			if (keyIndex + 1 < Keys.Count())
			{
				num1 = Keys[keyIndex + 1].Position;
				num5 = Keys[keyIndex + 1].Value;
			}

			switch (tangentInType)
			{
			case CurveTangent::Linear: {
				key.TangentIn = num6 - num7;
				break;
			}
			case CurveTangent::Smooth: {
				auto num8 = num1 - num3;
				auto num9 = num5 - num7;
				key.TangentIn = std::abs(num9) >= 1.1920928955078125E-07 ? num9 * std::abs(num3 - num2) / num8 : 0.0f;
				break;
			}
			default: {
				key.TangentIn = 0.0f;
				break;
			}
			}

			switch (tangentOutType)
			{
			case CurveTangent::Linear: {
				key.TangentOut = num5 - num6;
				break;
			}
			case CurveTangent::Smooth: {
				auto num10 = num1 - num3;
				auto num11 = num5 - num7;
				if (std::abs(num11) < 1.1920928955078125E-07)
				{
					key.TangentOut = 0.0f;
					break;
				}
				key.TangentOut = num11 * std::abs(num1 - num2) / num10;
				break;
			}
			default: {
				key.TangentOut = 0.0f;
				break;
			}
			}
		}

		constexpr float CalcCycle(float t) {
			auto num = (t - Keys[0].Position) * Keys.InvTimeRange;

			if (num < 0.0)
				--num;

			return static_cast<float>(static_cast<int32_t>(num));
		}

		constexpr float FindSegment(float t, CurveKey& k0, CurveKey& k1) {
			float segment = t;
			k0 = Keys[0];

			for (size_t index = 1; index < Keys.Count(); ++index)
			{
				k1 = Keys[index];
				if (k1.Position >= t)
				{
					const auto position1 = k0.Position;
					const auto position2 = k1.Position;
					const auto num1 = t;
					const auto num2 = position2 - position1;
					segment = 0.0f;
					if (num2 > 1E-10)
					{
						segment = ((num1 - position1) / num2);
						break;
					}
					break;
				}
				k0 = k1;
			}
			return segment;
		}

		static constexpr float Hermite(CurveKey k0, CurveKey k1, float t) {

			if (k0.Continuity == CurveContinuity::Step)
				return t >= 1.0 ? k1.Value : k0.Value;

			const auto num1 = t * t;
			const auto num2 = num1 * t;
			const auto internalValue1 = k0.Value;
			const auto internalValue2 = k1.Value;
			const auto tangentOut = k0.TangentOut;
			const auto tangentIn = k1.TangentIn;
			return (internalValue1 * (2.0F * num2 - 3.0F * num1 + 1.0F) + internalValue2 * (-2.0F * num2 + 3.0F * num1) + tangentOut * (num2 - 2.0F * num1 + t) + tangentIn * (num2 - num1));
		}
	};

}

#endif