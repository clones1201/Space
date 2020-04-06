set(COMMON_ROOT_DIR ${PROJECT_SOURCE_DIR}/Source/Engine/Common)

add_definitions(-DSPACE_COMMON_EXPORTS)

set(COMMON_INC_FILES 
	"${COMMON_ROOT_DIR}/Public/Basic.hpp"
	"${COMMON_ROOT_DIR}/Public/Common.hpp"
	"${COMMON_ROOT_DIR}/Public/ConfigReader.h"
	"${COMMON_ROOT_DIR}/Public/Log.h"
	"${COMMON_ROOT_DIR}/Public/Math.hpp"
	"${COMMON_ROOT_DIR}/Public/Prerequisites.hpp"
	"${COMMON_ROOT_DIR}/Public/Utility.hpp" 
	"${COMMON_ROOT_DIR}/Public/Asynchronous.hpp")
source_group("Pubilc" FILES ${COMMON_INC_FILES})

set(COMMON_SRC_FILES
	"${COMMON_ROOT_DIR}/Private/Basic.cpp"
	"${COMMON_ROOT_DIR}/Private/Common.cpp"
	"${COMMON_ROOT_DIR}/Private/ConfigReader.cpp"
	"${COMMON_ROOT_DIR}/Private/Log.cpp"
	"${COMMON_ROOT_DIR}/Private/Utility.cpp"
	"${COMMON_ROOT_DIR}/Private/Core.cpp")
source_group("Private" FILES ${COMMON_SRC_FILES})

add_library("Common" SHARED ${COMMON_SRC_FILES} ${COMMON_INC_FILES})
target_include_directories(Common PUBLIC "${COMMON_ROOT_DIR}/Public")
