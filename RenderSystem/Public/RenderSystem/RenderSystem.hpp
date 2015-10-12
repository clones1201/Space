#ifndef __SPACE_RENDERSYSTEM_RENDERSYSTEM_HPP__
#define __SPACE_RENDERSYSTEM_RENDERSYSTEM_HPP__

#include "Common/Common.hpp"

#include "RenderSystem/Prerequisites.hpp"

//#include "RenderSystem/ConstantBufferMap.hpp"
#include "RenderSystem/Shared.hpp"
#include "RenderSystem/DepthStencilView.hpp"
#include "RenderSystem/DeviceBuffer.hpp"
#include "RenderSystem/DeviceTexture.hpp"
#include "RenderSystem/InputLayout.hpp"
#include "RenderSystem/Renderable.hpp"
#include "RenderSystem/Rendering.hpp"
#include "RenderSystem/RenderTarget.hpp"
#include "RenderSystem/RenderWindow.hpp"
#include "RenderSystem/ShaderCommon.hpp"
#include "RenderSystem/Shader.hpp"
#include "RenderSystem/ShaderReflection.hpp"
#include "RenderSystem/ShaderResource.hpp"

namespace Space
{
	class SPACE_RENDERSYSTEM_API RenderSystem : private Interface
	{
	public:
		virtual ~RenderSystem();
	protected:
		RenderSystem();		 

	public:
		virtual RenderWindow* CreateRenderWindow(std::string const& name, int32 width, int32 height, bool isFullScreen) = 0;
		virtual DeviceBuffer* CreateBuffer(BufferType type, ResourceUsage usage, byte const* initialData, uint32 lengthInBytes) = 0;

		virtual DeviceTexture1D* CreateTexture1D(int32 X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData) = 0;
		virtual DeviceTexture1D* CreateTexture1DArray(int32 X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int32 arraySize, byte const* initialData) = 0;
		virtual DeviceTexture2D* CreateTexture2D(int32 X, int32 Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData) = 0;
		virtual DeviceTexture2D* CreateTexture2DArray(int32 X, int32 Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int32 arraySize, byte const* initialData) = 0;
		virtual DeviceTexture2D* CreateTexture2DFromFile(std::string const& filename, DataFormat format, ResourceUsage usage, ResourceBindFlag flag) = 0;
		virtual DeviceTexture3D* CreateTexture3D(int32 X, int32 Y, int32 Z, DataFormat format, ResourceUsage usage, ResourceBindFlag flag,byte const* initialData) = 0;

		virtual VertexShader* LoadVertexShaderFromMemory(byte const* byteCodes, uint32 sizeInBytes) = 0;
		virtual PixelShader* LoadPixelShaderFromMemory(byte const* byteCodes, uint32 sizeInBytes) = 0;
		
		virtual DepthStencilView* CreateDepthStencilView(DeviceTexture2D* pTexture) = 0;
		virtual RenderTarget* CreateRenderTarget(DeviceTexture2D* pTexture) = 0;
		virtual ShaderResource* CreateShaderResource(DeviceTexture2D* pTexture) = 0;
		virtual ShaderResource* CreateShaderResource(TextureBuffer* pTBuffer) = 0;
		
		virtual CommandList* CreateCommandList() = 0;
		virtual PipelineState* CreatePipelineState() = 0;

		virtual void ExecuteCommandList(CommandList* list) = 0;
	};
	typedef std::shared_ptr<RenderSystem> RenderSystemPtr;
}

#endif