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
#include "D3D11ShaderResource.hpp"
#include "D3D11Rendering.hpp"

namespace Space
{	 
	D3D11RenderSystem::~D3D11RenderSystem()
	{
	}
		
	D3D11RenderSystem* D3D11RenderSystem::Create()
	{
		TRY_CATCH_LOG(
			return new D3D11RenderSystem(),
			return nullptr
		);
	}

	D3D11RenderSystem::D3D11RenderSystem()
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

	D3D11Device* D3D11RenderSystem::GetDevice() const
	{
		return (D3D11Device*)&mDevice;
	}

	RenderWindow* D3D11RenderSystem::CreateRenderWindow(
		const std::string& title, int32 width, int32 height, bool fullscreen)
	{
		return D3D11RenderWindow::Create(mDevice, title, width, height, fullscreen);
	}
	DeviceBuffer* D3D11RenderSystem::CreateBuffer(
		BufferType type, ResourceUsage usage, 
		byte const* initialData, uint32 lengthInBytes)
	{
		return D3D11DeviceBuffer::Create(mDevice, 
			type, usage, initialData, lengthInBytes);
	}

	DeviceTexture1D* D3D11RenderSystem::CreateTexture1D(
		int32 X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, 
		byte const* initialData)
	{
		return D3D11DeviceTexture1D::Create(mDevice, 
			X, format, usage, flag, initialData);
	}
	DeviceTexture1D* D3D11RenderSystem::CreateTexture1DArray(
		int32 X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, 
		int32 arraySize, byte const* initialData)
	{
		return D3D11DeviceTexture1D::CreateArray(mDevice, 
			X, format, usage, flag, arraySize, initialData);
	}
	DeviceTexture2D* D3D11RenderSystem::CreateTexture2D(
		int32 X, int32 Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData)
	{
		return D3D11DeviceTexture2D::Create(mDevice, 
			X, Y, format, usage, flag,initialData);
	}
	DeviceTexture2D* D3D11RenderSystem::CreateTexture2DArray(
		int32 X, int32 Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int32 arraySize, byte const* initialData)
	{
		return D3D11DeviceTexture2D::Create(mDevice, 
			X, Y, format, usage, flag, initialData);
	}
	DeviceTexture2D* D3D11RenderSystem::CreateTexture2DFromFile(
		std::string const& filename, DataFormat format, ResourceUsage usage, ResourceBindFlag flag)
	{
		return D3D11DeviceTexture2D::CreateFromFile(mDevice,filename, format, usage, flag);
	}
	DeviceTexture3D* D3D11RenderSystem::CreateTexture3D(
		int32 X, int32 Y, int32 Z, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData)
	{
		return D3D11DeviceTexture3D::Create(mDevice, 
			X, Y, Z, format, usage, flag, initialData);
	}

	VertexShader* D3D11RenderSystem::LoadVertexShaderFromMemory(
		byte const* byteCodes, uint32 sizeInBytes) throw()
	{
		return D3D11VertexShader::LoadBinaryFromMemory(mDevice,
			byteCodes, sizeInBytes);
	}

	PixelShader* D3D11RenderSystem::LoadPixelShaderFromMemory(
		byte const* byteCodes, uint32 sizeInBytes) throw()
	{
		return D3D11PixelShader::LoadBinaryFromMemory(mDevice, 
			byteCodes, sizeInBytes);
	}

	DepthStencilView* D3D11RenderSystem::CreateDepthStencilView(
		DeviceTexture2D* pTexture) throw()
	{
		return D3D11DepthStencilView::Create(mDevice, pTexture);
	}

	RenderTarget* D3D11RenderSystem::CreateRenderTarget(DeviceTexture2D* pTexture) throw()
	{
		return D3D11RenderTarget::Create(mDevice, pTexture);
	}
	
	ShaderResource* D3D11RenderSystem::CreateShaderResource(
		DeviceTexture2D* pTexture)
	{
		return D3D11ShaderResource::Create(mDevice, pTexture);
	}
	ShaderResource* D3D11RenderSystem::CreateShaderResource(
		TextureBuffer* pTBuffer)
	{
		return D3D11ShaderResource::Create(mDevice, pTBuffer);
	}

	CommandList* D3D11RenderSystem::CreateCommandList()
	{
		return D3D11CommandList::Create(mDevice);
	}

	PipelineState* D3D11RenderSystem::CreatePipelineState()
	{
		return D3D11PipelineState::Create(mDevice);
	}

	void D3D11RenderSystem::ExecuteCommandList(CommandList* list)
	{
		assert(nullptr != dynamic_cast<D3D11CommandList*>(list));
		auto pList = static_cast<D3D11CommandList*>(list);
		if (nullptr != pList->GetList())
		{
			mDevice.GetImmediateContext()
				->ExecuteCommandList(pList->GetList(), true);
		}
	}

}