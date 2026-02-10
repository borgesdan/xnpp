#ifndef XNA_FRAMEWORK_GRAPHICS__EFFECTANNOTATION_HPP
#define XNA_FRAMEWORK_GRAPHICS__EFFECTANNOTATION_HPP

#include <string>
#include <memory>
#include <cstdint>
#include "Shared.hpp"
#include "Xna/Framework/Vector4.hpp"
#include "Xna/Framework/Matrix.hpp"
#include "Xna/Internal/Export.hpp"

namespace Xna {
	class EffectAnnotation final {
	public:
		inline std::string Name() const;
		inline std::string Semantic() const;
		inline size_t RowCount() const;
		inline size_t ColumnCount() const;
		inline EffectParameterClass ParameterClass() const;
		inline EffectParameterType ParameterType() const;
		XNPP_API bool GetValueBoolean();
		XNPP_API int32_t GetValueInt32();
		XNPP_API float GetValueSingle();
		XNPP_API Vector2 GetValueVector2();
		XNPP_API Vector3 GetValueVector3();
		XNPP_API Vector4 GetValueVector4();
		XNPP_API Matrix GetValueMatrix(); 
		XNPP_API std::string GetValueString();

		inline EffectAnnotation(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(EffectAnnotation const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		XNPP_API EffectAnnotation(EffectParameterTypeVariant const& variant);

		struct Implementation;
		std::shared_ptr<Implementation> impl;
	};
}

#endif