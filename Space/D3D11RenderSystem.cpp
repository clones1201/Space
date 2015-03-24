#include "D3D11Shared.hpp"
#include "D3D11Device.hpp"
#include "D3D11RenderSystemImpl.hpp"
#include "D3D11DeviceBuffer.hpp"
#include "D3D11DeviceTexture.hpp"
#include "D3D11RenderTarget.hpp"
#include "D3D11RenderWindow.hpp"
#include "Log.h"

namespace Space
{

	D3D11RenderSystem::D3D11RenderSystem()
	{		
	}
	 
	D3D11RenderSystem::~D3D11RenderSystem()
	{
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

	RenderWindow* D3D11RenderSystemImpl::CreateRenderWindow(const std::string& title, int width, int height, bool fullscreen)
	{
		return D3D11RenderWindow::Create(mDevice, title, width, height, fullscreen);
	}
	DeviceBuffer* D3D11RenderSystemImpl::CreateBuffer(BufferType type, ResourceUsage usage, byte const* initialData, size_t lengthInBytes)
	{
		return D3D11DeviceBuffer::Create(mDevice, type, usage, initialData, lengthInBytes);
	}

	DeviceTexture1D* D3D11RenderSystemImpl::CreateTexture1D(int X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData)
	{
		return D3D11DeviceTexture1D::Create(mDevice, X, format, usage, flag, initialData);
	}
	DeviceTexture1D* D3D11RenderSystemImpl::CreateTexture1DArray(int X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int arraySize, byte const* initialData)
	{
		return D3D11DeviceTexture1D::CreateArray(mDevice, X, format, usage, flag, arraySize, initialData);
	}
	DeviceTexture2D* D3D11RenderSystemImpl::CreateTexture2D(int X, int Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData)
	{
		return D3D11DeviceTexture2D::Create(mDevice, X, Y, format, usage, flag,initialData);
	}
	DeviceTexture2D* D3D11RenderSystemImpl::CreateTexture2DArray(int X, int Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int arraySize, byte const* initialData)
	{
		return D3D11DeviceTexture2D::Create(mDevice, X, Y, format, usage, flag, initialData);
	}
	DeviceTexture2D* D3D11RenderSystemImpl::CreateTexture2DFromFile(std::string const& filename)
	{
		return D3D11DeviceTexture2D::CreateFromFile(filename);
	}
	DeviceTexture3D* D3D11RenderSystemImpl::CreateTexture3D(int X, int Y, int Z, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData)
	{
		return D3D11DeviceTexture3D::Create(mDevice, X, Y, Z, format, usage, flag, initialData);
	}
}