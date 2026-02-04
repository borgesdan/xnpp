#include "Xna/Framework/Graphics/Effect/EffectAnnotation.hpp"

namespace Xna {
	bool EffectAnnotation::GetValueBoolean() {
		if (auto v = std::get_if<bool>(&impl->variant))
			return *v;
		
		throw CSharp::InvalidOperationException();
		return false;
	}

	int32_t EffectAnnotation::GetValueInt32() {
		if (auto v = std::get_if<int32_t>(&impl->variant))
			return *v;
		
		throw CSharp::InvalidOperationException();
		return 0;
	}

	float EffectAnnotation::GetValueSingle() {
		if (auto v = std::get_if<float>(&impl->variant))
			return *v;
		
		throw CSharp::InvalidOperationException();
		return 0;
	}

	Vector2 EffectAnnotation::GetValueVector2() {
		if (auto v = std::get_if<Vector2>(&impl->variant))
			return *v;
		
		throw CSharp::InvalidOperationException();
		return {};
	}

	Vector3 EffectAnnotation::GetValueVector3() {
		if (auto v = std::get_if<Vector3>(&impl->variant))
			return *v;
		
		throw CSharp::InvalidOperationException();
		return {};
	}

	Vector4 EffectAnnotation::GetValueVector4() {
		if (auto v = std::get_if<Vector4>(&impl->variant))
			return *v;
		
		throw CSharp::InvalidOperationException();
		return {};
	}

	Matrix EffectAnnotation::GetValueMatrix() {
		if (auto v = std::get_if<Matrix>(&impl->variant))
			return *v;
		
		throw CSharp::InvalidOperationException();
		return {};
	}

	std::string EffectAnnotation::GetValueString() {
		if (auto v = std::get_if<std::string>(&impl->variant))
			return *v;
		
		throw CSharp::InvalidOperationException();
		return {};
	}
}