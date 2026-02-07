#include "Xna/ContentPipeline/Serialization/Compiler/ContentTypeWriter.hpp"
#include "Xna/ContentPipeline/Serialization/Compiler/ContentCompiler.hpp"

namespace Xna {
	void ContentTypeWriter::DoInitialize(ContentCompiler& compiler) {		
		Initialize(compiler);		
		return;

		//TODO: [!] creio que esse código em C++ não seja mais necessário
		//if (!this.targetType.IsGenericType) return;
		if (!this->isGenericType && genericArgumentWriters.empty())
			return;
		//this.genericArgumentWriters = new List<ContentTypeWriter>();
		//foreach(Type genericArgument in this.targetType.GetGenericArguments())
		//	this.genericArgumentWriters.Add(compiler.GetTypeWriter(genericArgument));
		for (const auto& genericArgument : genericArgumentWriters)
			compiler.GetTypeWriter(genericArgument);
	}
}