#include "Log.h"
#include "Utility.hpp"
#include "D3D11Shared.hpp"
#include "D3D11Device.hpp"
#include "D3D11DeviceTexture.hpp"
#include "D3D11RenderTarget.hpp"
#include "D3D11DepthStencilView.hpp"

#include "DDSTextureLoader.h"

namespace Space
{
	class D3D11DeviceTexture1DImpl : public D3D11DeviceTexture1D
	{
	private:
		CComPtr<ID3D11Texture1D> m_pTexture1D = nullptr;

		D3D11_TEXTURE1D_DESC m_Desc;

		D3D11DevicePtr mDevice;
	public:
		D3D11DeviceTexture1DImpl(
			D3D11DevicePtr device,
			int32 X,
			DataFormat format,
			ResourceUsage usage,
			ResourceBindFlag flag,
			int32 arraySize,
			byte const* initialData
			) :
			D3D11DeviceTexture1D(X, format, usage, flag, arraySize),
			mDevice(device)
		{
			if (arraySize < 0 || arraySize > 8)
			{
				throw std::exception("Wrong Texture Array Size.");
			}

			ZeroMemory(&m_Desc, sizeof(m_Desc));
			m_Desc.ArraySize = arraySize;
			m_Desc.Width = m_Width;
			m_Desc.Usage = (D3D11_USAGE)usage;
			m_Desc.BindFlags = GetD3D11BindFlag(flag);
			m_Desc.MipLevels = 0;

			D3D11_SUBRESOURCE_DATA data;
			ZeroMemory(&data, sizeof(data));
			data.pSysMem = initialData;

			ID3D11Texture1D* pTexture = nullptr;
			HRESULT hr = mDevice->Get()->CreateTexture1D(
				&m_Desc,
				initialData == nullptr ? nullptr : &data,
				&pTexture);
			if (FAILED(hr))
			{
				throw std::exception("ID3D11Device::CreateTexture1D failed");
			}
			m_pTexture1D = pTexture;
		}

		D3D11DeviceTexture1DImpl(D3D11DevicePtr device,
			D3D11_TEXTURE1D_DESC desc,
			ID3D11Texture1D* pTexture)
			:D3D11DeviceTexture1D(
			desc.Width,
			GetDataFormat(desc.Format), (ResourceUsage)desc.Usage, Space::GetBindFlag(desc.BindFlags), desc.ArraySize),
			m_Desc(desc),
			mDevice(device)
		{
			if (pTexture == nullptr)
				throw std::exception("null ID3D11Texture3D interface pointer");

			m_pTexture1D = pTexture;
		}

		virtual ID3D11Texture1D* GetD3DTexture1D()
		{
			return (m_pTexture1D.p);
		}
	private:
		byte* m_pLock = nullptr;
		D3D11_MAPPED_SUBRESOURCE m_Subres;
	public:

		virtual byte* Lock()
		{
			switch (m_Usage)
			{
			case ResourceUsage::Default:
				if (m_pLock != nullptr)
				{
					throw std::exception("lock twice is unacceptable");
				}
				m_pLock = new byte[sizeof(m_Width *  GetFormatSize(m_Format))];
				return m_pLock;
			case ResourceUsage::Dynamic:
			case ResourceUsage::Staging:

				mDevice->GetImmediateContext()->Map(m_pTexture1D, 0, D3D11_MAP_READ_WRITE, 0, &m_Subres);

				return (byte*)(m_Subres.pData);
			case ResourceUsage::Immutable:
			default:
				return nullptr;
			}

		}

