#ifndef __SPACE_RENDERSYSTEM_RENDERSYSTEM_HPP__
#define __SPACE_RENDERSYSTEM_RENDERSYSTEM_HPP__

#include "Common/Common.hpp"

#include "RenderSystem/Prerequisites.hpp"

//#include "RenderSystem/ConstantBufferMap.hpp"
#include "RenderSystem/Shared.hpp"
#include "RenderSystem/DeviceResource.hpp"
#include "RenderSystem/Rendering.hpp"
#include "RenderSystem/Shader.hpp"
#include "RenderSystem/ShaderReflection.hpp"

namespace Space
{
	class SPACE_RENDERSYSTEM_API RenderSystem : private Interface
	{
	public:
		virtual ~RenderSystem();
	protected:
		RenderSystem();		 

	public:
		virtual RenderWindow* CreateRenderWindow(std::string const& name, int32 width, int32 height, bool fullscreen) = 0;
		virtual DeviceBuffer* CreateBuffer(BufferType type, ResourceUsage usage, byte const* initialData, size_t lengthInBytes) = 0;

		virtual DeviceTexture1D* CreateTexture1D(int32 X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData) = 0;
		virtual DeviceTexture1D* CreateTexture1DArray(int32 X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, size_t arraySize, byte const* initialData) = 0;
		virtual DeviceTexture2D* CreateTexture2D(int32 X, int32 Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData) = 0;
		virtual DeviceTexture2D* CreateTexture2DArray(int32 X, int32 Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, size_t arraySize, byte const* initialData) = 0;
		virtual DeviceTexture2D* CreateTexture2DFromFile(std::string const& filename, DataFormat format, ResourceUsage usage, ResourceBindFlag flag) = 0;
		virtual DeviceTexture3D* CreateTexture3D(int32 X, int32 Y, int32 Z, DataFormat format, ResourceUsage usage, ResourceBindFlag flag,byte const* initialData) = 0;

		virtual VertexShader* LoadVertexShaderFromMemory(byte const* byteCodes, size_t sizeInBytes) = 0;
		virtual PixelShader* LoadPixelShaderFromMemory(byte const* byteCodes, size_t sizeInBytes) = 0;
		
		virtual DepthStencilView* CreateDepthStencilView(DeviceTexture2D* pTexture) = 0;
		virtual RenderTarget* CreateRenderTarget(DeviceTexture2D* pTexture) = 0;
		virtual ShaderResource* CreateShaderResource(DeviceTexture2D* pTexture) = 0;
		virtual ShaderResource* CreateShaderResource(TextureBuffer* pTBuffer) = 0;
		
		virtual CommandList* CreateCommandList() = 0;
		virtual PipelineState* CreatePipelineState() = 0;

		virtual void ExecuteCommandList(CommandList* list) = 0;
	};
	typedef std::shared_ptr<RenderSystem> RenderSystemPtr;


	class SPACE_RENDERSYSTEM_API RenderTarget : private Interface
	{
	public:		
		virtual ~RenderTarget();

		inline uint32 GetWidth() const
		{
			return m_Width;
		}
		inline uint32 GetHeight() const
		{
			return m_Height;
		}
	protected:
		RenderTarget(uint32 width, uint32 height);

		uint32 m_Width, m_Height;
	};

	typedef std::shared_ptr<RenderTarget> RenderTargetPtr;

	class SPACE_RENDERSYSTEM_API RenderWindow : private Interface
	{
	public:
		virtual bool Initialize() = 0;

		virtual DeviceTexture2D* GetBackBuffer() throw() = 0;
		virtual void Present() = 0;
	protected:
		TypeTrait<Window>::Ptr m_pWindow;
		RenderWindow(const std::string& name, int32 width, int32 height, bool fullscreen) throw();

		std::string m_Name;
		int32 m_Width, m_Height;
		bool m_Fullscreen;
	};
	typedef std::shared_ptr<RenderWindow> RenderWindowPtr;
}

#endif