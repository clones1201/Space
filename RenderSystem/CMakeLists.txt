include_directories(
	"${PROJECT_BINARY_DIR}/Common/Public"
	"${PROJECT_BINARY_DIR}/RenderSystem/Public")

add_definitions(-DSPACE_RENDERSYSTEM_EXPORTS -DNOMINMAX)

set(INC_FILES 
	"${PROJECT_BINARY_DIR}/RenderSystem/Public/RenderSystem/ConstantBufferMap.hpp"
	"${PROJECT_BINARY_DIR}/RenderSystem/Public/RenderSystem/DepthStencilView.hpp"
	"${PROJECT_BINARY_DIR}/RenderSystem/Public/RenderSystem/DeviceBuffer.hpp"
	"${PROJECT_BINARY_DIR}/RenderSystem/Public/RenderSystem/DeviceTexture.hpp"
	"${PROJECT_BINARY_DIR}/RenderSystem/Public/RenderSystem/InputLayout.hpp"
	"${PROJECT_BINARY_DIR}/RenderSystem/Public/RenderSystem/Prerequisites.hpp"
	"${PROJECT_BINARY_DIR}/RenderSystem/Public/RenderSystem/Renderable.hpp"
	"${PROJECT_BINARY_DIR}/RenderSystem/Public/RenderSystem/Rendering.hpp"
	"${PROJECT_BINARY_DIR}/RenderSystem/Public/RenderSystem/RenderSystem.hpp"
	"${PROJECT_BINARY_DIR}/RenderSystem/Public/RenderSystem/RenderTarget.hpp"
	"${PROJECT_BINARY_DIR}/RenderSystem/Public/RenderSystem/RenderWindow.hpp"
	"${PROJECT_BINARY_DIR}/RenderSystem/Public/RenderSystem/Shader.hpp"
	"${PROJECT_BINARY_DIR}/RenderSystem/Public/RenderSystem/ShaderCommon.hpp"
	"${PROJECT_BINARY_DIR}/RenderSystem/Public/RenderSystem/ShaderReflection.hpp"
	"${PROJECT_BINARY_DIR}/RenderSystem/Public/RenderSystem/ShaderResource.hpp"
	"${PROJECT_BINARY_DIR}/RenderSystem/Public/RenderSystem/Shared.hpp")

set(SRC_FILES 
	"${PROJECT_BINARY_DIR}/RenderSystem/Private/DepthStencilView.cpp"
	"${PROJECT_BINARY_DIR}/RenderSystem/Private/DeviceBuffer.cpp"
	"${PROJECT_BINARY_DIR}/RenderSystem/Private/DeviceTexture.cpp"
	"${PROJECT_BINARY_DIR}/RenderSystem/Private/InputLayout.cpp"
	"${PROJECT_BINARY_DIR}/RenderSystem/Private/Renderable.cpp"
	"${PROJECT_BINARY_DIR}/RenderSystem/Private/RenderSystem.cpp"
	"${PROJECT_BINARY_DIR}/RenderSystem/Private/Rendering.cpp"
	"${PROJECT_BINARY_DIR}/RenderSystem/Private/RenderTarget.cpp"
	"${PROJECT_BINARY_DIR}/RenderSystem/Private/RenderWindow.cpp"
	"${PROJECT_BINARY_DIR}/RenderSystem/Private/Shader.cpp"
	"${PROJECT_BINARY_DIR}/RenderSystem/Private/ShaderReflection.cpp"
	"${PROJECT_BINARY_DIR}/RenderSystem/Private/ShaderResource.cpp"
	"${PROJECT_BINARY_DIR}/RenderSystem/Private/Shared.cpp")

add_library("Space.RenderSystem" SHARED ${SRC_FILES} ${INC_FILES})

target_link_libraries(Space.RenderSystem PUBLIC Space.Common)