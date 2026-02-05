#ifndef XNA_CONTENTPIPELINE_BUILDREQUEST_HPP
#define XNA_CONTENTPIPELINE_BUILDREQUEST_HPP

#include <filesystem>
#include "Shared.hpp"
#include "OpaqueDataDictionary.hpp"
#include <Xna/Internal/Misc.hpp>

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
		BuildOptions BuildOptions;

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
#include <filesystem>

namespace std
{
    template <>
    struct hash<Xna::BuildRequest>
    {
        size_t operator()(const Xna::BuildRequest& r) const noexcept
        {
            size_t seed = 0;

            Xna::Misc::HashCombine(
                seed, r.SourceFilename
            );

            Xna::Misc::HashCombine(
                seed, r.AssetName
            );

            Xna::Misc::HashCombine(
                seed, r.ImporterName
            );

            Xna::Misc::HashCombine(
                seed, r.ProcessorName
            );

            Xna::Misc::HashCombine(
                seed, r.ProcessorParameters
            );

            Xna::Misc::HashCombine(
                seed, r.BuildOptions
            );

            return seed;
        }
    };
}


#endif