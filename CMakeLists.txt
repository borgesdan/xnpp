# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif ()

cmake_minimum_required(VERSION 3.12)

project("XnppFramework" VERSION 0.1.0 LANGUAGES C CXX)

option(XNPP_BUILD_LIB "Build Xnpp library" ON)
option(XNPP_BUILD_CONTENT_PIPELINE "Build Xnpp Content Pipeline" ON)
option(XNPP_BUILD_CONTENT_CLI "Build Xnpp Content Pipeline CLI" ON)
option(XNPP_BUILD_GAME_TEST "Build Xnpp Game Test" ON)

if (XNPP_BUILD_LIB)
	add_subdirectory("xnpp-csharp")
	add_subdirectory("xnpp-framework")
endif()

if (XNPP_BUILD_CONTENT_PIPELINE)
	add_subdirectory("xnpp-content-pipeline")
endif()

if (XNPP_BUILD_CONTENT_CLI)
	add_subdirectory("xnpp-content-cli")
endif()

if(XNPP_BUILD_GAME_TEST)
add_subdirectory("xnpp-game-test")
endif()