		virtual void Unlock()
		{
			switch (m_Usage)
			{
			case ResourceUsage::Default:
				if (m_pLock == nullptr)
				{
					throw std::exception("Unlock twice is unacceptable");
				}

				D3D11_BOX box;
				box.left = 0; box.right = m_Width;
				box.top = 0; box.bottom = 1;
				box.front = 0; box.back = 1;
				mDevice->GetImmediateContext()->UpdateSubresource(
					m_pTexture1D, 0, &box,
					m_pLock,
					m_Width * GetFormatSize(m_Format), m_Width * GetFormatSize(m_Format));

				delete[] m_pLock;
				m_pLock = nullptr;
				break;
			case ResourceUsage::Dynamic:
			case ResourceUsage::Staging:
				mDevice->GetImmediateContext()->Unmap(m_pTexture1D, 0);
				break;
			case ResourceUsage::Immutable:
			default:
				break;
			}
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

		D3D11DevicePtr mDevice;
	public:
		D3D11DeviceTexture2DImpl(
			D3D11DevicePtr device,
			int32 X, int32 Y,
			DataFormat format,
			ResourceUsage usage,
			ResourceBindFlag flag,
			int32 arraySize,
			byte const* initialData
			) :
			D3D11DeviceTexture2D(X, Y, format, usage, flag, arraySize),
			mDevice(device)
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
			HRESULT hr = mDevice->Get()->CreateTexture2D(
				&m_Desc,
				initialData == nullptr ? nullptr : &data,
				&pTexture);
			if (FAILED(hr))
			{
				throw std::exception("ID3D11Device::CreateTexture2D failed");
			}
			m_pTexture2D = pTexture;
		}

		D3D11DeviceTexture2DImpl(D3D11DevicePtr device,
			D3D11_TEXTURE2D_DESC desc,
			ID3D11Texture2D* pTexture)
			:D3D11DeviceTexture2D(
			desc.Width, desc.Height,
			GetDataFormat(desc.Format), (ResourceUsage)desc.Usage, Space::GetBindFlag(desc.BindFlags), desc.ArraySize),
			m_Desc(desc),
			mDevice(device)
		{
			if (pTexture == nullptr)
				throw std::exception("null ID3D11Texture3D interface pointer");

			m_pTexture2D = pTexture;
		}

		virtual ID3D11Texture2D* GetD3DTexture2D()
		{
			return (m_pTexture2D.p);
		}

	private:
		byte* m_pLock = nullptr;
		D3D11_MAPPED_SUBRESOURCE m_Subres;
	public:

		virtual byte* Lock()
		{
			switch (m_Usage)
			{
			case ResourceUsage::Default:
				if (m_pLock != nullptr)
				{
					throw std::exception("lock twice is unacceptable");
				}
				m_pLock = new byte[sizeof(m_Width * m_Height * GetFormatSize(m_Format))];
				return m_pLock;
			case ResourceUsage::Dynamic:
			case ResourceUsage::Staging:

				mDevice->GetImmediateContext()->Map(m_pTexture2D, 0, D3D11_MAP_READ_WRITE, 0, &m_Subres);

				return (byte*)(m_Subres.pData);
			case ResourceUsage::Immutable:
			default:
				return nullptr;
			}

		}

		virtual void Unlock()
		{
			switch (m_Usage)
			{
			case ResourceUsage::Default:
				if (m_pLock == nullptr)
				{
					throw std::exception("Unlock twice is unacceptable");
				}

				D3D11_BOX box;
				box.left = 0; box.right = m_Width;
				box.top = 0; box.bottom = m_Height;
				box.front = 0; box.back = 1;
				mDevice->GetImmediateContext()->UpdateSubresource(
					m_pTexture2D, 0, &box,
					m_pLock,
					m_Width * GetFormatSize(m_Format), m_Width * m_Height * GetFormatSize(m_Format));

				delete[] m_pLock;
				m_pLock = nullptr;
				break;
			case ResourceUsage::Dynamic:
			case ResourceUsage::Staging:
				mDevice->GetImmediateContext()->Unmap(m_pTexture2D, 0);
				break;
			case ResourceUsage::Immutable:
			default:
				break;
			}
		}
	};

