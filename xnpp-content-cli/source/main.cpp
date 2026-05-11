#include <Xna/ContentPipeline/App.hpp>
#include <Xna/ContentPipeline/BuildTask.hpp>
#include <iostream>
#include <filesystem>
#include <string>

using namespace Xna;

int main(int argc, char* argv[]) {

    App::Register();

    //// 1 - ler argv
    //// 2 - argv deve ter o caminho do arquivo .json
    //if (argc < 2) {
    //    std::cerr << "Error: .json file path not provided.\n";
    //    std::cerr << "Usage: " << argv[0] << " <path_to_content.json>\n";
    //    return 1; // Finaliza o programa por erro de argumento
    //}

    //std::filesystem::path filePath = argv[1];

    //// 3 - Se não existe exibir mensagem e finalizar 
    //if (!std::filesystem::exists(filePath)) {
    //    std::cerr << "Error: The file '" << filePath.string() << "' does not exist.\n";
    //    return 1; // Finaliza o programa pois o arquivo não foi encontrado
    //}

    //// 4 - Se existe, passar o arquivo do arquivo para BuildTask::OpenProject
    //// 5 - BuildTask::OpenProject(caminhoArquivo);
    //BuildTask::OpenProject(filePath.string());

    BuildTask::OpenProject("C:\\Users\\Borges\\source\\repos\\XnppProjects\\02_platformer\\Content\\content.json");

    // 6 - Finalizar o programa
    return 0;
}