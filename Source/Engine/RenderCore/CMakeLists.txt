set(RENDERCORE_ROOT_DIR ${PROJECT_SOURCE_DIR}/Source/Engine/RenderCore)

set(RENDERCORE_INC_FILES
	"${RENDERCORE_ROOT_DIR}/Public/RenderCommon.hpp"
	"${RENDERCORE_ROOT_DIR}/Public/DeviceResource.hpp"
	#"${RENDERCORE_ROOT_DIR}/Public/Prerequisites.hpp"
	"${RENDERCORE_ROOT_DIR}/Public/Rendering.hpp"
	"${RENDERCORE_ROOT_DIR}/Public/RenderSystem.hpp"
	"${RENDERCORE_ROOT_DIR}/Public/Shader.hpp"
	#"${RENDERCORE_ROOT_DIR}/Public/ShaderReflection.hpp"
	"${RENDERCORE_ROOT_DIR}/Public/Shared.hpp")
source_group("Public" FILES ${RENDERCORE_INC_FILES})

set(RENDERCORE_SRC_FILES
	"${RENDERCORE_ROOT_DIR}/Private/DeviceResource.cpp"
	"${RENDERCORE_ROOT_DIR}/Private/InputLayout.cpp"
	"${RENDERCORE_ROOT_DIR}/Private/RenderSystem.cpp"
	"${RENDERCORE_ROOT_DIR}/Private/Rendering.cpp"
	"${RENDERCORE_ROOT_DIR}/Private/Shader.cpp"
	"${RENDERCORE_ROOT_DIR}/Private/Shared.cpp")
source_group("Private" FILES ${RENDERCORE_SRC_FILES})

set(D3D11RENDERSYSTEM_INC_FILES
	"${RENDERCORE_ROOT_DIR}/D3D11RenderSystem/Public/D3D11Device.hpp"
	"${RENDERCORE_ROOT_DIR}/D3D11RenderSystem/Public/D3D11DeviceResource.hpp"
	"${RENDERCORE_ROOT_DIR}/D3D11RenderSystem/Public/D3D11Prerequisites.hpp"
	"${RENDERCORE_ROOT_DIR}/D3D11RenderSystem/Public/D3D11PipelineState.h"
	"${RENDERCORE_ROOT_DIR}/D3D11RenderSystem/Public/D3D11CommandList.h"
	"${RENDERCORE_ROOT_DIR}/D3D11RenderSystem/Public/D3D11RenderSystem.hpp"
	"${RENDERCORE_ROOT_DIR}/D3D11RenderSystem/Public/D3D11RenderTarget.hpp"
	"${RENDERCORE_ROOT_DIR}/D3D11RenderSystem/Public/D3D11ResourceView.hpp"
	"${RENDERCORE_ROOT_DIR}/D3D11RenderSystem/Public/D3D11Shader.hpp"
	"${RENDERCORE_ROOT_DIR}/D3D11RenderSystem/Public/D3D11Shared.hpp")
source_group("D3D11\\Public" FILES ${D3D11RENDERSYSTEM_INC_FILES})

set(D3D11RENDERSYSTEM_SRC_FILES
	"${RENDERCORE_ROOT_DIR}/D3D11RenderSystem/Private/D3D11CommandList.cpp"
	"${RENDERCORE_ROOT_DIR}/D3D11RenderSystem/Private/D3D11Device.cpp"
	"${RENDERCORE_ROOT_DIR}/D3D11RenderSystem/Private/D3D11DeviceResource.cpp"
	"${RENDERCORE_ROOT_DIR}/D3D11RenderSystem/Private/D3D11PipelineState.cpp" 
	"${RENDERCORE_ROOT_DIR}/D3D11RenderSystem/Private/D3D11RenderSystem.cpp"
	"${RENDERCORE_ROOT_DIR}/D3D11RenderSystem/Private/D3D11RenderTarget.cpp"
	"${RENDERCORE_ROOT_DIR}/D3D11RenderSystem/Private/D3D11ResourceView.cpp"
	"${RENDERCORE_ROOT_DIR}/D3D11RenderSystem/Private/D3D11Shader.cpp"
	"${RENDERCORE_ROOT_DIR}/D3D11RenderSystem/Private/D3D11ShaderReflection.cpp"
	"${RENDERCORE_ROOT_DIR}/D3D11RenderSystem/Private/D3D11Shared.cpp")
source_group("D3D11\\Private" FILES ${D3D11RENDERSYSTEM_SRC_FILES})

add_definitions(-DRENDERCORE_EXPORTS)

add_library("RenderCore" SHARED
	${RENDERCORE_INC_FILES}
	${RENDERCORE_SRC_FILES}
	${D3D11RENDERSYSTEM_INC_FILES}
	${D3D11RENDERSYSTEM_SRC_FILES})
	
add_dependencies(RenderCore Common Platform)
target_include_directories(RenderCore 
	PUBLIC "${RENDERCORE_ROOT_DIR}/Public"
	PUBLIC "${RENDERCORE_ROOT_DIR}/D3D11RenderSystem/Public")
target_link_libraries(RenderCore
	PUBLIC Common
	PUBLIC Platform
	PUBLIC d3d11
	PUBLIC dxgi
	PUBLIC d3dcompiler)