	class D3D11DeviceTexture3DImpl : public D3D11DeviceTexture3D
	{
	private:
		CComPtr<ID3D11Texture3D> m_pTexture3D = nullptr;

		D3D11_TEXTURE3D_DESC m_Desc;

		D3D11DevicePtr mDevice;
	public:
		D3D11DeviceTexture3DImpl(
			D3D11DevicePtr device,
			int32 X, int32 Y, int32 Z,
			DataFormat format,
			ResourceUsage usage,
			ResourceBindFlag flag,
			byte const* initialData
			) :
			D3D11DeviceTexture3D(X, Y, Z, format, usage, flag),
			mDevice(device)
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
			HRESULT hr = mDevice->Get()->CreateTexture3D(
				&m_Desc,
				initialData == nullptr ? nullptr : &data,
				&pTexture);
			if (FAILED(hr))
			{
				throw std::exception("ID3D11Device::CreateTexture3D failed");
			}
			m_pTexture3D = pTexture;
		}

		D3D11DeviceTexture3DImpl(D3D11DevicePtr device,
			D3D11_TEXTURE3D_DESC desc,
			ID3D11Texture3D* pTex)
			:D3D11DeviceTexture3D(
			desc.Width, desc.Height, desc.Depth,
			GetDataFormat(desc.Format), (ResourceUsage)desc.Usage, Space::GetBindFlag(desc.BindFlags)),
			m_Desc(desc),
			mDevice(device)
		{
			if (m_pTexture3D == nullptr)
				throw std::exception("null ID3D11Texture3D interface pointer");

			m_pTexture3D = pTex;
		}
	private:
		byte* m_pLock = nullptr;
		D3D11_MAPPED_SUBRESOURCE m_Subres;
	public:

		virtual byte* Lock()
		{
			switch (m_Usage)
			{
			case ResourceUsage::Default:
				if (m_pLock != nullptr)
				{
					throw std::exception("lock twice is unacceptable");
				}
				m_pLock = new byte[sizeof(m_Width * m_Height * m_Depth * GetFormatSize(m_Format))];
				return m_pLock;
			case ResourceUsage::Dynamic:
			case ResourceUsage::Staging:

				mDevice->GetImmediateContext()->Map(m_pTexture3D, 0, D3D11_MAP_READ_WRITE, 0, &m_Subres);

				return (byte*)(m_Subres.pData);
			case ResourceUsage::Immutable:
			default:
				return nullptr;
			}

		}

		virtual void Unlock()
		{
			switch (m_Usage)
			{
			case ResourceUsage::Default:
				if (m_pLock == nullptr)
				{
					throw std::exception("Unlock twice is unacceptable");
				}

				D3D11_BOX box;
				box.left = 0; box.right = m_Width;
				box.top = 0; box.bottom = m_Height;
				box.front = 0; box.back = m_Depth;
				mDevice->GetImmediateContext()->UpdateSubresource(
					m_pTexture3D, 0, &box,
					m_pLock,
					m_Width * GetFormatSize(m_Format), m_Width * m_Height * GetFormatSize(m_Format));

				delete[] m_pLock;
				m_pLock = nullptr;
				break;
			case ResourceUsage::Dynamic:
			case ResourceUsage::Staging:
				mDevice->GetImmediateContext()->Unmap(m_pTexture3D, 0);
				break;
			case ResourceUsage::Immutable:
			default:
				break;
			}
		}

