#include "Xna/ContentPipeline/Serialization/Compiler/Writers/SoundEffectWriter.hpp"

namespace Xna {
    void SoundEffectWriter::WriteT(ContentWriter& output, const SoundEffectContent* value) {
        if (value == nullptr)
            throw CSharp::ArgumentNullException("value");

        ContentWriter::Base& output1 = output;

        output1.Write(static_cast<int32_t>(value->Format.size()));        
        output1.Write(value->Format.data(), value->Format.size());
        output1.Write(static_cast<int32_t>(value->Data.size()));        
        output1.Write(value->Data.data(), value->Data.size());
        output1.Write(value->LoopStart);
        output1.Write(value->LoopLength);
        output1.Write(value->Duration);
    }
}