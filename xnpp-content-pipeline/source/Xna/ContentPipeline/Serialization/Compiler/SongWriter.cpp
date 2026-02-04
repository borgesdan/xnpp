#include "Xna/ContentPipeline/Serialization/Compiler/Writers/SongWriter.hpp"
#include <stdexcept>
#include "Xna/ContentPipeline/Serialization/Compiler/ContentWriter.hpp"

namespace Xna {
	void SongWriter::WriteT(ContentWriter& output, const SongContent* value) {
		if (value == nullptr)
			throw std::runtime_error("SongContent value is null");

		ContentWriter::Base& output1 = output;
		output1.Write(value->FileName.string());
		output.WriteObject<int32_t>(&value->Duration);
	}
}