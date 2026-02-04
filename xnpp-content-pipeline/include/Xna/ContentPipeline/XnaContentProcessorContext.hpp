#ifndef XNA_CONTENTPIPELINE_XNACONTENTPROCESSORCONTEXT_HPP
#define XNA_CONTENTPIPELINE_XNACONTENTPROCESSORCONTEXT_HPP

#include "ContentProcessorContext.hpp"
#include "BuildItem.hpp"
#include "BuildCoordinator.hpp"
#include "ContentBuildLogger.hpp"
#include <string>

namespace Xna {
	class XnaContentProcessorContext : public ContentProcessorContext {
	public:
		XnaContentProcessorContext(
			BuildCoordinator const& buildCoordinator,
			BuildItem const& buildItem,
			ContentBuildLogger const& logger,
			Xna::TargetPlatform targetPlatform,
			GraphicsProfile targetProfile,
			std::string const& buildConfiguration);

		~XnaContentProcessorContext() override = default;

		ContentBuildLogger Logger()const override { return impl->logger; }
		OpaqueDataDictionary Parameters()const override { return impl->parameters; }
		Xna::TargetPlatform TargetPlatform()const override { return impl->targetPlatform; }
		GraphicsProfile TargetProfile()const override { return impl->targetProfile; }
		std::string BuildConfiguration()const override { return impl->buildConfiguration; }
		std::filesystem::path OutputFilename()const override { return impl->buildCoordinator.GetAbsolutePath(impl->buildItem.OutputFilename()); }
		std::filesystem::path OutputDirectory()const override { return impl->buildCoordinator.Settings().OutputDirectory; }
		std::filesystem::path IntermediateDirectory()const override { return impl->buildCoordinator.Settings().IntermediateDirectory; }
		void AddDependency(std::filesystem::path filename)const override { impl->buildCoordinator.AddDependency(impl->buildItem, filename); }
		void AddOutputFile(std::filesystem::path filename)const override { impl->buildCoordinator.AddExtraOutput(impl->buildItem, filename); }

		void BuildAsset(
			ExternalReferenceBase& sourceAsset,
			ExternalReferenceBase& output,
			std::string const& processorName,
			OpaqueDataDictionary const& processorParameters,
			std::string const& importerName,
			std::filesystem::path const& assetName) override;

		inline XnaContentProcessorContext(std::nullptr_t) { impl = nullptr; }
		inline bool operator==(XnaContentProcessorContext const& other) const noexcept { return impl == other.impl; }
		inline bool operator==(std::nullptr_t) const noexcept { return impl == nullptr; }
		inline explicit operator bool() const noexcept { return impl != nullptr; }

	private:
		BuildItem RequestBuild(
			ExternalReferenceBase& sourceAsset,
			std::filesystem::path const& assetName,
			std::string const& importerName,
			std::string const& processorName,
			OpaqueDataDictionary const& processorParameters,
			BuildOptions const& buildOptions);

	private:
		struct Implementation {
			BuildCoordinator buildCoordinator{ nullptr };
			BuildItem buildItem{ nullptr };
			ContentBuildLogger logger{ nullptr };
			OpaqueDataDictionary parameters{ nullptr };
			Xna::TargetPlatform targetPlatform;
			GraphicsProfile targetProfile;
			std::string buildConfiguration;
		};

		std::shared_ptr<Implementation> impl;
	};
}

#endif