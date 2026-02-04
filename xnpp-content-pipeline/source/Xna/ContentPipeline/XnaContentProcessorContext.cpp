#include "Xna/ContentPipeline/XnaContentProcessorContext.hpp"
#include <Xna/CSharp/Exception.hpp>

namespace Xna {
    XnaContentProcessorContext::XnaContentProcessorContext(
        BuildCoordinator const& buildCoordinator,
        BuildItem const& buildItem,
        ContentBuildLogger const& logger,
        Xna::TargetPlatform targetPlatform,
        GraphicsProfile targetProfile,
        std::string const& buildConfiguration) {
        impl = std::make_shared<Implementation>();
        impl->buildCoordinator = buildCoordinator;
        impl->buildItem = buildItem;
        impl->logger = logger;
        impl->targetProfile = targetProfile;
        impl->targetPlatform = targetPlatform;
        impl->buildConfiguration = buildConfiguration;
    }

    void XnaContentProcessorContext::BuildAsset(
        ExternalReferenceBase& sourceAsset,
        ExternalReferenceBase& output,
        std::string const& processorName,
        OpaqueDataDictionary const& processorParameters,
        std::string const& importerName,
        std::filesystem::path const& assetName) {
        
        auto buildItem = RequestBuild(sourceAsset, assetName, importerName, processorName, processorParameters, BuildOptions::None);
       
        if (!impl->buildItem.Requests().Contains(buildItem.OutputFilename()))
            impl->buildItem.Requests().Add(buildItem.OutputFilename());

        output.Replace(impl->buildCoordinator.GetAbsolutePath(buildItem.OutputFilename()), std::nullopt);
    }

    BuildItem XnaContentProcessorContext::RequestBuild(
        ExternalReferenceBase& sourceAsset,
        std::filesystem::path const& assetName,
        std::string const& importerName,
        std::string const& processorName,
        OpaqueDataDictionary const& processorParameters,
        BuildOptions const& buildOptions) {       
        if (sourceAsset.Filename().empty())
            throw CSharp::ArgumentNullException("sourceAsset.Filename");

        //TODO: [?] essa parte do código usa generics, mas a função não permite isso
        //if (!processorName.empty())
        //    impl->buildCoordinator.ProcessorManager().ValidateProcessorTypes(processorName, typeof(TInput), typeof(TOutput)); 

        BuildRequest request{};
        request.SourceFilename = sourceAsset.Filename();
        request.AssetName = assetName;
        request.ImporterName = importerName;
        request.ProcessorName = processorName;
        request.BuildOptions = buildOptions;
        
        if (processorParameters != nullptr)
        {
            for(const auto& [key, value] : processorParameters)
                request.ProcessorParameters.Add(key, value);
        }

        return impl->buildCoordinator.RequestBuild(request);
    }
}