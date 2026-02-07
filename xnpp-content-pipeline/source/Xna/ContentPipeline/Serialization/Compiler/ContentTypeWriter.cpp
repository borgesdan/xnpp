#include "Xna/ContentPipeline/Serialization/Compiler/ContentTypeWriter.hpp"
#include "Xna/ContentPipeline/Serialization/Compiler/ContentCompiler.hpp"

namespace Xna {
	void ContentTypeWriter::DoInitialize(ContentCompiler& compiler) {
		//TODO: [!] implementar corretamente
		Initialize(compiler);

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