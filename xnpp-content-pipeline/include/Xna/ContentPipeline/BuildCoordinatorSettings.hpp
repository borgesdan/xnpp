#ifndef XNA_CONTENTPIPELINE_BUILDCOORDINATORSETTINGS_HPP
#define XNA_CONTENTPIPELINE_BUILDCOORDINATORSETTINGS_HPP

#include <filesystem>
#include <string>
#include <Xna/Framework/Content/Pipeline/Shared.hpp>
#include <Xna/Framework/Graphics/Shared.hpp>

namespace Xna {
	struct BuildCoordinatorSettings final {
	public:
		static inline const std::string CacheFilenameWithoutDirectory = "ContentPipeline.xml";
        TargetPlatform TargetPlatform;
        GraphicsProfile TargetProfile;
        std::string BuildConfiguration;
        bool CompressContent{ false };
        std::filesystem::path RootDirectory;
        std::filesystem::path LoggerRootDirectory;
        std::filesystem::path IntermediateDirectory;
        std::filesystem::path OutputDirectory;
        //[ContentSerializerIgnore]
        bool RebuildAll{false};

        void InitializePaths();

        std::filesystem::path CacheFilename() const {
            auto dir = IntermediateDirectory;
            return  dir / "ContentPipeline.xml";
        }

        std::filesystem::path GetAbsolutePath(std::filesystem::path const& path) const;
        std::filesystem::path GetRelativePath(std::filesystem::path const& path) const;
        inline bool MatchesPreviousSettings(BuildCoordinatorSettings const other) const {
            return TargetPlatform == other.TargetPlatform
                && TargetProfile == other.TargetProfile
                && CompressContent == other.CompressContent
                && BuildConfiguration == other.BuildConfiguration
                && RootDirectory == other.RootDirectory //StringComparison.OrdinalIgnoreCase
                && IntermediateDirectory == other.IntermediateDirectory //StringComparison.OrdinalIgnoreCase
                && OutputDirectory == other.OutputDirectory; //StringComparison.OrdinalIgnoreCase
        }

        constexpr bool operator==(const BuildCoordinatorSettings& other) const noexcept {
            return 
                TargetPlatform == other.TargetPlatform 
                && TargetProfile == other.TargetProfile 
                && BuildConfiguration == other.BuildConfiguration 
                && CompressContent == other.CompressContent 
                && RootDirectory == other.RootDirectory 
                && LoggerRootDirectory == other.LoggerRootDirectory 
                && IntermediateDirectory == other.IntermediateDirectory 
                && OutputDirectory == other.OutputDirectory 
                && RebuildAll == other.RebuildAll;
        }

	private:
        std::filesystem::path PrepareOutputDirectory(std::filesystem::path directory, std::filesystem::path defaultValue);
        
        static inline const std::filesystem::path defaultIntermediateDirectory = "obj";
		static inline const std::filesystem::path defaultOutputDirectory = "bin";
    };
}

#endif