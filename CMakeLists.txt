cmake_minimum_required(VERSION 3.12)

project("XnppFramework" VERSION 0.1.0 LANGUAGES CXX)

option(XNPP_BUILD_LIB "Build Xnpp library" ON)
option(XNPP_BUILD_APP "Build Xnpp app" ON)

if (XNPP_BUILD_LIB)
	add_subdirectory("xnpp-framework")
endif()

if (XNPP_BUILD_APP)
	add_subdirectory("xnpp-content-pipeline")
endif()
