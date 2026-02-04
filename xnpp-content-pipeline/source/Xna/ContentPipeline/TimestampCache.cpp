#include "Xna/ContentPipeline/TimestampCache.hpp"
#include "Xna/ContentPipeline/PathUtils.hpp"

namespace Xna {
	TimePoint TimestampCache::GetTimestamp(std::filesystem::path const& filename) {
        TimePoint timestamp{};
        if (!impl->timestampCache.contains(filename))
        {
            timestamp = PathUtils::GetTimestamp(filename);
            impl->timestampCache[filename] = timestamp;
        }
        else {
            timestamp = impl->timestampCache[filename];
        }

        return timestamp;
	}

    void TimestampCache::Remove(std::filesystem::path const& filename) {
        impl->timestampCache.erase(filename);
        impl->existsCache.erase(filename);
    }

    bool TimestampCache::FileExists(std::filesystem::path const& filename) {
        bool flag;
        if (!impl->existsCache.contains(filename))
        {
            flag = std::filesystem::exists(filename);
            impl->existsCache[filename] = flag;
        }
        else {
            flag = impl->existsCache[filename];
        }

        return flag;
    }
}