# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif ()

# Includes directory
set(PROJECT_INCLUDES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Add source to this project's executable.
add_library(xnpp
        "sources/Xna/CSharp/IO/MemoryStream.cpp"
        "sources/Xna/CSharp/IO/Stream.cpp"
        "sources/Xna/CSharp/IO/FileStream.cpp"
        "sources/Xna/CSharp/IO/BinaryReader.cpp"
        "sources/Xna/CSharp/Windows/Forms/SystemInformation.cpp"
        "sources/Xna/CSharp/Windows/Forms/Screen.cpp"
        "sources/Xna/CSharp/Event.cpp"
        "sources/Xna/CSharp/IO/BinaryWriter.cpp"
        "sources/Xna/CSharp/BitConverter.cpp"        
        "sources/Xna/Framework/FrameworkDispatcher.cpp"       
        "sources/xna/Framework/PackUtils.cpp"
        "sources/Xna/Framework/TitleContainer.cpp"
        "sources/Xna/Framework/Vector2.cpp"
        "sources/Xna/Framework/Vector3.cpp"
        "sources/Xna/Framework/Vector4.cpp"
        "sources/xna/Framework/Game/GameWindow.cpp"
        "sources/Xna/Framework/Game/Game.cpp"
        "sources/Xna/Framework/Game/GameHost.cpp"
        "sources/Xna/Framework/Game/GameClock.cpp"
        "sources/Xna/Framework/Game/GraphicsDeviceManager.cpp"
        "sources/Xna/Framework/Game/GameComponentCollection.cpp"
        "sources/Xna/Framework/Graphics/GraphicsResource.cpp"
        "sources/Xna/Framework/Graphics/BlendState.cpp"
        "sources/Xna/Framework/Graphics/DepthStencilState.cpp"
        "sources/Xna/Framework/Graphics/RasterizerState.cpp"
        "sources/Xna/Framework/Graphics/SamplerState.cpp"
        "sources/Xna/Framework/Graphics/Texture3D.cpp"
        "sources/Xna/Framework/Graphics/Texture2D.cpp"
        
        "sources/Xna/Framework/Graphics/GraphicsDevice.cpp"
        "sources/Xna/Framework/Graphics/RenderTarget2D.cpp"
        "sources/Xna/Framework/Graphics/IndexBuffer.cpp"
        "sources/Xna/Framework/Graphics/DynamicIndexBuffer.cpp"
        "sources/Xna/Framework/Graphics/DisplayModeCollection.cpp"
        "sources/Xna/Framework/Graphics/GraphicsAdapter.cpp"
        "sources/Xna/Framework/Graphics/SpriteBatch.cpp"
        "sources/Xna/Framework/Graphics/SpriteFont.cpp"
        "sources/Xna/Framework/Graphics/GraphicsContentHelper.cpp"
        "sources/Xna/Framework/Graphics/ModelBoneCollection.cpp" 
        "sources/Xna/Framework/Graphics/ModelBone.cpp"
        "sources/Xna/Framework/Graphics/ModelMesh.cpp" 
        "sources/Xna/Framework/Graphics/EffectPass.cpp"
        "sources/Xna/Framework/Graphics/EffectAnnotation.cpp"
        "sources/Xna/Framework/Graphics/EffectParameter.cpp"
        "sources/Xna/Framework/Graphics/Effect.cpp"
        "sources/Xna/Framework/Graphics/BasicEffect.cpp"
        "sources/Xna/Framework/Graphics/EffectHelpers.cpp" 
        "sources/Xna/Framework/Graphics/EffectTechnique.cpp" 
        "sources/Xna/Framework/Graphics/EffectTechniqueCollection.cpp"
        "sources/Xna/Framework/Graphics/EffectAnnotationCollection.cpp"
        "sources/Xna/Framework/Graphics/DynamicVertexBuffer.cpp"
        "sources/Xna/Framework/Graphics/VertexDeclaration.cpp"
        "sources/Xna/Framework/Graphics/VertexBufferBinding.cpp"
        "sources/Xna/Framework/Graphics/VertexBuffer.cpp"
        "sources/Xna/Framework/Content/ContentManager.cpp"
        "sources/Xna/Framework/Content/ContentReader.cpp"
        "sources/Xna/Framework/Content/ContentTypeReaderManager.cpp"
        "sources/Xna/Platform/Windows/WindowsPlatform.cpp"
        "sources/Xna/Platform/Windows/WindowsGraphicsDevice.cpp"
        "sources/Xna/Platform/Windows/WindowsStates.cpp"
        "sources/Xna/Platform/Windows/WindowsSpriteBatch.cpp"
        "sources/Xna/Platform/Windows/WindowsTextures.cpp"
        "sources/Xna/Platform/Windows/WindowsAdapter.cpp"
        "sources/Xna/Platform/Windows/WindowsGameWindow.cpp"
        "sources/Xna/Platform/Windows/WindowsInputs.cpp"
        "sources/Xna/Platform/Windows/WindowsPlatformAudio.cpp"       
        "sources/Xna/Platform/Windows/WindowsBuffer.cpp"
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
         "sources/Xna/CSharp/Type.cpp" "sources/Xna/Framework/Content/Readers/Texture2DReader.cpp")

target_compile_features(xnpp PUBLIC cxx_std_20)

target_compile_definitions(xnpp PRIVATE USING_XINPUT)
target_compile_definitions(xnpp PRIVATE XNPP_EXPORT_DLL)

find_package(directxtk CONFIG REQUIRED)

target_link_libraries(xnpp
        PUBLIC
            Microsoft::DirectXTK 
            dxgi.lib
            dxguid.lib
            D3d11.lib 
            windowscodecs.lib
            Shlwapi.lib 
            runtimeobject.lib
            xaudio2.lib
            Mfplat.lib
            Oleaut32.lib
            Ole32.lib
            Propsys.lib
)

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

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/xnppConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/xnppConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/xnppConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/xnpp
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/xnppConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/xnppConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/xnpp
)