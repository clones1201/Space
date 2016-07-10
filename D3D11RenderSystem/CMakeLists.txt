include_directories(
	"${PROJECT_BINARY_DIR}/Common/Public"
	"${PROJECT_BINARY_DIR}/RenderSystem/Public"
	"${PROJECT_BINARY_DIR}/D3D11RenderSystem/Public"
	"${DXTK_DIR}/Inc")


add_definitions(-DSPACE_D3D11_EXPORTS -DNOMINMAX)

set(INC_FILES  
	"${PROJECT_BINARY_DIR}/D3D11RenderSystem/Public/RenderSystem/D3D11DepthStencilView.hpp"
	"${PROJECT_BINARY_DIR}/D3D11RenderSystem/Public/RenderSystem/D3D11Device.hpp"
	"${PROJECT_BINARY_DIR}/D3D11RenderSystem/Public/RenderSystem/D3D11DeviceBuffer.hpp"
	"${PROJECT_BINARY_DIR}/D3D11RenderSystem/Public/RenderSystem/D3D11DeviceTexture.hpp"
	"${PROJECT_BINARY_DIR}/D3D11RenderSystem/Public/RenderSystem/D3D11Rendering.hpp" 
	"${PROJECT_BINARY_DIR}/D3D11RenderSystem/Public/RenderSystem/D3D11RenderSystem.hpp"
	"${PROJECT_BINARY_DIR}/D3D11RenderSystem/Public/RenderSystem/D3D11RenderTarget.hpp"
	"${PROJECT_BINARY_DIR}/D3D11RenderSystem/Public/RenderSystem/D3D11RenderWindow.hpp"
	"${PROJECT_BINARY_DIR}/D3D11RenderSystem/Public/RenderSystem/D3D11Shader.hpp"
	"${PROJECT_BINARY_DIR}/D3D11RenderSystem/Public/RenderSystem/D3D11ShaderReflection.hpp"
	"${PROJECT_BINARY_DIR}/D3D11RenderSystem/Public/RenderSystem/D3D11ShaderResource.hpp"
	"${PROJECT_BINARY_DIR}/D3D11RenderSystem/Public/RenderSystem/D3D11Shared.hpp")

set(SRC_FILES 
	"${PROJECT_BINARY_DIR}/D3D11RenderSystem/Private/D3D11CommandList.cpp"
	"${PROJECT_BINARY_DIR}/D3D11RenderSystem/Private/D3D11DepthStencilView.cpp"
	"${PROJECT_BINARY_DIR}/D3D11RenderSystem/Private/D3D11Device.cpp"
	"${PROJECT_BINARY_DIR}/D3D11RenderSystem/Private/D3D11DeviceBuffer.cpp"
	"${PROJECT_BINARY_DIR}/D3D11RenderSystem/Private/D3D11DeviceTexture.cpp"
	"${PROJECT_BINARY_DIR}/D3D11RenderSystem/Private/D3D11PipelineState.cpp" 
	"${PROJECT_BINARY_DIR}/D3D11RenderSystem/Private/D3D11RenderSystem.cpp"
	"${PROJECT_BINARY_DIR}/D3D11RenderSystem/Private/D3D11RenderTarget.cpp"
	"${PROJECT_BINARY_DIR}/D3D11RenderSystem/Private/D3D11RenderWindow.cpp"
	"${PROJECT_BINARY_DIR}/D3D11RenderSystem/Private/D3D11Shader.cpp"
	"${PROJECT_BINARY_DIR}/D3D11RenderSystem/Private/D3D11ShaderReflection.cpp"
	"${PROJECT_BINARY_DIR}/D3D11RenderSystem/Private/D3D11ShaderResource.cpp"
	"${PROJECT_BINARY_DIR}/D3D11RenderSystem/Private/D3D11Shared.cpp")

add_library("Space.D3D11RenderSystem" SHARED ${SRC_FILES} ${INC_FILES})

target_link_libraries(Space.D3D11RenderSystem 
	PUBLIC Space.Common 
	PUBLIC Space.RenderSystem
	PUBLIC debug ${DXTK_LIB_DEBUG} 
	PUBLIC optimized ${DXTK_LIB_RELEASE}
	PUBLIC d3d11
	PUBLIC dxgi
	PUBLIC d3dcompiler)