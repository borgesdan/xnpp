#ifndef XNA_CONTENT_READERS_SONGREADER_HPP
#define XNA_CONTENT_READERS_SONGREADER_HPP

#include "../ContentTypeReader.hpp"
#include "../ContentReader.hpp"
#include "Xna/Framework/Media/Song.hpp"
#include <filesystem>
#include "Xna/CSharp/IO/BinaryReader.hpp"

namespace Xna {
    struct SongReader : public ContentTypeReaderT<Song> {
    protected:
        inline Song Read(ContentReader& input, Song& existingInstance) override {
            const auto referenceName = input.ReadString();
            const auto absolutePathToReference = input.GetAbsolutePathToReference(referenceName);
            const auto duration = input.ReadObject<int32_t>();
            
            return Song(input.AssetName().string(), absolutePathToReference, duration);
        }
    };
}

#endif