set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/Binary)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/Binary)

include_directories(
	"${PROJECT_BINARY_DIR}/Space/Public"
	"${DXTK_DIR}/Inc"
	"${ASSIMP_DIR}/include"
	"${RAPIDJSON_DIR}/include")

add_definitions(-DSPACE_EXPORTS -DNOMINMAX -DSPACE_GAME)

set(COMMON_INC_FILES 
	"${PROJECT_BINARY_DIR}/Space/Public/Common/Basic.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/Common/Common.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/Common/ConfigReader.h"
	"${PROJECT_BINARY_DIR}/Space/Public/Common/Core.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/Common/Log.h"
	"${PROJECT_BINARY_DIR}/Space/Public/Common/Math.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/Common/Prerequisites.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/Common/Utility.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/Common/Window.hpp")
source_group(Common\\Header	FILES ${COMMON_INC_FILES})

set(RENDERSYSTEM_INC_FILES
	"${PROJECT_BINARY_DIR}/Space/Public/RenderSystem/DeviceResource.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/RenderSystem/Prerequisites.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/RenderSystem/Rendering.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/RenderSystem/RenderSystem.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/RenderSystem/ResourceView.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/RenderSystem/Shader.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/RenderSystem/ShaderReflection.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/RenderSystem/Shared.hpp")
source_group(RenderSystem\\Header FILES ${RENDERSYSTEM_INC_FILES})

set(D3D11RENDERSYSTEM_INC_FILES
	"${PROJECT_BINARY_DIR}/Space/Public/D3D11RenderSystem/D3D11Device.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/D3D11RenderSystem/D3D11DeviceResource.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/D3D11RenderSystem/D3D11Rendering.hpp" 
	"${PROJECT_BINARY_DIR}/Space/Public/D3D11RenderSystem/D3D11RenderSystem.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/D3D11RenderSystem/D3D11ResourceView.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/D3D11RenderSystem/D3D11Shader.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/D3D11RenderSystem/D3D11ShaderReflection.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/D3D11RenderSystem/D3D11Shared.hpp")
source_group(D3D11RenderSystem\\Header FILES ${D3D11RENDERSYSTEM_INC_FILES})

set(SPACE_INC_FILES
	"${PROJECT_BINARY_DIR}/Space/Public/Space/Game.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/Space/Material.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/Space/Mesh.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/Space/Prerequisites.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/Space/Renderer.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/Space/Scene.hpp"
	"${PROJECT_BINARY_DIR}/Space/Public/Space/Space.hpp")
source_group(Space\\Header FILES ${SPACE_INC_FILES})

set(COMMON_SRC_FILES
	"${PROJECT_BINARY_DIR}/Space/Source/Common/Basic.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/Common/Common.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/Common/ConfigReader.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/Common/Core.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/Common/Event.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/Common/Log.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/Common/Utility.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/Common/Window.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/Common/Win32Window.cpp")
source_group(Common\\Source FILES ${COMMON_SRC_FILES})

set(RENDERSYSTEM_SRC_FILES
	"${PROJECT_BINARY_DIR}/Space/Source/RenderSystem/DeviceResource.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/RenderSystem/InputLayout.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/RenderSystem/RenderSystem.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/RenderSystem/Rendering.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/RenderSystem/ResourceView.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/RenderSystem/Shader.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/RenderSystem/ShaderReflection.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/RenderSystem/Shared.cpp")
source_group(RenderSystem\\Source FILES ${RENDERSYSTEM_SRC_FILES})

set(D3D11RENDERSYSTEM_SRC_FILES
	"${PROJECT_BINARY_DIR}/Space/Source/D3D11RenderSystem/D3D11CommandList.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/D3D11RenderSystem/D3D11Device.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/D3D11RenderSystem/D3D11DeviceResource.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/D3D11RenderSystem/D3D11PipelineState.cpp" 
	"${PROJECT_BINARY_DIR}/Space/Source/D3D11RenderSystem/D3D11RenderSystem.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/D3D11RenderSystem/D3D11ResourceView.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/D3D11RenderSystem/D3D11Shader.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/D3D11RenderSystem/D3D11ShaderReflection.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/D3D11RenderSystem/D3D11Shared.cpp")
source_group(D3D11RenderSystem\\Source FILES ${D3D11RENDERSYSTEM_SRC_FILES})

set(SPACE_SRC_FILES
	"${PROJECT_BINARY_DIR}/Space/Source/Space/Material.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/Space/Mesh.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/Space/Renderer.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/Space/Scene.cpp"
	"${PROJECT_BINARY_DIR}/Space/Source/Space/Space.cpp")
source_group(Space\\Source FILES ${SPACE_SRC_FILES})

set(INC_FILES ${COMMON_INC_FILES} ${RENDERSYSTEM_INC_FILES} ${D3D11RENDERSYSTEM_INC_FILES} ${SPACE_INC_FILES})
set(SRC_FILES ${COMMON_SRC_FILES} ${RENDERSYSTEM_SRC_FILES} ${D3D11RENDERSYSTEM_SRC_FILES} ${SPACE_SRC_FILES})

add_library("Common" ${COMMON_SRC_FILES} ${COMMON_INC_FILES})
add_library("RenderSystem" ${RENDERSYSTEM_SRC_FILES} ${COMMON_INC_FILES})
add_library("D3D11RenderSystem" ${D3D11RENDERSYSTEM_SRC_FILES} ${D3D11RENDERSYSTEM_INC_FILES})
add_executable("Space" ${SPACE_SRC_FILES} ${SPACE_INC_FILES})

target_link_libraries(Space 
	PUBLIC Common
	PUBLIC RenderSystem
	PUBLIC D3D11RenderSystem
	PUBLIC debug ${DXTK_LIB_DEBUG} 
	PUBLIC optimized ${DXTK_LIB_RELEASE}
	PUBLIC debug ${ASSIMP_LIB_DEBUG}
	PUBLIC optimized ${ASSIMP_LIB_RELEASE}
	PUBLIC d3d11
	PUBLIC dxgi
	PUBLIC d3dcompiler)