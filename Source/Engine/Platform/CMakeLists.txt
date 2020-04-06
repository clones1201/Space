set(PLATFORM_ROOT_DIR ${PROJECT_SOURCE_DIR}/Source/Engine/Platform)

set(PLATFORM_INC_FILES 
	"${PLATFORM_ROOT_DIR}/Public/Window.hpp"
	"${PLATFORM_ROOT_DIR}/Public/PlatformCommon.h")
source_group("Pubilc" FILES ${PLATFORM_INC_FILES})

set(PLATFORM_SRC_FILES
	"${PLATFORM_ROOT_DIR}/Private/Window.cpp"
	"${PLATFORM_ROOT_DIR}/Windows/Win32Window.cpp"
	"${PLATFORM_ROOT_DIR}/Private/Event.cpp"
	)
source_group("Private" FILES ${PLATFORM_SRC_FILES})

add_definitions("-DPLATFORM_EXPORTS")
add_library("Platform" SHARED ${PLATFORM_SRC_FILES} ${PLATFORM_INC_FILES})
add_dependencies(Platform Common)
target_include_directories(Platform PUBLIC ${PLATFORM_ROOT_DIR}/Public)
target_link_libraries(Platform Common)