#ifndef XNA_FRAMEWORK_GRAPHICS__EFFECTANNOTATION_HPP
#define XNA_FRAMEWORK_GRAPHICS__EFFECTANNOTATION_HPP

#include <string>
#include <memory>
#include <cstdint>
#include "Shared.hpp"
#include "Xna/Framework/Vector4.hpp"
#include "Xna/Framework/Matrix.hpp"

namespace Xna {
	class EffectAnnotation final {
	public:
		std::string Name() const;
		std::string Semantic() const;
		size_t RowCount() const;
		size_t ColumnCount() const;
		EffectParameterClass ParameterClass() const;
		EffectParameterType ParameterType() const;
		bool GetValueBoolean();
		int32_t GetValueInt32();
		float GetValueSingle();
		Vector2 GetValueVector2();
		Vector3 GetValueVector3();
		Vector4 GetValueVector4();
		Matrix GetValueMatrix(); 
		std::string GetValueString();

		inline EffectAnnotation(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(EffectAnnotation const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		EffectAnnotation(EffectParameterTypeVariant const& variant);

		struct Implementation;
		std::shared_ptr<Implementation> impl;
	};
}

#endif