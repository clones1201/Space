#include "D3D11RenderSystem/D3D11RenderSystem.hpp"
#include "D3D11RenderSystem/D3D11DeviceResource.hpp"
#include "D3D11RenderSystem/D3D11Shader.hpp"
#include "D3D11RenderSystem/D3D11Rendering.hpp"
#include "D3D11RenderSystem/D3D11ResourceView.hpp"

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
		:mDevice(new D3D11Device())
	{
		if (mDevice == nullptr)
		{
			//try second time
			mDevice.reset(new D3D11Device());
		}

		if (mDevice == nullptr)
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
		byte const* initialData, size_t lengthInBytes)
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
		size_t arraySize, byte const* initialData)
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
		int32 X, int32 Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, size_t arraySize, byte const* initialData)
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
		byte const* byteCodes, size_t sizeInBytes) throw()
	{
		return D3D11VertexShader::LoadBinaryFromMemory(mDevice,
			byteCodes, sizeInBytes);
	}

	PixelShader* D3D11RenderSystem::LoadPixelShaderFromMemory(
		byte const* byteCodes, size_t sizeInBytes) throw()
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
			mDevice->GetImmediateContext()
				->ExecuteCommandList(pList->GetList(), true);
		}
	}


		D3D11RenderTarget::D3D11RenderTarget(
		D3D11DevicePtr device, DeviceTexture2D* pBackBuffer)
		:mDevice(device), 
		RenderTarget(pBackBuffer->GetWidth(), pBackBuffer->GetHeight())
	{
			assert(nullptr != dynamic_cast<D3D11DeviceTexture2D*>(pBackBuffer));

			auto pD3DDeviceTexture2D = static_cast<D3D11DeviceTexture2D*>(pBackBuffer);
			if (pD3DDeviceTexture2D == nullptr)
				throw std::exception("Wrong DeviceTexture2D pointer type");

			CComPtr<ID3D11Texture2D> pBuffer = pD3DDeviceTexture2D->GetD3DTexture2D();

			D3D11_TEXTURE2D_DESC texDesc;
			pBuffer->GetDesc(&texDesc);
			
			if (texDesc.ArraySize != 1)
			{
				throw std::exception("Texture2D array size incorrect.");
			}
			
			ZeroMemory(&m_RTVDesc, sizeof(m_RTVDesc));
			m_RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			m_RTVDesc.Texture2D.MipSlice = 0;
			m_RTVDesc.Format = texDesc.Format;

			ID3D11RenderTargetView* pRenderTargetView = nullptr;
			HRESULT hr = mDevice->Get()->CreateRenderTargetView(pBuffer, &m_RTVDesc, &pRenderTargetView);
			if (FAILED(hr))
			{
				throw std::exception("CreateRenderTargetView failed.");
			}
			m_pRenderTargetView = pRenderTargetView;
		}
 
	D3D11RenderTarget* D3D11RenderTarget::Create(D3D11DevicePtr device,DeviceTexture2D* pBackBuffer)
	{
		try
		{
			if (pBackBuffer == nullptr)
				throw std::exception("Null DeviceTexture2D pointer");

			return new D3D11RenderTarget(device, pBackBuffer);
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}

	
	D3D11RenderWindow::D3D11RenderWindow(
		D3D11DevicePtr device, 
		const std::string& name, int32 width, int32 height, bool fullscreen)
		:mDevice(device), RenderWindow(name, width, height, fullscreen)
	{
	}

	DeviceTexture2D* D3D11RenderWindow::GetBackBuffer()
	{
		return m_pBackBuffer.get();
	}

	bool D3D11RenderWindow::Initialize()
	{
		auto window = dynamic_cast<Win32Window*>(m_pWindow.get());
		if (window == nullptr)
		{
			throw std::exception("Window type error.");
		}

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = window->Width();
		sd.BufferDesc.Height = window->Height();
		sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = window->GetHandle();
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = !RenderWindow::m_Fullscreen;

		IDXGISwapChain* pSwapChain = nullptr;
		HRESULT hr = mDevice->GetDXGIFactory()->CreateSwapChain(mDevice->Get(), &sd, &(pSwapChain));
		if (FAILED(hr)){
			DebugLog("RenderWindow Initialize Failed");
			return false;
		}
		m_pSwapChain = pSwapChain;

		ID3D11Texture2D* pBackBuffer = nullptr;
		hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&(pBackBuffer));
		if (FAILED(hr))
		{
			Log("Get Back Buffer Failed\n");
			return false;
		}
		m_pBackBuffer.reset(D3D11DeviceTexture2D::Create(mDevice, pBackBuffer));
		window->Show();
		return true;
	}

	void D3D11RenderWindow::Present()
	{
		if (m_pSwapChain != nullptr)
		{
			m_pSwapChain->Present(0, 0);
		}
	}

	void D3D11RenderWindow::Resize(int32 width, int32 height)
	{
		m_pSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

		ID3D11Texture2D* pBackBuffer = nullptr;
		HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&(pBackBuffer));
		if (FAILED(hr))
		{
			Log("Get Back Buffer Failed\n");
			return;
		}
		m_pBackBuffer.reset(D3D11DeviceTexture2D::Create(mDevice, pBackBuffer));
	}


	D3D11RenderWindow* D3D11RenderWindow::Create(D3D11DevicePtr mDevice, const std::string& name, int32 width, int32 height, bool fullscreen)
	{
		try
		{
			return new D3D11RenderWindow(mDevice, name, width, height, fullscreen);
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}
}