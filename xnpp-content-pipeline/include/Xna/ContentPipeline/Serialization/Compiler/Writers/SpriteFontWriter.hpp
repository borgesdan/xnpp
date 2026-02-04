#ifndef XNA_COMPILER_SPRITEFONTWRITER_HPP
#define XNA_COMPILER_SPRITEFONTWRITER_HPP

#include "Xna/ContentPipeline/Serialization/Compiler/BuiltinTypeWriter.hpp"
#include "Xna/ContentPipeline/Graphics/SpriteFontContent.hpp"

namespace Xna {
	struct SpriteFontWriter : BuiltinTypeWriter<SpriteFontContent> {
        std::string GetRuntimeReader(TargetPlatform targetPlatform) override {
            return "SpriteFontReader";
        }

    protected:
		inline void WriteT(ContentWriter& output, const SpriteFontContent* value) {
            output.WriteObject(&value->Texture);
            output.WriteObject(&value->Glyphs);
            output.WriteObject(&value->Cropping);
            output.WriteObject(&value->CharacterMap);

            ContentWriter::Base& output1 = output;

            output1.Write(value->LineSpacing);
            output1.Write(value->Spacing);
            output.WriteObject(&value->Kerning);
            output1.Write(value->DefaultCharacter.has_value());
            
            if (!value->DefaultCharacter.has_value())
                return;

            output1.Write(*value->DefaultCharacter);
		}
	};
}

#endif