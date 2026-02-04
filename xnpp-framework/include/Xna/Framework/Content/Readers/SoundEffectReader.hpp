#ifndef XNA_CONTENT_READERS_SOUNDEFFECTREADER_HPP
#define XNA_CONTENT_READERS_SOUNDEFFECTREADER_HPP

#include "../ContentTypeReader.hpp"
#include "../ContentReader.hpp"
#include "Xna/Framework/Audio/SoundEffect.hpp"

namespace Xna {
    struct SoundEffectReader : public ContentTypeReaderT<SoundEffect> {
    protected:
        inline SoundEffect Read(ContentReader& input, SoundEffect& existingInstance) override {
            const auto count1 = input.ReadInt32();
            const auto format = input.ReadBytes(count1);
            const auto count2 = input.ReadInt32();
            const auto data = input.ReadBytes(count2);
            const auto loopStart = input.ReadInt32();
            const auto loopLength = input.ReadInt32();
            const auto num = input.ReadInt32();
            return SoundEffect(format, data, loopStart, loopLength, CSharp::TimeSpan::FromMilliseconds(static_cast<double>(num)));
        }
    };
}

#endif