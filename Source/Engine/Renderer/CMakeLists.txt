
set(RENDERER_ROOT_DIR ${PROJECT_SOURCE_DIR}/Source/Engine/Renderer)


set(RENDERER_INC_FILES
	${RENDERER_ROOT_DIR}/Public/RendererCommon.h
#	${RENDERER_ROOT_DIR}/Public/Material.cpp
	${RENDERER_ROOT_DIR}/Public/Mesh.h
	${RENDERER_ROOT_DIR}/Public/Renderer.h
)

set(RENDERER_SRC_FILES
#	${RENDERER_ROOT_DIR}/Private/Material.h
	${RENDERER_ROOT_DIR}/Private/Mesh.cpp
	${RENDERER_ROOT_DIR}/Private/Renderer.cpp
)

add_definitions(-DRENDERER_EXPORTS -DNOMINMAX)
include_directories("${PROJECT_BINARY_DIR}/ThirdParty/rapidjson/rapidjson-master/include")

add_library("Renderer" SHARED
	${RENDERER_INC_FILES} ${RENDERER_SRC_FILES})
add_dependencies(Renderer 
	Common
	RenderCore
	Platform)
target_include_directories(Renderer
	PUBLIC "${RENDERER_ROOT_DIR}/Public")
target_link_libraries(Renderer
	PUBLIC Common
	PUBLIC Platform
	PUBLIC RenderCore)
