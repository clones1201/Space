#include "Log.h"
#include "D3D11Shared.hpp"
#include "D3D11Device.hpp"
#include "D3D11DeviceTexture.hpp"
#include "D3D11RenderTarget.hpp"
#include "D3D11DepthStencilView.hpp"

namespace Space
{
	class D3D11DeviceTexture1DImpl : public D3D11DeviceTexture1D
	{
	private:
		TextureType m_Type;
		DataFormat m_Format;
		ResourceUsage m_Usage;
		ResourceBindFlag m_Flag;

		CComPtr<ID3D11Texture1D> m_pTexture1D = nullptr;

		D3D11_TEXTURE1D_DESC m_Desc;

		D3D11Device& mDevice;
	public:
		D3D11DeviceTexture1DImpl( 
			D3D11Device& device,
			int X,
			DataFormat format,
			ResourceUsage usage,
			ResourceBindFlag flag,
			int arraySize,
			byte const* initialData
			) :
			m_Type( arraySize > 1 ? TT_Texture1DArray : TT_Texture1D ),
			m_Format(format), m_Usage(usage),m_Flag(flag), mDevice(device)
		{
			if (arraySize < 0 || arraySize > 8)
			{
				throw std::exception("Wrong Texture Array Size.");
			}

			ZeroMemory(&m_Desc, sizeof(m_Desc));
			m_Desc.ArraySize = arraySize;
			m_Desc.Width = X;
			m_Desc.Usage = (D3D11_USAGE)usage;
			m_Desc.BindFlags = GetD3D11BindFlag(flag);
			m_Desc.MipLevels = 0;

			D3D11_SUBRESOURCE_DATA data;
			ZeroMemory(&data, sizeof(data));
			data.pSysMem = initialData;

			ID3D11Texture1D* pTexture = nullptr;
			HRESULT hr = mDevice->CreateTexture1D(
				&m_Desc,
				initialData == nullptr ? nullptr : &data,
				&pTexture);
			if (FAILED(hr))
			{
				throw std::exception("ID3D11Device::CreateTexture1D failed");
			}
			m_pTexture1D = pTexture;
		}
		
		virtual int GetArraySize() const
		{
			return (m_Desc.ArraySize);
		}
		virtual TextureType GetType() const
		{
			return m_Type;
		}
	};

	class D3D11DeviceTexture2DImpl : public D3D11DeviceTexture2D
	{
	private:
		TextureType m_Type;
		DataFormat m_Format;
		ResourceUsage m_Usage;

		CComPtr<ID3D11Texture2D> m_pTexture2D = nullptr;

		D3D11_TEXTURE2D_DESC m_Desc;

		D3D11Device& mDevice;
	public:
		D3D11DeviceTexture2DImpl(
			D3D11Device& device,
			int X,int Y,
			DataFormat format,
			ResourceUsage usage,
			ResourceBindFlag flag,
			int arraySize,
			byte const* initialData
			) :
			m_Type(arraySize > 1 ? TT_Texture2DArray : TT_Texture2D),
			m_Format(format), m_Usage(usage), mDevice(device)
		{
			if (arraySize < 0 || arraySize > 8)
			{
				throw std::exception("Wrong Texture Array Size.");
			}

			ZeroMemory(&m_Desc, sizeof(m_Desc));
			m_Desc.ArraySize = arraySize;
			m_Desc.Width = X;
			m_Desc.Height = Y;
			m_Desc.Usage = (D3D11_USAGE)usage;
			m_Desc.BindFlags = GetD3D11BindFlag(flag);
			m_Desc.MipLevels = 0;

			D3D11_SUBRESOURCE_DATA data;
			ZeroMemory(&data, sizeof(data));
			data.pSysMem = initialData;

			ID3D11Texture2D* pTexture = nullptr;
			HRESULT hr = mDevice->CreateTexture2D(
				&m_Desc,
				initialData == nullptr ? nullptr : &data,
				&pTexture);
			if (FAILED(hr))
			{
				throw std::exception("ID3D11Device::CreateTexture2D failed");
			}
			m_pTexture2D = pTexture;
		}

