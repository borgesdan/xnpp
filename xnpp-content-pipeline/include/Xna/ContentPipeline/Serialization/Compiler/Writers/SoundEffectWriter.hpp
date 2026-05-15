#ifndef XNA_WRITER_SOUNDEFFECTWRITER_HPP
#define XNA_WRITER_SOUNDEFFECTWRITER_HPP

#include "Xna/ContentPipeline/Audio/SoundEffectContent.hpp"
#include "Xna/ContentPipeline/Serialization/Compiler/BuiltinTypeWriter.hpp"
#include <Xna/CSharp/Exception.hpp>
#include <Xna/Internal/Macros.hpp>

namespace Xna {
	struct SoundEffectWriter : public BuiltinTypeWriter<SoundEffectContent> {
		inline std::string GetRuntimeReader(TargetPlatform targetPlatform) override {
			return "SoundEffectReader";
		}
	protected:
		XNPP_API void WriteT(ContentWriter& output, const SoundEffectContent* value) override;
	};
}

#endif