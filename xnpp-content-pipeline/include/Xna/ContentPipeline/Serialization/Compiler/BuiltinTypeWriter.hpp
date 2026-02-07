#ifndef XNA_COMPILER_BUILTINTYPEWRITER_HPP
#define XNA_COMPILER_BUILTINTYPEWRITER_HPP

#include "ContentTypeWriter.hpp"
#include <vector>
#include <optional>

namespace Xna {
	template <typename T>
	struct BuiltinTypeWriter : public ContentTypeWriterT<T> {
	protected:	
		BuiltinTypeWriter(){}
		BuiltinTypeWriter(bool isGenericType, std::vector<CSharp::Type> genericTypes)
			: ContentTypeWriterT<T>(isGenericType, genericTypes) { }
		
		~BuiltinTypeWriter() override = default;

		virtual std::string GetRuntimeReader(TargetPlatform targetPlatform) = 0;
	};
}

#endif