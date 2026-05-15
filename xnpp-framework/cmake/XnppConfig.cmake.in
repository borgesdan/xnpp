@PACKAGE_INIT@

include(CMakeFindDependencyMacro)

include("${CMAKE_CURRENT_LIST_DIR}/xnppTargets.cmake")

check_required_components(xnpp)

# Substitui o caminho chumbado por um caminho relativo inteligente resolvido pelo CMake
set_and_check(XNPP_SHADERS_DIR "@PACKAGE_SHADERS_INSTALL_DIR@")

# Cria uma função utilitária para os usuários da lib
function(xnpp_deploy_shaders TARGET_NAME)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${TARGET_NAME}>/shaders"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${XNPP_SHADERS_DIR}"
        "$<TARGET_FILE_DIR:${TARGET_NAME}>/shaders"
        COMMENT "Copiando shaders do bgfx/XNPP para o output de ${TARGET_NAME}..."
    )
endfunction()

function(xnpp_build_content TARGET_NAME)
    # 1. Resolução dos caminhos
    if(ARGC GREATER 1)
        set(PIPELINE_CONFIG_FILE "${ARGV1}")
        get_filename_component(CONTENT_DIR "${PIPELINE_CONFIG_FILE}" DIRECTORY)
    else()
        set(CONTENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Content")
        set(PIPELINE_CONFIG_FILE "${CONTENT_DIR}/content.json")
    endif()

    if(NOT EXISTS "${CONTENT_DIR}" OR NOT EXISTS "${PIPELINE_CONFIG_FILE}")
        message(STATUS "XNPP: Pasta Content ou content.json não encontrados. Pulando o build do content pipeline para o target '${TARGET_NAME}'.")
        return()
    endif()

    # 2. Localiza a ferramenta CLI instalada
    find_program(
        XNPP_CONTENT_CLI
        NAMES xnpp-content-cli
        REQUIRED
        MESSAGE "XNPP: xnpp-content-cli não foi encontrado. O pacote xnpp foi compilado com a feature 'content-pipeline'?"
    )

    # 3. Resolve a plataforma
    if(WIN32)
        set(CONTENT_PLATFORM "Windows")
    elseif(APPLE)
        set(CONTENT_PLATFORM "Mac")
    elseif(UNIX AND NOT APPLE)
        set(CONTENT_PLATFORM "Linux")
    else()
        set(CONTENT_PLATFORM "Unknown")
    endif()

    set(SOURCE_CONTENT_DIR "${CONTENT_DIR}/bin/${CONTENT_PLATFORM}")
    
    # 4. Acopla o comando diretamente ao alvo (Sem targets extras = Sem ciclos!)
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        # Garante que a pasta destino exista
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${TARGET_NAME}>/Content"
        
        # Executa a CLI do XNPP
        COMMAND ${XNPP_CONTENT_CLI} "${PIPELINE_CONFIG_FILE}"
        
        # Copia da pasta bin/[Plataforma]/ gerada pela CLI para a pasta do executável
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${SOURCE_CONTENT_DIR}" "$<TARGET_FILE_DIR:${TARGET_NAME}>/Content"
        
        COMMENT "XNPP: Compilando e copiando Content Pipeline para ${TARGET_NAME}..."
        VERBATIM
    )
endfunction()