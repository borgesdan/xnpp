#ifndef XNA_FRAMEWORK_GRAPHICS__EFFECTPARAMETER_HPP
#define XNA_FRAMEWORK_GRAPHICS__EFFECTPARAMETER_HPP

#include "Shared.hpp"
#include <string>
#include <vector>
#include <cstdint>
#include "../Texture.hpp"
#include "../Texture2D.hpp"
#include "../Texture3D.hpp"
#include "../TextureCube.hpp"
#include "Xna/Framework/Matrix.hpp"
#include "Xna/Framework/Quaternion.hpp"
#include "Xna/Framework/Vector4.hpp"

namespace Xna {
	struct EffectAnnotationCollection;
	struct EffectParameterCollection;

	//Represents an Effect parameter.
	class EffectParameter final {
	public:
		EffectAnnotationCollection Annotations() const;
		size_t RowCount() const;
		size_t ColumnCount() const;
		EffectParameterCollection StructureMembers() const;
		EffectParameterCollection Elements() const;
		EffectParameterClass ParameterClass() const;
		EffectParameterType ParameterType() const;

		void SetValue(Texture const& value);
		void SetValue(std::string const& value);
		void SetValue(std::vector<Matrix> const& value);
		void SetValue(Matrix const& value);
		void SetValue(std::vector<Quaternion> const& value);
		void SetValue(Quaternion const& value);
		void SetValue(std::vector<Vector4> const& value);
		void SetValue(Vector4 const& value);
		void SetValue(std::vector<Vector3> const& value);
		void SetValue(Vector3 const& value);
		void SetValue(std::vector<Vector2> const& value);
		void SetValue(Vector2 const& value);
		void SetValue(std::vector<float> const& value);
		void SetValue(float const& value);
		void SetValue(std::vector<int32_t> const& value);
		void SetValue(int32_t const& value);
		void SetValue(std::vector<bool> const& value);
		void SetValue(bool const& value);

		void SetValueTranspose(std::vector<Matrix> const& value);
		void SetValueTranspose(Matrix const& value);

		bool GetValueBoolean() const;
		std::vector<bool> GetValueBooleanArray(size_t count) const;
		int32_t GetValueInt32() const;
		std::vector<int32_t> GetValueInt32Array(size_t count) const;
		float GetValueSingle() const;
		std::vector<float> GetValueSingleArray(size_t count) const;
		Vector2 GetValueVector2() const;
		std::vector<Vector2> GetValueVector2Array(size_t count) const;
		Vector3 GetValueVector3() const;
		std::vector<Vector3> GetValueVector3Array(size_t count) const;
		Vector4 GetValueVector4() const;
		std::vector<Vector4> GetValueVector4Array(size_t count) const;
		Quaternion GetValueQuaternion() const;
		std::vector<Quaternion> GetValueQuaternionArray(size_t count) const;
		Matrix GetValueMatrix() const;
		std::vector<Matrix> GetValueMatrixArray(size_t count) const;
		Matrix GetValueMatrixTranspose() const;
		std::vector<Matrix> GetValueMatrixTransposeArray(size_t count) const;
		std::string GetValueString() const;
		Texture2D GetValueTexture2D() const;
		TextureCube GetValueTextureCube() const;
		Texture3D GetValueTexture3D() const;

		inline EffectParameter(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(EffectParameter const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		struct Implementation;
		std::shared_ptr<Implementation> impl;
	};
}

#endif