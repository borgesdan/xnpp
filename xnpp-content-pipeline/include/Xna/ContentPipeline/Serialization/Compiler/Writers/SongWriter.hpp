#ifndef XNA_WRITER_SONGWRITER_HPP
#define XNA_WRITER_SONGWRITER_HPP

#include "Xna/ContentPipeline/Audio/SongContent.hpp"
#include "Xna/ContentPipeline/Serialization/Compiler/BuiltinTypeWriter.hpp"
#include <Xna/CSharp/Exception.hpp>

namespace Xna {
	struct SongWriter : public BuiltinTypeWriter<SongContent> {
		std::string GetRuntimeReader(TargetPlatform targetPlatform) override {
			return "SongReader";
		}
	protected:
		void WriteT(ContentWriter& output, const SongContent* value) override;
	};
}

#endif