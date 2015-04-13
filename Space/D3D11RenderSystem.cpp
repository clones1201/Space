#include "Log.h"
#include "D3D11RenderSystem.hpp"
#include "D3D11Shared.hpp"
#include "D3D11Device.hpp"
#include "D3D11DeviceBuffer.hpp"
#include "D3D11DeviceTexture.hpp"
#include "D3D11DepthStencilView.hpp"
#include "D3D11RenderTarget.hpp"
#include "D3D11RenderWindow.hpp"
#include "D3D11Shader.hpp"

namespace Space
{

	class D3D11RenderSystemImpl : public D3D11RenderSystem
	{
	private:
		D3D11Device mDevice;

	public:
		D3D11RenderSystemImpl();
		~D3D11RenderSystemImpl(){}

		D3D11Device* GetDevice() const throw();

		virtual RenderWindow* CreateRenderWindow(const std::string& title, int32 width, int32 height, bool fullscreen) throw();
		virtual DeviceBuffer* CreateBuffer(BufferType type, ResourceUsage usage, byte const* initialData, size_t lengthInBytes) throw();
		virtual DeviceTexture1D* CreateTexture1D(int32 X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData) throw();
		virtual DeviceTexture1D* CreateTexture1DArray(int32 X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int32 arraySize, byte const* initialData) throw();
		virtual DeviceTexture2D* CreateTexture2D(int32 X, int32 Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData) throw();
		virtual DeviceTexture2D* CreateTexture2DArray(int32 X, int32 Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int32 arraySize, byte const* initialData) throw();
		virtual DeviceTexture2D* CreateTexture2DFromFile(std::string const& filename, DataFormat format, ResourceUsage usage, ResourceBindFlag flag) throw();
		virtual DeviceTexture3D* CreateTexture3D(int32 X, int32 Y, int32 Z, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData) throw();

		/*virtual VertexShader* LoadVertexShaderFromFile(std::string const& filename) throw();
		virtual VertexShader* LoadVertexShaderFromFile(std::wstring const& filename) throw();*/
		virtual VertexShader* LoadVertexShaderFromMemory(byte const* byteCodes, uint32 sizeInBytes) throw();

		/*virtual PixelShader* LoadPixelShaderFromFile(std::string const& filename) throw();
		virtual PixelShader* LoadPixelShaderFromFile(std::wstring const& filename) throw();*/
		virtual PixelShader* LoadPixelShaderFromMemory(byte const* byteCodes, uint32 sizeInBytes) throw();

		virtual DepthStencilView* CreateDepthStencilView(DeviceTexture2D* pTexture) throw();
		virtual RenderTarget* CreateRenderTarget(DeviceTexture2D* pTexture) throw();
	};

	D3D11RenderSystem::D3D11RenderSystem()
	{		
	}
	 
	D3D11RenderSystem::~D3D11RenderSystem()
	{
	}
		
	D3D11RenderSystem* D3D11RenderSystem::Create()
	{
		try
		{
			return new D3D11RenderSystemImpl();
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}

	D3D11RenderSystemImpl::D3D11RenderSystemImpl()
		:mDevice()
	{
		if (!mDevice.IsValid())
		{
			//try second time
			mDevice = D3D11Device();
		}

		if (!mDevice.IsValid())
		{
			throw std::exception("D3D11Device failed.");
		}
	}

	D3D11Device* D3D11RenderSystemImpl::GetDevice() const
	{
		return (D3D11Device*)&mDevice;
	}

	RenderWindow* D3D11RenderSystemImpl::CreateRenderWindow(const std::string& title, int32 width, int32 height, bool fullscreen)
	{
		return D3D11RenderWindow::Create(mDevice, title, width, height, fullscreen);
	}
	DeviceBuffer* D3D11RenderSystemImpl::CreateBuffer(BufferType type, ResourceUsage usage, byte const* initialData, size_t lengthInBytes)
	{
		return D3D11DeviceBuffer::Create(mDevice, type, usage, initialData, lengthInBytes);
	}

	DeviceTexture1D* D3D11RenderSystemImpl::CreateTexture1D(int32 X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData)
	{
		return D3D11DeviceTexture1D::Create(mDevice, X, format, usage, flag, initialData);
	}
	DeviceTexture1D* D3D11RenderSystemImpl::CreateTexture1DArray(int32 X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int32 arraySize, byte const* initialData)
	{
		return D3D11DeviceTexture1D::CreateArray(mDevice, X, format, usage, flag, arraySize, initialData);
	}
	DeviceTexture2D* D3D11RenderSystemImpl::CreateTexture2D(int32 X, int32 Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData)
	{
		return D3D11DeviceTexture2D::Create(mDevice, X, Y, format, usage, flag,initialData);
	}
	DeviceTexture2D* D3D11RenderSystemImpl::CreateTexture2DArray(int32 X, int32 Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int32 arraySize, byte const* initialData)
	{
		return D3D11DeviceTexture2D::Create(mDevice, X, Y, format, usage, flag, initialData);
	}
	DeviceTexture2D* D3D11RenderSystemImpl::CreateTexture2DFromFile(std::string const& filename, DataFormat format, ResourceUsage usage, ResourceBindFlag flag)
	{
		return D3D11DeviceTexture2D::CreateFromFile(mDevice,filename, format, usage, flag);
	}
	DeviceTexture3D* D3D11RenderSystemImpl::CreateTexture3D(int32 X, int32 Y, int32 Z, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData)
	{
		return D3D11DeviceTexture3D::Create(mDevice, X, Y, Z, format, usage, flag, initialData);
	}

	VertexShader* D3D11RenderSystemImpl::LoadVertexShaderFromMemory(byte const* byteCodes, uint32 sizeInBytes) throw()
	{
		return D3D11VertexShader::LoadBinaryFromMemory(mDevice,byteCodes, sizeInBytes);
	}

	PixelShader* D3D11RenderSystemImpl::LoadPixelShaderFromMemory(byte const* byteCodes, uint32 sizeInBytes) throw()
	{
		return D3D11PixelShader::LoadBinaryFromMemory(mDevice, byteCodes, sizeInBytes);
	}

	DepthStencilView* D3D11RenderSystemImpl::CreateDepthStencilView(DeviceTexture2D* pTexture) throw()
	{
		return D3D11DepthStencilView::Create(mDevice, pTexture);
	}

	RenderTarget* D3D11RenderSystemImpl::CreateRenderTarget(DeviceTexture2D* pTexture) throw()
	{
		return D3D11RenderTarget::Create(mDevice, pTexture);
	}

}