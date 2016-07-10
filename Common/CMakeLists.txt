set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/Binary)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/Binary)

include_directories("${PROJECT_BINARY_DIR}/Common/Public")

add_definitions(-DSPACE_COMMON_EXPORTS -DNOMINMAX)

set(INC_FILES 
	"${PROJECT_BINARY_DIR}/Common/Public/Common/Basic.hpp"
	"${PROJECT_BINARY_DIR}/Common/Public/Common/Common.hpp"
	"${PROJECT_BINARY_DIR}/Common/Public/Common/ConfigReader.h"
	"${PROJECT_BINARY_DIR}/Common/Public/Common/Core.hpp"
	"${PROJECT_BINARY_DIR}/Common/Public/Common/Event.hpp"
	"${PROJECT_BINARY_DIR}/Common/Public/Common/Log.h"
	"${PROJECT_BINARY_DIR}/Common/Public/Common/Math.hpp"
	"${PROJECT_BINARY_DIR}/Common/Public/Common/Prerequisites.hpp"
	"${PROJECT_BINARY_DIR}/Common/Public/Common/TemplateTrait.hpp"
	"${PROJECT_BINARY_DIR}/Common/Public/Common/Utility.hpp"
	"${PROJECT_BINARY_DIR}/Common/Public/Common/Win32Window.hpp"
	"${PROJECT_BINARY_DIR}/Common/Public/Common/Window.hpp")

set(SRC_FILES 
	"${PROJECT_BINARY_DIR}/Common/Private/Basic.cpp"
	"${PROJECT_BINARY_DIR}/Common/Private/Common.cpp"
	"${PROJECT_BINARY_DIR}/Common/Private/ConfigReader.cpp"
	"${PROJECT_BINARY_DIR}/Common/Private/Core.cpp"
	"${PROJECT_BINARY_DIR}/Common/Private/Event.cpp"
	"${PROJECT_BINARY_DIR}/Common/Private/Log.cpp"
	"${PROJECT_BINARY_DIR}/Common/Private/Utility.cpp"
	"${PROJECT_BINARY_DIR}/Common/Private/Win32Window.cpp"
	"${PROJECT_BINARY_DIR}/Common/Private/Window.cpp")

add_library("Space.Common" SHARED ${SRC_FILES} ${INC_FILES})

