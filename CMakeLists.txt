cmake_minimum_required(VERSION 3.12)

project("XnppFramework" VERSION 0.1.0 LANGUAGES CXX)

option(XNPP_BUILD_LIB "Build Xnpp library" ON)
option(XNPP_BUILD_CONTENTPIPELINE "Build Xnpp ContentPipeline" ON)
option(XNPP_BUILD_GAME_TEST "Build Xnpp Game Test" ON)

if (XNPP_BUILD_LIB)
	add_subdirectory("xnpp-framework")
endif()

if (XNPP_BUILD_CONTENTPIPELINE)
	add_subdirectory("xnpp-content-pipeline")
endif()

if(XNPP_BUILD_GAME_TEST)
add_subdirectory("xnpp-game-test")
endif()
