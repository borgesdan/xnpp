#include <Xna/ContentPipeline/App.hpp>
#include <Xna/ContentPipeline/BuildTask.hpp>

using namespace Xna;

static void ClearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int main(int argc, char* argv[]) {

    App::Register();

    std::string caminhoArquivo;

    while (true) {
        std::cout << "Enter the full file path (ex: D:\\Project\\Project.json): ";

        std::getline(std::cin, caminhoArquivo);

        if (!std::filesystem::exists(caminhoArquivo)) {
            std::cout << "File not found";
            continue;
        }

        auto result = BuildTask::OpenProject(caminhoArquivo.c_str());

        break;
    }


    return 0;
}