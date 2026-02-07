#ifndef XNA_WRITERS_LISTWRITER_HPP
#define XNA_WRITERS_LISTWRITER_HPP

#include "Xna/ContentPipeline/Serialization/Compiler/BuiltinTypeWriter.hpp"
#include <vector>

namespace Xna {
	template <typename T>
	struct ListWriter : public BuiltinTypeWriter<std::vector<T>> {
		ListWriter(std::string const& friendlyTypeName) 
			: friendlyTypeName(friendlyTypeName) {}

		std::string GetRuntimeReader(TargetPlatform targetPlatform) override {
			std::string reader = "ListReader<";
			reader.append(friendlyTypeName);
			reader.append(">");

			return reader;
		}

	protected:
		void Initialize(ContentCompiler& compiler) override {
			elementWriter = compiler.GetTypeWriter(typeid(T));

			if (!elementWriter)
				throw std::runtime_error("ElementeWriter not founded.");
		}

		void WriteT(ContentWriter& output, const std::vector<T>* value) override {
			ContentWriter::Base& output1 = output;
			output1.Write((int32_t)value->size());
			
			for(const auto& obj : *value)
				output.WriteObject<T>(&obj, *elementWriter);
		}

		virtual bool ShouldCompressContent(TargetPlatform targetPlatform, std::any const& value) {
			return elementWriter->ShouldCompressContent(targetPlatform, {});
		}

	private:
		std::shared_ptr<ContentTypeWriter> elementWriter;
		std::string friendlyTypeName;
	};
}

#endif