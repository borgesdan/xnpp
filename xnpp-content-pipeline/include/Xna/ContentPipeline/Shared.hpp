#ifndef XNA_CONTENTPIPELINE_SHARED_HPP
#define XNA_CONTENTPIPELINE_SHARED_HPP

#include <chrono>
#include <ctime>
#include <Xna/Internal/TypeRegistry.hpp>

namespace Xna {
    using TimePoint = std::chrono::system_clock::time_point;
    using Time = std::chrono::system_clock;

    static inline std::time_t GetTime(TimePoint const& time) {        
        return std::chrono::system_clock::to_time_t(time);
    }

    static inline std::tm GetLocalTime(std::time_t const& time) {
        return *std::localtime(&time);
    }

    enum class BuildOptions
    {
        None = 0,
        IntermediateFile = 1,
        DontCacheImportedData = 2,
    };
}

#endif