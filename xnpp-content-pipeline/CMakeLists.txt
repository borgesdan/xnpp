# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif ()

# Includes directory
set(PROJECT_INCLUDES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Add source to this project's executable.
add_executable(XnppContentPipeline "source.cpp"
        "source/Xna/ContentPipeline/Serialization/Compiler/ContentWriter.cpp"
        "source/Xna/ContentPipeline/PathUtils.cpp"
        "source/Xna/ContentPipeline/Serialization/Compiler/ContentCompiler.cpp"
        "source/Xna/ContentPipeline/BuildCoordinator.cpp"
        "source/Xna/ContentPipeline/BuildCoordinatorSettings.cpp"
        "source/Xna/ContentPipeline/BuildItemCollection.cpp" 
        "source/Xna/ContentPipeline/ImporterManager.cpp"  
        "source/Xna/ContentPipeline/OpaqueDataDictionary.cpp"
        "source/Xna/ContentPipeline/ProcessorManager.cpp"
        "source/Xna/ContentPipeline/TimestampCache.cpp" 
        "source/Xna/ContentPipeline/XnaContentProcessorContext.cpp" 
        "source/Xna/ContentPipeline/BuildContent.cpp"
        "source/Xna/ContentPipeline/BuildTask.cpp" 
        "source/Xna/ContentPipeline/App.cpp" 
        "source/Xna/ContentPipeline/Importers/TextureImporter.cpp"
        "source/Xna/ContentPipeline/Graphics/MipmapChainCollection.cpp"
        "source/Xna/ContentPipeline/Processors/TextureProcessor.cpp"
        "source/Xna/ContentPipeline/Importers/FontDescriptionImporter.cpp" 
        "source/Xna/ContentPipeline/Graphics/FontDescription.cpp"
        "source/Xna/ContentPipeline/Processors/FontDescriptionProcessor.cpp" "source/Xna/ContentPipeline/Audio/AudioFormat.cpp" "source/Xna/ContentPipeline/Importers/WavImporter.cpp" "source/Xna/ContentPipeline/Processors/SoundEffectProcessor.cpp" "source/Xna/ContentPipeline/Serialization/Compiler/SoundEffectWriter.cpp" "source/Xna/ContentPipeline/Audio/AudioHelper.cpp" "source/Xna/ContentPipeline/Importers/WmaImporter.cpp" "source/Xna/ContentPipeline/Processors/SongProcessor.cpp" "source/Xna/ContentPipeline/Serialization/Compiler/SongWriter.cpp")

target_compile_features(XnppContentPipeline PUBLIC cxx_std_20)

#Add tests and install targets

find_package(lua CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(pugixml CONFIG REQUIRED)
find_package(Freetype REQUIRED)
find_package(FFMPEG REQUIRED)

find_path(STB_INCLUDE_DIR "stb_image.h")
target_include_directories(XnppContentPipeline
    PRIVATE
    ${FFMPEG_INCLUDE_DIRS}
    ${Stb_INCLUDE_DIR}
)

target_link_libraries(XnppContentPipeline
        PRIVATE
        xnpp
        lua
        nlohmann_json::nlohmann_json   
        pugixml::pugixml
        Freetype::Freetype
        ${FFMPEG_LIBRARIES}
)



target_compile_definitions(XnppContentPipeline
    PRIVATE
        #Ativa todas as verificações de segurança opcionais do sol2.
        #Checagem de: tipos Lua ↔ C++, número de argumentos, validade de referências (userdata)
        #chamadas a funções nulas, Validação adicional em: sol::function, sol::protected_function, sol::object
        SOL_ALL_SAFETIES_ON=1 
        #Garante que exceções C++ lançadas dentro de funções expostas ao Lua:
        #não atravessem a stack do Lua diretamente
        #sejam capturadas e convertidas em erro Lua (lua_error)
        SOL_EXCEPTIONS_SAFE_PROPAGATION=1 
        
)

target_include_directories(XnppContentPipeline
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

add_custom_command(TARGET XnppContentPipeline POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:xnpp>
        $<TARGET_FILE_DIR:XnppContentPipeline>
)