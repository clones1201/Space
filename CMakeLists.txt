cmake_minimum_required(VERSION 2.6)
project(Space)

set(Space_VERSION_MAJOR 0)
set(Space_VERSION_MINOR 1)

add_definitions(-D_UNICODE -DUNICODE)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Binary)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Binary)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Binary)

if (WIN32)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251")
endif (WIN32)

#add_subdirectory(${PROJECT_SOURCE_DIR}/Source/ThirdParty)


add_subdirectory(${PROJECT_SOURCE_DIR}/Source/Engine/Common)
add_subdirectory(${PROJECT_SOURCE_DIR}/Source/Engine/Platform)
add_subdirectory(${PROJECT_SOURCE_DIR}/Source/Engine/RenderCore)
add_subdirectory(${PROJECT_SOURCE_DIR}/Source/Engine/Renderer)
add_subdirectory(${PROJECT_SOURCE_DIR}/Source/Engine/Space)

