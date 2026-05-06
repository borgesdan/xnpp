# Enable Hot Reload for MSVC compilers if supported
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(
        CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
        "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,
        $<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,
        $<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>"
    )
endif()

# =========================
# Content Pipeline Library
# =========================
add_library(xnpp-content-pipeline STATIC
    source/Xna/ContentPipeline/Serialization/Compiler/ContentWriter.cpp
    source/Xna/ContentPipeline/PathUtils.cpp
    source/Xna/ContentPipeline/Serialization/Compiler/ContentCompiler.cpp
    source/Xna/ContentPipeline/BuildCoordinator.cpp
    source/Xna/ContentPipeline/BuildCoordinatorSettings.cpp
    source/Xna/ContentPipeline/BuildItemCollection.cpp
    source/Xna/ContentPipeline/ImporterManager.cpp
    source/Xna/ContentPipeline/OpaqueDataDictionary.cpp
    source/Xna/ContentPipeline/ProcessorManager.cpp
    source/Xna/ContentPipeline/TimestampCache.cpp
    source/Xna/ContentPipeline/XnaContentProcessorContext.cpp
    source/Xna/ContentPipeline/BuildContent.cpp
    source/Xna/ContentPipeline/BuildTask.cpp
    source/Xna/ContentPipeline/App.cpp

    # Importers
    source/Xna/ContentPipeline/Importers/TextureImporter.cpp
    source/Xna/ContentPipeline/Importers/FontDescriptionImporter.cpp
    source/Xna/ContentPipeline/Importers/WavImporter.cpp
    source/Xna/ContentPipeline/Importers/WmaImporter.cpp
    source/Xna/ContentPipeline/Importers/OggImporter.cpp

    # Graphics
    source/Xna/ContentPipeline/Graphics/MipmapChainCollection.cpp
    source/Xna/ContentPipeline/Graphics/FontDescription.cpp

    # Processors
    source/Xna/ContentPipeline/Processors/TextureProcessor.cpp
    source/Xna/ContentPipeline/Processors/FontDescriptionProcessor.cpp
    source/Xna/ContentPipeline/Processors/SoundEffectProcessor.cpp
    source/Xna/ContentPipeline/Processors/SongProcessor.cpp

    # Audio
    source/Xna/ContentPipeline/Audio/AudioFormat.cpp
    source/Xna/ContentPipeline/Audio/AudioHelper.cpp

    # Writers
    source/Xna/ContentPipeline/Serialization/Compiler/SoundEffectWriter.cpp
    source/Xna/ContentPipeline/Serialization/Compiler/SongWriter.cpp
    source/Xna/ContentPipeline/Serialization/Compiler/ContentTypeWriter.cpp
)

target_compile_features(xnpp-content-pipeline PRIVATE cxx_std_20)

# =========================
# Dependencies
# =========================
find_package(nlohmann_json CONFIG REQUIRED)
find_package(pugixml CONFIG REQUIRED)
find_package(Freetype REQUIRED)
find_package(FFMPEG CONFIG)

find_path(STB_INCLUDE_DIR stb_image.h REQUIRED)

target_include_directories(xnpp-content-pipeline
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>

    PRIVATE
        ${STB_INCLUDE_DIR}
        ${FFMPEG_INCLUDE_DIRS}
)

target_link_libraries(xnpp-content-pipeline
    PUBLIC
        xnpp

    PRIVATE
        nlohmann_json::nlohmann_json
        pugixml::pugixml
        Freetype::Freetype
        ${FFMPEG_LIBRARIES}
)

# =========================
# Install
# =========================
install(
    TARGETS xnpp-content-pipeline
    EXPORT xnppTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(
    DIRECTORY include/
    DESTINATION include
)