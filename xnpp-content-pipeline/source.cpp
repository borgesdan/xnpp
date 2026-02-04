#include "Xna/ContentPipeline/Serialization/Intermediate/ContentTypeSerializer.hpp"
#include "Xna/ContentPipeline/Serialization/Intermediate/XmlListItemSerializer.hpp"
#include "Xna/ContentPipeline/Serialization/Compiler/ContentWriter.hpp"
#include <sol/sol.hpp>
#include "Xna/ContentPipeline/NamedValueDictionary.hpp"
#include "Xna/ContentPipeline/OpaqueDataDictionary.hpp"
#include "Xna/ContentPipeline/Serialization/Compiler/Writers/PrimitivesWriter.hpp"
#include "Xna/ContentPipeline/Serialization/Compiler/Writers/BoundingBoxWriter.hpp"
#include "Xna/ContentPipeline/Serialization/Compiler/Writers/BoundingFrustumWriter.hpp"
#include "Xna/ContentPipeline/Serialization/Compiler/Writers/BoundingSphereWriter.hpp"
#include "Xna/ContentPipeline/BuildItem.hpp"
#include "Xna/ContentPipeline/BuildCoordinator.hpp"
#include "Xna/ContentPipeline/BuildTask.hpp"
#include "Xna/ContentPipeline/App.hpp"
#include "Xna/ContentPipeline/Graphics/CharacterCollection.hpp"
#include "Xna/ContentPipeline/Processors/FontDescriptionProcessor.hpp"

using namespace Xna;

void ClearConsole() {
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