		virtual RenderTarget* GetRenderTarget() const
		{
			return D3D11RenderTarget::Create(mDevice, m_pTexture2D);
		}
		virtual DepthStencilView* GetDepthStencil() const
		{
			return D3D11DepthStencilView::Create(mDevice,m_pTexture2D);
		}

		virtual int GetArraySize() const
		{
			return (m_Desc.ArraySize);
		}
		virtual TextureType GetType() const
		{
			return m_Type;
		}
	};

	class D3D11DeviceTexture3DImpl : public D3D11DeviceTexture3D
	{
	private:
		TextureType m_Type;
		DataFormat m_Format;
		ResourceUsage m_Usage;
		ResourceBindFlag m_Flag;

		CComPtr<ID3D11Texture3D> m_pTexture3D = nullptr;

		D3D11_TEXTURE3D_DESC m_Desc;

		D3D11Device& mDevice;
	public:
		D3D11DeviceTexture3DImpl(
			D3D11Device& device,
			int X, int Y, int Z, 
			DataFormat format,
			ResourceUsage usage, 
			ResourceBindFlag flag,
			byte const* initialData
			) :
			m_Type(TT_Texture3D),
			m_Format(format), m_Usage(usage), mDevice(device)
		{
			/*if (arraySize < 0 || arraySize > 8)
			{
				throw std::exception("Wrong Texture Array Size.");
			}*/

			ZeroMemory(&m_Desc, sizeof(m_Desc));
			//m_Desc.ArraySize = arraySize;
			m_Desc.Width = X; m_Desc.Height = Y; m_Desc.Depth = Z;
			m_Desc.Usage = (D3D11_USAGE)usage;
			m_Desc.BindFlags = GetD3D11BindFlag(flag);
			m_Desc.MipLevels = 0;

			D3D11_SUBRESOURCE_DATA data;
			ZeroMemory(&data, sizeof(data));
			data.pSysMem = initialData;

			ID3D11Texture3D* pTexture = nullptr;
			HRESULT hr = mDevice->CreateTexture3D(
				&m_Desc,
				initialData == nullptr ? nullptr : &data,
				&pTexture);
			if (FAILED(hr))
			{
				throw std::exception("ID3D11Device::CreateTexture3D failed");
			}
			m_pTexture3D = pTexture;
		}
		virtual int GetArraySize() const
		{
			return 1;
		}
		virtual TextureType GetType() const
		{
			return m_Type;
		}
	};

	D3D11DeviceTexture1D* D3D11DeviceTexture1D::CreateArray(D3D11Device &device, int X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int arraySize, byte const* initialData)
	{
		try
		{
			return new D3D11DeviceTexture1DImpl(device, X, format, usage, flag, arraySize, initialData);
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}
	D3D11DeviceTexture2D* D3D11DeviceTexture2D::CreateArray(D3D11Device &device, int X, int Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int arraySize, byte const* initialData)
	{
		try
		{
			return new D3D11DeviceTexture2DImpl(device, X,Y, format, usage, flag, arraySize, initialData);
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}
	/*D3D11DeviceTexture3D* D3D11DeviceTexture3D::CreateArray(D3D11Device &device, int X, int Y, int Z, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int arraySize, byte const* initialData)
	{
		try
		{
			return new D3D11DeviceTexture3DImpl(device, X, format, usage, arraySize, initialData);
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}*/
	D3D11DeviceTexture1D* D3D11DeviceTexture1D::Create(D3D11Device &device, int X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData)
	{
		try
		{
			return new D3D11DeviceTexture1DImpl(device, X, format, usage, flag, 1, initialData);
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}
	D3D11DeviceTexture2D* D3D11DeviceTexture2D::Create(D3D11Device &device, int X, int Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData)
	{
		try
		{
			return new D3D11DeviceTexture2DImpl(device, X,Y, format, usage, flag, 1, initialData);
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}
	D3D11DeviceTexture3D* D3D11DeviceTexture3D::Create(D3D11Device &device, int X, int Y, int Z, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData)
	{
		try
		{
			return new D3D11DeviceTexture3DImpl(device, X,Y,Z, format, usage, flag, initialData);
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}

}