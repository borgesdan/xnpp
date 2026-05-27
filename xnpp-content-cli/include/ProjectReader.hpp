#ifndef CONTENT_CLI_PROJECT_READER_HPP
#define CONTENT_CLI_PROJECT_READER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <Xna/ContentPipeline/CompileItem.hpp>

namespace Xna {
    struct ItemGroups {
        std::vector<CompileItem> CompileItems;
        std::vector<NoneItem> NoneItems;
    };

    struct ContentProject {
        ItemGroups Groups;
    };

    class ProjectReader {
    public:
        static bool LoadFromFile(const std::filesystem::path& filePath, ContentProject& outProject);
    };
}

#endif