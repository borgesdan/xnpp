#ifndef XNA_CONTENTPIPELINE_BUILDREQUEST_HPP
#define XNA_CONTENTPIPELINE_BUILDREQUEST_HPP

#include <filesystem>
#include "Shared.hpp"
#include "OpaqueDataDictionary.hpp"
#include <cstddef>

namespace Xna {
	struct BuildRequest final {
		//[ContentSerializer(ElementName = "Source", AllowNull = false)]
		std::filesystem::path SourceFilename;
		//[ContentSerializer(ElementName = "Name", Optional = true)]
		std::filesystem::path AssetName;
		//[ContentSerializer(ElementName = "Importer", Optional = true)]
		std::string ImporterName;
		//[ContentSerializer(ElementName = "Processor", Optional = true)]
		std::string ProcessorName;
		//[ContentSerializer(ElementName = "Parameters", Optional = true)]
		OpaqueDataDictionary ProcessorParameters{};
		//[ContentSerializer(ElementName = "Options")]
		Xna::BuildOptions BuildOptions;

		inline bool operator==(const BuildRequest& other) const noexcept {
			return SourceFilename == other.SourceFilename 
				&& AssetName == other.AssetName 
				&& ImporterName == other.ImporterName 
				&& ProcessorName == other.ProcessorName 
				&& ProcessorParameters == other.ProcessorParameters 
				&& BuildOptions == other.BuildOptions;
		}
	};
}

#include <functional>

namespace std
{
    //Returns a hash reporting input values
    template <class T>
    static constexpr void HashCombine(std::size_t& seed, const T& v) {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    template <>
    struct hash<Xna::BuildRequest>
    {
        size_t operator()(const Xna::BuildRequest& r) const noexcept
        {
            size_t seed = 0;

            HashCombine(
                seed, r.SourceFilename
            );

            HashCombine(
                seed, r.AssetName
            );

            HashCombine(
                seed, r.ImporterName
            );

            HashCombine(
                seed, r.ProcessorName
            );

            HashCombine(
                seed, r.ProcessorParameters
            );

            HashCombine(
                seed, r.BuildOptions
            );

            return seed;
        }
    };
}


#endif
