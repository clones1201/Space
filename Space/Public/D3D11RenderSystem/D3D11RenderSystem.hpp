#ifndef __SPACE_D3D11_RENDERSYSTEM_HPP__
#define __SPACE_D3D11_RENDERSYSTEM_HPP__

#include "RenderSystem/RenderSystem.hpp"

#include "D3D11RenderSystem/D3D11Prerequisites.hpp" 
#include "D3D11RenderSystem/D3D11Device.hpp"

namespace Space
{
	class SPACE_D3D11_API D3D11RenderSystem : public RenderSystem
	{
	public:
		static D3D11RenderSystem* Create();
		D3D11Device* GetDevice() const throw();

		virtual RenderWindow* CreateRenderWindow(const std::string& title, int32 width, int32 height, bool fullscreen) throw();
		virtual DeviceBuffer* CreateBuffer(BufferType type, ResourceUsage usage, byte const* initialData, size_t lengthInBytes) throw();
		virtual DeviceTexture1D* CreateTexture1D(int32 X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData) throw();
		virtual DeviceTexture1D* CreateTexture1DArray(int32 X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, size_t arraySize, byte const* initialData) throw();
		virtual DeviceTexture2D* CreateTexture2D(int32 X, int32 Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData) throw();
		virtual DeviceTexture2D* CreateTexture2DArray(int32 X, int32 Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, size_t arraySize, byte const* initialData) throw();
		virtual DeviceTexture2D* CreateTexture2DFromFile(std::string const& filename, DataFormat format, ResourceUsage usage, ResourceBindFlag flag) throw();
		virtual DeviceTexture3D* CreateTexture3D(int32 X, int32 Y, int32 Z, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData) throw();

		virtual VertexShader* LoadVertexShaderFromMemory(byte const* byteCodes, size_t sizeInBytes) throw();
		virtual PixelShader* LoadPixelShaderFromMemory(byte const* byteCodes, size_t sizeInBytes) throw();

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


	class SPACE_D3D11_API D3D11RenderTarget : public RenderTarget
	{
	public:
		inline ID3D11RenderTargetView* GetRenderTargetView() const
		{
			return (m_pRenderTargetView.p);
		}
		
		static D3D11RenderTarget* Create(D3D11DevicePtr device, DeviceTexture2D* pBackBuffer);
	protected:
		D3D11RenderTarget(D3D11DevicePtr device, DeviceTexture2D* pBackBuffer);

		D3D11DevicePtr mDevice;

		D3D11_RENDER_TARGET_VIEW_DESC m_RTVDesc;

		CComPtr<ID3D11RenderTargetView> m_pRenderTargetView = nullptr;
		//CComPtr<ID3D11Texture2D> m_pBackBuffer = nullptr; 
	};

	class SPACE_D3D11_API D3D11RenderWindow :
		public RenderWindow
	{
	public:
		virtual DeviceTexture2D* GetBackBuffer() throw();
		virtual void Present();
		virtual void Resize(int32 width, int32 height);
		virtual bool Initialize();
		
		friend class D3D11RenderSystem;
		friend class D3D11RenderWindow;
	
		static D3D11RenderWindow* Create(D3D11DevicePtr device, const std::string& name, int32 width, int32 height, bool fullscreen) throw();
	protected:
		D3D11RenderWindow(D3D11DevicePtr device, const std::string& name, int32 width, int32 height, bool fullscreen);

	private:
		D3D11DevicePtr mDevice;

		TypeTrait<DeviceTexture2D>::Ptr m_pBackBuffer = nullptr;
		CComPtr<IDXGISwapChain> m_pSwapChain = nullptr;
	};
}

#endif 