		virtual ID3D11Texture3D* GetD3DTexture3D()
		{
			return (m_pTexture3D.p);
		}
	};

	D3D11DeviceTexture1D* D3D11DeviceTexture1D::CreateArray(D3D11DevicePtr device, int32 X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int32 arraySize, byte const* initialData)
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

	D3D11DeviceTexture1D::D3D11DeviceTexture1D(
		int32 X,
		DataFormat format,
		ResourceUsage usage,
		ResourceBindFlag flag,
		int32 arraySize)
		:DeviceTexture1D(X, format, usage, flag, arraySize)
	{}

	D3D11DeviceTexture1D* D3D11DeviceTexture1D::Create(D3D11DevicePtr device, int32 X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData)
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

	D3D11DeviceTexture1D* D3D11DeviceTexture1D::Create(D3D11DevicePtr device, ID3D11Texture1D* pTexture)
	{
		try
		{
			D3D11_TEXTURE1D_DESC desc;
			pTexture->GetDesc(&desc);
			return new D3D11DeviceTexture1DImpl(device, desc, pTexture);
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}

	D3D11DeviceTexture2D::D3D11DeviceTexture2D(
		int32 X, int32 Y,
		DataFormat format,
		ResourceUsage usage,
		ResourceBindFlag flag,
		int32 arraySize)
		:DeviceTexture2D(X, Y, format, usage, flag, arraySize)
	{}

	D3D11DeviceTexture2D* D3D11DeviceTexture2D::CreateArray(D3D11DevicePtr device, int32 X, int32 Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int32 arraySize, byte const* initialData)
	{
		try
		{
			return new D3D11DeviceTexture2DImpl(device, X, Y, format, usage, flag, arraySize, initialData);
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}

	D3D11DeviceTexture2D* D3D11DeviceTexture2D::Create(D3D11DevicePtr device, int32 X, int32 Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData)
	{
		try
		{
			return new D3D11DeviceTexture2DImpl(device, X, Y, format, usage, flag, 1, initialData);
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}

	D3D11DeviceTexture2D* D3D11DeviceTexture2D::Create(D3D11DevicePtr device, ID3D11Texture2D* pTexture)
	{
		try
		{
			D3D11_TEXTURE2D_DESC desc;
			pTexture->GetDesc(&desc);
			return new D3D11DeviceTexture2DImpl(device, desc, pTexture);
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}

	D3D11DeviceTexture2D* D3D11DeviceTexture2D::CreateFromFile(D3D11DevicePtr device, std::string const& filename, DataFormat format, ResourceUsage usage, ResourceBindFlag flag)
	{
		return CreateFromFile(device, str2wstr(filename), format, usage, flag);
	}

	D3D11DeviceTexture2D* D3D11DeviceTexture2D::CreateFromFile(D3D11DevicePtr device, std::wstring const& filename, DataFormat format, ResourceUsage usage, ResourceBindFlag flag)
	{
		ID3D11Texture2D* pTexture = nullptr;
		auto hr = DirectX::CreateDDSTextureFromFileEx(device->Get(), filename.c_str(), filename.size(), (D3D11_USAGE)usage,
			(UINT)GetD3D11BindFlag(flag), 0, 0, true, (ID3D11Resource**)&pTexture, nullptr);
		if (FAILED(hr))
		{
			return nullptr;
		}

		try
		{
			D3D11_TEXTURE2D_DESC desc;
			pTexture->GetDesc(&desc);
			return new D3D11DeviceTexture2DImpl(device, desc, pTexture);
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}

	D3D11DeviceTexture3D::D3D11DeviceTexture3D(
		int32 X, int32 Y, int32 Z,
		DataFormat format,
		ResourceUsage usage,
		ResourceBindFlag flag)
		:DeviceTexture3D(X, Y, Z, format, usage, flag)
	{
	}

	D3D11DeviceTexture3D* D3D11DeviceTexture3D::Create(D3D11DevicePtr device, int32 X, int32 Y, int32 Z, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData)
	{
		try
		{
			return new D3D11DeviceTexture3DImpl(device, X, Y, Z, format, usage, flag, initialData);
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}

	D3D11DeviceTexture3D* D3D11DeviceTexture3D::Create(D3D11DevicePtr device, ID3D11Texture3D* pTexture)
	{
		try
		{
			D3D11_TEXTURE3D_DESC desc;
			pTexture->GetDesc(&desc);
			return new D3D11DeviceTexture3DImpl(device, desc, pTexture);
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}
}