cmake_minimum_required(VERSION 2.6)
project(Space)

set(Space_VERSION_MAJOR 0)
set(Space_VERSION_MINOR 1)

add_definitions(-D_UNICODE -DUNICODE)

set(DXSDK_DIR $ENV{DXSDK_DIR} CACHE STRING "path to directX sdk" FORCE)
set(ASSIMP_DIR $ENV{ASSIMP_DIR} CACHE STRING "path to assimp library" FORCE)
set(RAPIDJSON_DIR $ENV{RAPIDJSON_DIR} CACHE STRING "path to rapidJSON library" FORCE)
set(DXTK_DIR $ENV{DXTK_DIR} CACHE STRING "path to DirectXTK library" FORCE)

find_library(DXTK_LIB_DEBUG NAMES DirectXTK PATHS "${DXTK_DIR}/Bin/Desktop_2015/x64/Debug")
find_library(DXTK_LIB_RELEASE NAMES DirectXTK PATHS "${DXTK_DIR}/Bin/Desktop_2015/x64/Release")

find_library(ASSIMP_LIB_DEBUG NAMES assimp-vc140-mtd PATHS "${ASSIMP_DIR}/lib/Debug")
find_library(ASSIMP_LIB_RELEASE NAMES assimp-vc140-mt PATHS "${ASSIMP_DIR}/lib/Release")

set(ASSIMP_BIN_DEBUG ${ASSIMP_DIR})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/Binary)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/Binary)

if (WIN32)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251")
endif (WIN32)

add_subdirectory(${PROJECT_BINARY_DIR}/Space)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	file(COPY 
		"${ASSIMP_DIR}/bin/Debug/assimp-vc140-mtd.dll"
		DESTINATION ${EXECUTABLE_OUTPUT_PATH})
else()
	file(COPY 
		"${ASSIMP_DIR}/bin/Release/assimp-vc140-mt.dll"
		DESTINATION ${EXECUTABLE_OUTPUT_PATH})
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")