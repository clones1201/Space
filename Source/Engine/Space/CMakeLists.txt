set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/Binary)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/Binary)

set(SPACE_ROOT_DIR ${PROJECT_SOURCE_DIR}/Source/Engine/Space)

add_definitions(-DSPACE_EXPORTS -DNOMINMAX -DSPACE_GAME)

set(SPACE_INC_FILES
	"${SPACE_ROOT_DIR}/Public/Space.hpp")

set(SPACE_SRC_FILES
	"${SPACE_ROOT_DIR}/Private/Space.cpp")

add_executable("Space"
	${SPACE_SRC_FILES} ${SPACE_INC_FILES})

add_dependencies(Space 
	Common
	RenderCore)

target_include_directories(Space PUBLIC ${SPACE_ROOT_DIR}/Public)
target_link_libraries(Space
	PUBLIC Common
	PUBLIC RenderCore)