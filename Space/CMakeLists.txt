set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/Binary)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/Binary)

include_directories(
	"${PROJECT_BINARY_DIR}/Common/Public"
	"${PROJECT_BINARY_DIR}/RenderSystem/Public"
	"${PROJECT_BINARY_DIR}/D3D11RenderSystem/Public"
	"${PROJECT_BINARY_DIR}/Space/Public"
	"${ASSIMP_DIR}/include"
	"${RAPIDJSON_DIR}/include")

add_definitions(-DSPACE_EXPORTS -DNOMINMAX -DSPACE_GAME)

set(INC_FILES 
	"${PROJECT_BINARY_DIR}/Space/Public/Space/Behavior.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/Space/Game.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/Space/GameObject.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/Space/Material.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/Space/Mesh.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/Space/Prerequisites.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/Space/Renderer.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/Space/ResourcePool.h"
	"${PROJECT_BINARY_DIR}/Space/Public/Space/Scene.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/Space/ShaderResourcePool.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/Space/Space.hpp")

set(SRC_FILES 
	"${PROJECT_BINARY_DIR}/Space/Source/Component.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/Game.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/GameObject.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/Material.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/Mesh.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/Renderer.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/Scene.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/ShaderResourcePool.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/Space.cpp")

add_executable("Space" ${SRC_FILES} ${INC_FILES})

target_link_libraries(Space 
	PUBLIC Space.Common 
	PUBLIC Space.RenderSystem
	PUBLIC Space.D3D11RenderSystem
	PUBLIC debug ${DXTK_LIB_DEBUG} 
	PUBLIC optimized ${DXTK_LIB_RELEASE}
	PUBLIC debug ${ASSIMP_LIB_DEBUG}
	PUBLIC optimized ${ASSIMP_LIB_RELEASE}
	PUBLIC d3d11
	PUBLIC dxgi
	PUBLIC d3dcompiler)