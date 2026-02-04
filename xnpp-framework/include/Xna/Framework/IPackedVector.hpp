#ifndef XNA_FRAMEWORK_IPACKEDVECTOR_HPP
#define XNA_FRAMEWORK_IPACKEDVECTOR_HPP

#include "Vector4.hpp"

namespace Xna {
	//Interface that converts packed vector types to and from Vector4 values, allowing multiple encodings to be manipulated in a generic way.
	class IPackedVector {
	public:
		// 	Expands the packed representation into a Vector4.
		virtual Vector4 ToVector4() const = 0;
		//Sets the packed representation from a Vector4.
		virtual void PackFromVector4(Vector4 const& vector) = 0;
	};

	//Converts packed vector types to and from Vector4 values.
	template <typename T>
	class IPackedVectorT {
	public:
		//Directly gets or sets the packed representation of the value.
		virtual T PackedValue() const = 0;
		//Directly gets or sets the packed representation of the value.
		virtual void PackedValue(T const& value) = 0;
	};
}

#endif