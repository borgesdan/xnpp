#include "Xna/ContentPipeline/BuildTask.hpp"
#include "Xna/ContentPipeline/CompileItem.hpp"
#include <fstream>

using json = nlohmann::json;
namespace fs = std::filesystem;

namespace Xna {
	static void from_json(const json& j, CompileItem& item)
	{
		j.at("Include").get_to(item.Include);
		j.at("Name").get_to(item.Name);
		j.at("Importer").get_to(item.Importer);
		j.at("Processor").get_to(item.Processor);
	}

	static void from_json(const json& j, NoneItem& item)
	{
		j.at("Include").get_to(item.Include);
		j.at("Name").get_to(item.Name);
		j.at("CopyToOutputDirectory").get_to(item.CopyToOutputDirectory);
	}

	static std::vector<CompileItem> LoadCompileItems(const json& root)
	{
		if (!root.contains("ItemGroups") ||	!root["ItemGroups"].contains("CompileItems")) {
			return {};
		}

		return root["ItemGroups"]["CompileItems"]
			.get<std::vector<CompileItem>>();
	}

	static std::vector<NoneItem> LoadNoneItems(const json& root)
	{
		if (!root.contains("ItemGroups") || !root["ItemGroups"].contains("NoneItems")) {
			return {};
		}

		return root["ItemGroups"]["NoneItems"]
			.get<std::vector<NoneItem>>();
	}

	bool BuildTask::OpenProject(fs::path const& filename) {
		filename.lexically_normal();
		std::ifstream file(filename);

		if (!file)
			return false;

		json data;
		try {
			data = json::parse(file);
		}
		catch (const json::parse_error& e)
		{
			std::cerr << "Erro de parse JSON:\n";
			std::cerr << e.what() << '\n';
			std::cerr << "Byte position: " << e.byte << '\n';
		}

		auto filename1 = filename;

		const auto& rootDirectory = filename1.remove_filename();
		const auto& loggerDir = rootDirectory / "log";
		const auto paths = LoadCompileItems(data);
		const auto noneItems = LoadNoneItems(data);
		logger = ContentBuildLogger();

		auto buildContent = BuildContent(
			logger, Xna::TargetPlatform::Windows, 
			Xna::GraphicsProfile::HiDef,
			"",
			false,
			rootDirectory,
			loggerDir,
			"",
			"",
			true, 
			paths,
			noneItems);

		buildContent.Execute();
		
		
	}
}