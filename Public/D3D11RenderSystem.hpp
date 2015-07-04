#ifndef __SPACE_D3D11_RENDERSYSTEM_HPP__
#define __SPACE_D3D11_RENDERSYSTEM_HPP__

#include "RenderSystem.hpp"
#include "Basic.hpp"
#include "D3D11Prerequisites.hpp" 
#include "D3D11Device.hpp"

namespace Space
{
	class D3D11RenderSystem : public RenderSystem 
	{
	public:
		static D3D11RenderSystem* Create();
		D3D11Device* GetDevice() const throw();

		virtual RenderWindow* CreateRenderWindow(const std::string& title, int32 width, int32 height, bool fullscreen) throw();
		virtual DeviceBuffer* CreateBuffer(BufferType type, ResourceUsage usage, byte const* initialData, uint32 lengthInBytes) throw();
		virtual DeviceTexture1D* CreateTexture1D(int32 X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData) throw();
		virtual DeviceTexture1D* CreateTexture1DArray(int32 X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int32 arraySize, byte const* initialData) throw();
		virtual DeviceTexture2D* CreateTexture2D(int32 X, int32 Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData) throw();
		virtual DeviceTexture2D* CreateTexture2DArray(int32 X, int32 Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int32 arraySize, byte const* initialData) throw();
		virtual DeviceTexture2D* CreateTexture2DFromFile(std::string const& filename, DataFormat format, ResourceUsage usage, ResourceBindFlag flag) throw();
		virtual DeviceTexture3D* CreateTexture3D(int32 X, int32 Y, int32 Z, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData) throw();

		virtual VertexShader* LoadVertexShaderFromMemory(byte const* byteCodes, uint32 sizeInBytes) throw();
		virtual PixelShader* LoadPixelShaderFromMemory(byte const* byteCodes, uint32 sizeInBytes) throw();

		virtual DepthStencilView* CreateDepthStencilView(DeviceTexture2D* pTexture) throw();
		virtual RenderTarget* CreateRenderTarget(DeviceTexture2D* pTexture) throw();
		virtual ShaderResource* CreateShaderResource(DeviceTexture2D* pTexture);
		virtual ShaderResource* CreateShaderResource(TextureBuffer* pTBuffer);
		
		virtual CommandList* CreateCommandList();
		virtual PipelineState* CreatePipelineState();
		
		virtual void ExecuteCommandList(CommandList* list);

		friend class Core;

		virtual ~D3D11RenderSystem();
	protected:
		D3D11RenderSystem();

	private:
		D3D11DevicePtr mDevice;
	};

}

#endif 
