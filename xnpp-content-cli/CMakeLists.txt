cmake_minimum_required(VERSION 3.20)

add_executable(xnpp-content-cli
    source/main.cpp
 "source/ProjectReader.cpp")

target_compile_features(xnpp-content-cli PRIVATE cxx_std_20)

find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(xnpp-content-cli
    PRIVATE
        xnpp-content-pipeline
        nlohmann_json::nlohmann_json
)

target_include_directories(xnpp-content-cli
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>   
)

# Para Windows: copiar DLLs runtime se xnpp virar shared futuramente
if(WIN32)
    add_custom_command(TARGET xnpp-content-cli POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:xnpp-content-cli>
            $<TARGET_FILE_DIR:xnpp-content-cli>
        COMMAND_EXPAND_LISTS
    )
endif()

# instalação
install(
    TARGETS xnpp-content-cli
    RUNTIME DESTINATION bin
)