#include "Xna/ContentPipeline/App.hpp"
#include "Xna/ContentPipeline/ProcessorManager.hpp"
#include <string>
#include <Xna/CSharp/Type.hpp>
#include <stdexcept>

namespace Xna {
	struct Resources {
		static constexpr std::string_view CantFindProcessor = "Cannot find content processor '{0}'.";
	};	

	std::shared_ptr<IContentProcessor> ProcessorManager::GetInstance(std::string& processorName, OpaqueDataDictionary const& parameters, ContentIdentity const& contentIdentity, ContentBuildLogger& buildLogger) {
		const auto& registries = App::GetProcessorRegistry().GetRegistries();

		std::optional<std::string> sourceProcessorName;
		bool finded = false;

		for (size_t i = 0; i < registries.size(); ++i) {
			auto& registry = registries[i];
			sourceProcessorName = App::GetProcessorRegistry().GetName(i);

			if (!sourceProcessorName)
				continue;

			if (sourceProcessorName == processorName)
			{
				finded = true;
				break;
			}

			const auto& friendlyNames = App::GetProcessorRegistry().GetFriendlyNames(i);

			if (friendlyNames.empty())
				continue;

			for (const auto& friendly : friendlyNames) {
				if (friendly == processorName)
				{
					finded = true;
					break;
				}
			}

			if (finded)
				break;
		}

		if(!finded)
			throw std::runtime_error(std::format(Resources::CantFindProcessor, processorName));

		auto instance = App::GetProcessorRegistry().CreateInstance(*sourceProcessorName);

		if (!instance)
			throw std::runtime_error(std::format(Resources::CantFindProcessor, processorName));

		SetProcessorParameters(instance, parameters, contentIdentity, buildLogger);
		
		return instance;
	}

	void ProcessorManager::SetProcessorParameters(
		std::shared_ptr<IContentProcessor>& processor,
		OpaqueDataDictionary const& parameters,
		ContentIdentity const& contentIdentity,
		ContentBuildLogger& buildLogger) {
		//TODO: [!] não entendi para que isso aqui
	}
}