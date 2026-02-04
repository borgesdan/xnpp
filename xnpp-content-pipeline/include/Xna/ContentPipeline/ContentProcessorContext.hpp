#ifndef XNA_CONTENTPIPELINE_CONTENTPROCESSORCONTEXT_HPP
#define XNA_CONTENTPIPELINE_CONTENTPROCESSORCONTEXT_HPP

#include <filesystem>
#include "ContentBuildLogger.hpp"
#include "OpaqueDataDictionary.hpp"
#include "ExternalReference.hpp"
#include <Xna/Framework/Content/Pipeline/Shared.hpp>
#include <Xna/Framework/Graphics/Shared.hpp>

namespace Xna {
	struct ContentProcessorContext {
		virtual ~ContentProcessorContext() = default;
		virtual ContentBuildLogger Logger() const = 0;
		virtual OpaqueDataDictionary Parameters() const = 0;
		virtual Xna::TargetPlatform TargetPlatform() const = 0;
		virtual GraphicsProfile TargetProfile() const = 0;
		virtual std::string BuildConfiguration() const = 0;
		virtual std::filesystem::path OutputFilename() const = 0;
		virtual std::filesystem::path OutputDirectory() const = 0;
		virtual std::filesystem::path IntermediateDirectory() const = 0;
		virtual void AddDependency(std::filesystem::path filename) const = 0;
		virtual void AddOutputFile(std::filesystem::path filename) const = 0;		

		template <typename TInput, typename TOutput>
		ExternalReference<TOutput> BuildAsset(
			ExternalReference<TInput>& sourceAsset,
			std::string const& processorName,
			OpaqueDataDictionary& processorParameters,
			std::string const& importerName,
			std::filesystem::path assetName) {

			ExternalReference<TOutput> output{};
			BuildAsset(sourceAsset, output, processorName, processorParameters, importerName, assetName);

			return output;
		}

		virtual void BuildAsset(
			ExternalReferenceBase& sourceAsset,
			ExternalReferenceBase& output,
			std::string const& processorName,
			OpaqueDataDictionary const& processorParameters,
			std::string const& importerName,
			std::filesystem::path const& assetName) = 0;		
	};
}

#endif