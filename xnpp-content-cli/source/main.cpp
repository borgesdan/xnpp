#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>

#include "ProjectReader.hpp"
#include <Xna/ContentPipeline/BuildContent.hpp>
#include <Xna/ContentPipeline/App.hpp>

using namespace Xna;

// --- Helper Conversion Functions ---
static std::string ToLower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

static Xna::TargetPlatform ParsePlatform(const std::string& str) {
    std::string lowerStr = ToLower(str);
    if (lowerStr == "windows" || lowerStr == "win") return Xna::TargetPlatform::Windows;
    if (lowerStr == "linux" || lowerStr == "lin") return Xna::TargetPlatform::Linux;
    if (lowerStr == "mac" || lowerStr == "macos") return Xna::TargetPlatform::Mac;
    return Xna::TargetPlatform::CrossDesktop;
}

static Xna::GraphicsProfile ParseProfile(const std::string& str) {
    std::string lowerStr = ToLower(str);
    if (lowerStr == "reach") return Xna::GraphicsProfile::Reach;
    return Xna::GraphicsProfile::HiDef;
}

static void PrintHelp() {
    std::cout << "XNPP Content Pipeline CLI\n\n"
        << "Usage: xnpp-content-cli [file.json] [options]\n\n"
        << "Options:\n"
        << "  -f, --file <path>        JSON project file (Required)\n"
        << "  -p, --platform <target>  windows, linux, mac, crossdesktop (Default: crossdesktop)\n"
        << "  -g, --profile <profile>  reach, hidef (Default: hidef)\n"
        << "  -c, --compress           Enable asset compression (Default: false)\n"
        << "  -l, --logger-dir <path>  Output directory for logs\n"
        << "  -i, --inter-dir <path>   Output directory for intermediate files\n"
        << "  -o, --output-dir <path>  Final output directory (Default: current directory)\n"
        << "  -r, --rebuild            Force rebuild of all files\n"
        << "  -h, --help               Show this help message\n";
}

int main(int argc, char* argv[]) {

    App::Register();

    // 1. Default Values
    std::string filePath = "";
    Xna::TargetPlatform platform = Xna::TargetPlatform::CrossDesktop;
    Xna::GraphicsProfile profile = Xna::GraphicsProfile::HiDef;
    bool compress = false;
    std::string loggerDir = "";
    std::string intermediateDir = "";
    std::string outputDir = "";
    bool rebuild = false;

    // 2. Command Line Argument Parser
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            PrintHelp();
            return 0;
        }
        else if ((arg == "-f" || arg == "--file") && i + 1 < argc) {
            filePath = argv[++i];
        }
        else if ((arg == "-p" || arg == "--platform") && i + 1 < argc) {
            platform = ParsePlatform(argv[++i]);
        }
        else if ((arg == "-g" || arg == "--profile") && i + 1 < argc) {
            profile = ParseProfile(argv[++i]);
        }
        else if (arg == "-c" || arg == "--compress") {
            compress = true;
        }
        else if ((arg == "-l" || arg == "--logger-dir") && i + 1 < argc) {
            loggerDir = argv[++i];
        }
        else if ((arg == "-i" || arg == "--inter-dir") && i + 1 < argc) {
            intermediateDir = argv[++i];
        }
        else if ((arg == "-o" || arg == "--output-dir") && i + 1 < argc) {
            outputDir = argv[++i];
        }
        else if (arg == "-r" || arg == "--rebuild") {
            rebuild = true;
        }
        // If it doesn't start with '-' and filePath is empty, assume it's the JSON file (Positional Argument)
        else if (filePath.empty() && arg[0] != '-') {
            filePath = arg;
        }
        else {
            std::cerr << "Invalid argument or missing parameter: " << arg << "\n";
            return 1;
        }
    }

    // 3. File Validation
    if (filePath.empty()) {
        std::cerr << "Error: JSON file not specified.\n\n";
        PrintHelp();
        return 1;
    }

    // The Root Directory is generally the folder where the JSON is located
    std::filesystem::path jsonPath(filePath);
    const auto rootDirectory = jsonPath.remove_filename();

    // 4. Load JSON Project
    ContentProject myProject;
    std::cout << "Reading project file: " << filePath << "...\n";

    if (!ProjectReader::LoadFromFile(filePath, myProject)) {
        std::cerr << "Failed to load JSON file.\n";
        return 1;
    }

    // 5. Extract Lists
    const auto& compileItems = myProject.Groups.CompileItems;
    const auto& noneItems = myProject.Groups.NoneItems;

    // 6. Initialize Build Components
    ContentBuildLogger logger;

    std::cout << "Starting BuildContent for " << compileItems.size() << " items...\n";

    // 7. Configure and Execute BuildContent
    BuildContent buildContent(
        logger,                     // logger
        platform,                   // targetPlatform
        profile,                    // targetProfile
        "",                         // buildConfiguration 
        compress,                   // compressContent
        rootDirectory,              // rootDirectory
        loggerDir,                  // loggerRootDirectory
        intermediateDir,            // intermediateDirectory
        outputDir,                  // outputDirectory
        rebuild,                    // rebuildAll
        compileItems,               // sourceAssets
        noneItems                   // copyAssets
    );

    bool success = buildContent.Execute();

    if (success) {
        std::cout << "\nBuild completed successfully!\n";
        return 0;
    }
    else {
        std::cerr << "\nErrors occurred during the build.\n";
        return 1;
    }
}