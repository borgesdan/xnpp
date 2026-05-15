# Includes directory
set(PROJECT_INCLUDES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(XNPP_FRAMEWORK_SRC  
    "sources/Xna/Framework/FrameworkDispatcher.cpp"       
    "sources/xna/Framework/PackUtils.cpp"
    "sources/Xna/Framework/TitleContainer.cpp"   
    "sources/xna/Framework/Game/GameWindow.cpp"
    "sources/Xna/Framework/Game/Game.cpp"
    "sources/Xna/Framework/Game/GameHost.cpp"
    "sources/Xna/Framework/Game/GameClock.cpp"
    "sources/Xna/Framework/Game/GraphicsDeviceManager.cpp"
    "sources/Xna/Framework/Game/GameComponentCollection.cpp"
    "sources/Xna/Framework/Graphics/GraphicsResource.cpp"
    "sources/Xna/Framework/Graphics/Texture3D.cpp"
    "sources/Xna/Framework/Graphics/Texture2D.cpp"        
    "sources/Xna/Framework/Graphics/GraphicsDevice.cpp"
    "sources/Xna/Framework/Graphics/RenderTarget2D.cpp"
    "sources/Xna/Framework/Graphics/IndexBuffer.cpp"    
    "sources/Xna/Framework/Graphics/DisplayModeCollection.cpp"
    "sources/Xna/Framework/Graphics/GraphicsAdapter.cpp"
    "sources/Xna/Framework/Graphics/SpriteBatch.cpp"
    "sources/Xna/Framework/Graphics/SpriteFont.cpp"
    "sources/Xna/Framework/Graphics/GraphicsContentHelper.cpp"   
    "sources/Xna/Framework/Input/Input.cpp"
    "sources/Xna/Framework/Content/ContentManager.cpp"
    "sources/Xna/Framework/Content/ContentReader.cpp"
    "sources/Xna/Framework/Content/ContentTypeReaderManager.cpp"    
    "sources/Xna/App.cpp"
    "sources/Xna/Framework/Audio/AudioFormat.cpp"
    "sources/Xna/Framework/Audio/AudioHelper.cpp"
    "sources/Xna/Framework/Audio/WavFile.cpp"
    "sources/Xna/Framework/Audio/SoundEffect.cpp"
    "sources/Xna/Framework/Audio/SoundEffectInstance.cpp"
    "sources/Xna/Framework/Audio/DynamicSoundEffectInstance.cpp" 
    "sources/Xna/Framework/Media/Song.cpp" 
    "sources/Xna/Framework/Media/MediaPlayer.cpp"
    "sources/Xna/Framework/Media/MediaQueue.cpp"
    "sources/Xna/Framework/Content/Readers/BasicReaders.cpp" 
    "sources/Xna/Framework/Graphics/Model.cpp"
)

set(XNPP_PLATFORM_SRC
    "sources/Xna/Platform/Cross/System.cpp" 
    "sources/Xna/Platform/Cross/GameWindow.cpp"
    "sources/Xna/Platform/Cross/GameHost.cpp"
    "sources/Xna/Platform/Cross/Input.cpp" 
    "sources/Xna/Platform/Cross/Audio.cpp"
    "sources/Xna/Platform/Cross/Graphics/Device.cpp"
    "sources/Xna/Platform/Cross/Graphics/Texture.cpp"
    "sources/Xna/Platform/Cross/Graphics/SpriteBatch.cpp"
    "sources/Xna/Platform/Cross/Graphics/Adapter.cpp"
    "sources/Xna/Platform/Cross/Graphics/Buffer.cpp"
)

set(MINIAUDIO_VORBIS_SRC 
    "sources/Xna/Platform/Cross/third-party/miniaudio/miniaudio_libvorbis.c"
)

add_library(xnpp
        ${XNPP_FRAMEWORK_SRC}
        ${XNPP_PLATFORM_SRC}
        ${MINIAUDIO_VORBIS_SRC} 
)

# Lógica mágica de exportação/importação:
if(BUILD_SHARED_LIBS)
    # Se for DLL, dizemos APENAS para o xnpp exportar os símbolos.
    # Os consumidores não recebem essa macro e caem no 'dllimport'.
    target_compile_definitions(xnpp PRIVATE XNPP_EXPORT_DLL)
else()
    # Se for ESTÁTICO, nós desligamos o import/export.
    # Usamos PUBLIC para que qualquer target que faça link com xnpp 
    # (como seus .exe) também herde a macro XNPP_STATIC automaticamente!
    target_compile_definitions(xnpp PUBLIC XNPP_STATIC)
endif()

target_compile_features(xnpp PUBLIC cxx_std_20)

#SDL3
find_package(SDL3 CONFIG REQUIRED)
#libvorbis
find_package(Ogg CONFIG REQUIRED)
find_package(Vorbis CONFIG REQUIRED)
#bgfx
find_package(bgfx CONFIG REQUIRED)

target_link_libraries(xnpp       
    PUBLIC
        xnpp-csharp
    PRIVATE 
        SDL3::SDL3
        Ogg::ogg
        Vorbis::vorbis
        Vorbis::vorbisfile
        bgfx::bgfx
        bgfx::bx
        bgfx::bimg        
)

if(WIN32)
    target_link_libraries(xnpp
        PRIVATE
            dxgi.lib
    )
endif()

target_include_directories(xnpp
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>            
)

set_target_properties(xnpp PROPERTIES
    EXPORT_NAME xnpp
)

include(GNUInstallDirs)

install(
    TARGETS xnpp
    EXPORT xnppTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    EXPORT xnppTargets
    FILE xnppTargets.cmake
    NAMESPACE xnpp::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/xnpp
)

include(CMakePackageConfigHelpers)

# Define onde os shaders serão instalados no sistema do usuário / vcpkg (ex: share/xnpp/shaders)
set(SHADERS_INSTALL_DIR "${CMAKE_INSTALL_DATADIR}/xnpp/shaders")

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/xnppConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/xnppConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/xnppConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/xnpp
    PATH_VARS SHADERS_INSTALL_DIR # Expondo o diretório para o arquivo .cmake.in
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/xnppConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/xnppConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/xnpp
)

# Cópia da pasta shaders
# Descobre a plataforma alvo
if(WIN32)
    set(SHADER_OS_DIR "windows")
elseif(APPLE)
    set(SHADER_OS_DIR "mac")
elseif(UNIX)
    set(SHADER_OS_DIR "linux")
endif()

# ---------------------------------------------------------
# 1. PARA O BUILD LOCAL (Testes internos da XNPP)
# ---------------------------------------------------------
add_custom_command(TARGET xnpp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders/binary/${SHADER_OS_DIR}"
        "$<TARGET_FILE_DIR:xnpp>/shaders"
    COMMENT "Copying the shaders folder to the local output directory..."
)

set_target_properties(xnpp PROPERTIES
    ADDITIONAL_CLEAN_FILES "$<TARGET_FILE_DIR:xnpp>/shaders"
)

# ---------------------------------------------------------
# 2. PARA O EMPACOTAMENTO (vcpkg / install)
# ---------------------------------------------------------
# Instala os shaders da plataforma correspondente usando a variável dinâmica
install(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/shaders/binary/${SHADER_OS_DIR}/"
    DESTINATION "${SHADERS_INSTALL_DIR}"
)