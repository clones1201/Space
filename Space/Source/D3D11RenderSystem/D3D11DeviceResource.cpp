#include "D3D11RenderSystem/D3D11Shared.hpp"
#include "D3D11RenderSystem/D3D11DeviceResource.hpp"
#include "D3D11RenderSystem/D3D11CommandList.hpp"
#include "DDSTextureLoader.h"

namespace Space {
	namespace Render {
		D3D11DeviceBuffer::D3D11DeviceBuffer(D3D11Device* device, BufferType type,
			ResourceUsage usage, byte const* initialData, size_t lengthInBytes)
		{
			D3D11_BUFFER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.ByteWidth = (UINT)lengthInBytes;
			desc.Usage = GetD3D11Usage(usage);
			desc.BindFlags = GetD3D11BufferBindFlags(type);

			D3D11_SUBRESOURCE_DATA data;
			ZeroMemory(&data, sizeof(data));
			data.pSysMem = initialData;

			m_pBuffer = nullptr;
			HRESULT hr = device->Get()->CreateBuffer(
				&desc,
				initialData != nullptr ? &data : nullptr,
				&(m_pBuffer.p));
			if (FAILED(hr))
			{
				throw std::exception("D3D11DeviceBufferImpl CreateBuffer failed.");
			}
		}

		bool D3D11DeviceBuffer::Update(
			D3D11CommandList* pCommandList,
			ResourceUsage usage,
			BufferType type,
			size_t lengthInTotal, size_t startOffset, size_t lengthInBytes,
			byte const* pData)
		{
			if (pData == nullptr)
				return false;

			if (startOffset + lengthInBytes > lengthInTotal)
				return false;

			bool isSuccess = false;
			HRESULT hr = S_OK;
			switch (usage)
			{
			case ResourceUsage::Default:
			{
				D3D11_BOX box; //buffer is a 1D dimension memory.
				box.left = (UINT)startOffset;
				box.right = (UINT)(startOffset + lengthInBytes);
				box.front = 0; box.back = 1;
				box.top = 0; box.bottom = 1;

				auto pBox = &box;
				if (type == BufferType::ConstantBuffer)
					pBox = nullptr;

				pCommandList->GetContext()->UpdateSubresource(m_pBuffer, 0, pBox, pData, 0, 0);

				isSuccess = true;
			}
			break;
			case ResourceUsage::Staging:
			case ResourceUsage::Dynamic:
			{
				D3D11_MAPPED_SUBRESOURCE mapped;
				hr = pCommandList->GetContext()->Map(m_pBuffer, 0, D3D11_MAP_WRITE, 0, &mapped);
				if (SUCCEEDED(hr))
				{
					auto error = memcpy_s((byte*)mapped.pData + startOffset, lengthInTotal, pData, lengthInBytes);

					if (error == 0) isSuccess = true;

					pCommandList->GetContext()->Unmap(m_pBuffer, 0);
				}
			}
			break;
			case ResourceUsage::Immutable:
			default:
				break;
			}
			return isSuccess;
		}

		D3D11DeviceTexture1D::D3D11DeviceTexture1D(
			D3D11Device* device,
			int32 X,
			DataFormat format,
			ResourceUsage usage,
			ResourceBindFlag flag,
			int32 arraySize,
			byte const* initialData):
			m_Device(device),
			m_DataFormat(format)
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
			HRESULT hr = device->Get()->CreateTexture1D(
				&m_Desc,
				initialData == nullptr ? nullptr : &data,
				&pTexture);
			if (FAILED(hr))
			{
				throw std::exception("ID3D11Device::CreateTexture1D failed");
			}
			m_pTexture1D = pTexture;
		}

		D3D11DeviceTexture1D::D3D11DeviceTexture1D(
			D3D11Device* device,
			D3D11_TEXTURE1D_DESC desc,
			CComPtr<ID3D11Texture1D> pTexture):
			m_Device(device)
		{
			if (pTexture == nullptr)
				throw std::exception("null ID3D11Texture3D interface pointer");

			m_pTexture1D = pTexture;
		}

		byte* D3D11DeviceTexture1D::Lock(D3D11CommandList* pCommandList)
		{
			int32 X;
			X = GetWidth();
			D3D11_USAGE usage = m_Desc.Usage;
			UINT pixel_size = (UINT)GetFormatSize(GetDataFormat(m_Desc.Format));
			switch (usage)
			{
			case D3D11_USAGE_DEFAULT:
				if (m_pLock != nullptr)
				{
					throw std::exception("lock twice is unacceptable");
				}
				m_pLock = new byte[sizeof(X *  pixel_size)];
				return m_pLock;
			case D3D11_USAGE_DYNAMIC:
			case D3D11_USAGE_STAGING:

				pCommandList->GetContext()->Map(m_pTexture1D, 0, D3D11_MAP_READ_WRITE, 0, &m_Subres);

				return (byte*)(m_Subres.pData);
			case D3D11_USAGE_IMMUTABLE:
			default:
				return nullptr;
			}

		}

		void D3D11DeviceTexture1D::Unlock(D3D11CommandList* pCommandList)
		{
			int32 X;
			X = GetWidth();
			D3D11_USAGE usage = m_Desc.Usage;
			UINT pixel_size = (UINT)GetFormatSize(GetDataFormat(m_Desc.Format));
			switch (usage)
			{
			case D3D11_USAGE_DEFAULT:
				if (m_pLock == nullptr)
				{
					throw std::exception("Unlock twice is unacceptable");
				}

				D3D11_BOX box;
				box.left = 0; box.right = X;
				box.top = 0; box.bottom = 1;
				box.front = 0; box.back = 1;
				pCommandList->GetContext()->UpdateSubresource(
					m_pTexture1D, 0, &box,
					m_pLock,
					UINT(X * pixel_size), UINT(X * pixel_size));

				delete[] m_pLock;
				m_pLock = nullptr;
				break;
			case D3D11_USAGE_DYNAMIC:
			case D3D11_USAGE_STAGING:
				pCommandList->GetContext()->Unmap(m_pTexture1D, 0);
				break;
			case D3D11_USAGE_IMMUTABLE:
			default:
				break;
			}
		}
		
		D3D11DeviceTexture2D::D3D11DeviceTexture2D(
			D3D11Device* device,
			int32 X, int32 Y,
			DataFormat format,
			ResourceUsage usage,
			ResourceBindFlag flag,
			int32 arraySize,
			byte const* initialData):
			m_Device(device),
			m_DataFormat(format)
		{
			if (arraySize < 0 || arraySize > 8)
			{
				throw std::exception("Wrong Texture Array Size.");
			}

			ZeroMemory(&m_Desc, sizeof(m_Desc));
			m_Desc.ArraySize = arraySize;
			m_Desc.Format = GetDXGIFormat(format);
			m_Desc.Width = X;
			m_Desc.Height = Y;
			m_Desc.Usage = (D3D11_USAGE)usage;
			m_Desc.BindFlags = GetD3D11BindFlag(flag);
			m_Desc.MipLevels = 1; 
			m_Desc.SampleDesc.Count = 1;
			m_Desc.SampleDesc.Quality = 0;		
			m_Desc.CPUAccessFlags = 0;
			m_Desc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA data;
			ZeroMemory(&data, sizeof(data));
			data.pSysMem = initialData;

			ID3D11Texture2D* pTexture = nullptr;
			HRESULT hr = m_Device->Get()->CreateTexture2D(
				&m_Desc,
				initialData == nullptr ? nullptr : &data,
				&pTexture);
			if (FAILED(hr))
			{
				throw std::exception("ID3D11Device::CreateTexture2D failed");
			}
			m_pTexture2D = pTexture;
		}

		D3D11DeviceTexture2D::D3D11DeviceTexture2D(
			D3D11Device* device,
			CComPtr<ID3D11Texture2D> pTexture):
			m_Device(device),
			m_pTexture2D(pTexture)
		{
			if (pTexture == nullptr)
				throw std::exception("null ID3D11Texture3D interface pointer");
			ZeroMemory(&m_Desc, sizeof(m_Desc));
			m_pTexture2D->GetDesc(&m_Desc);

			m_DataFormat = GetDataFormat(m_Desc.Format);
		}

		byte* D3D11DeviceTexture2D::Lock(D3D11CommandList* pCommandList)
		{
			int32 X, Y;
			X = GetWidth(); Y = GetHeight();
			D3D11_USAGE usage = m_Desc.Usage;
			UINT pixel_size = (UINT)GetFormatSize(GetDataFormat(m_Desc.Format));
			switch (usage)
			{
			case D3D11_USAGE_DEFAULT:
				if (m_pLock != nullptr)
				{
					throw std::exception("lock twice is unacceptable");
				}
				m_pLock = new byte[X * Y * pixel_size];
				return m_pLock;
			case D3D11_USAGE_DYNAMIC:
			case D3D11_USAGE_STAGING:

				pCommandList->GetContext()->Map(m_pTexture2D, 0, D3D11_MAP_READ_WRITE, 0, &m_Subres);

				return (byte*)(m_Subres.pData);
			case D3D11_USAGE_IMMUTABLE:
			default:
				return nullptr;
			}

		}

		void D3D11DeviceTexture2D::Unlock(D3D11CommandList* pCommandList)
		{
			int32 X, Y;
			X = GetWidth(); Y = GetHeight();
			D3D11_USAGE usage = m_Desc.Usage;
			UINT pixel_size = (UINT)GetFormatSize(GetDataFormat(m_Desc.Format));
			switch (usage)
			{
			case D3D11_USAGE_DEFAULT:
				if (m_pLock == nullptr)
				{
					throw std::exception("Unlock twice is unacceptable");
				}

				D3D11_BOX box;
				box.left = 0; box.right = X;
				box.top = 0; box.bottom = Y;
				box.front = 0; box.back = 1;
				pCommandList->GetContext()->UpdateSubresource(
					m_pTexture2D, 0, &box,
					m_pLock,
					UINT(X * pixel_size), UINT(X * Y * pixel_size));

				delete[] m_pLock;
				m_pLock = nullptr;
				break;
			case D3D11_USAGE_DYNAMIC:
			case D3D11_USAGE_STAGING:
				m_Device->GetImmediateContext()->Unmap(m_pTexture2D, 0);
				break;
			case D3D11_USAGE_IMMUTABLE:
			default:
				break;
			}
		}
		D3D11DeviceTexture3D::D3D11DeviceTexture3D(
			D3D11Device* device,
			int32 X, int32 Y, int32 Z,
			DataFormat format,
			ResourceUsage usage,
			ResourceBindFlag flag,
			byte const* initialData):
			m_Device(device),
			m_DataFormat(format)
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
			m_Desc.Format = GetDXGIFormat(format);

			D3D11_SUBRESOURCE_DATA data;
			ZeroMemory(&data, sizeof(data));
			data.pSysMem = initialData;

			ID3D11Texture3D* pTexture = nullptr;
			HRESULT hr = m_Device->Get()->CreateTexture3D(
				&m_Desc,
				initialData == nullptr ? nullptr : &data,
				&pTexture);
			if (FAILED(hr))
			{
				throw std::exception("ID3D11Device::CreateTexture3D failed");
			}
			m_pTexture3D = pTexture;
		}

		D3D11DeviceTexture3D::D3D11DeviceTexture3D(
			D3D11Device* device,
			D3D11_TEXTURE3D_DESC desc,
			ID3D11Texture3D* pTex)
			:m_Desc(desc), m_pTexture3D(pTex)
		{
		}

		byte* D3D11DeviceTexture3D::Lock(D3D11CommandList* pCommandList)
		{
			int32 X, Y, Z;
			X = GetWidth(); Y = GetHeight(); Z = GetDepth();
			D3D11_USAGE usage = m_Desc.Usage;
			UINT pixel_size = (UINT)GetFormatSize(GetDataFormat(m_Desc.Format));
			switch (usage)
			{
			case D3D11_USAGE_DEFAULT:
				if (m_pLock != nullptr)
				{
					throw std::exception("lock twice is unacceptable");
				}
				m_pLock = new byte[X * Y * Z * pixel_size];
				return m_pLock;
			case D3D11_USAGE_DYNAMIC:
			case D3D11_USAGE_STAGING:

				pCommandList->GetContext()->Map(m_pTexture3D, 0, D3D11_MAP_READ_WRITE, 0, &m_Subres);

				return (byte*)(m_Subres.pData);
			case D3D11_USAGE_IMMUTABLE:
			default:
				return nullptr;
			}

		}

		void D3D11DeviceTexture3D::Unlock(D3D11CommandList* pCommandList)
		{
			int32 X, Y, Z;
			X = GetWidth(); Y = GetHeight(); Z = GetDepth();
			D3D11_USAGE usage = m_Desc.Usage;
			UINT pixel_size = (UINT)GetFormatSize(GetDataFormat(m_Desc.Format));
			switch (usage)
			{
			case D3D11_USAGE_DEFAULT:
				if (m_pLock == nullptr)
				{
					throw std::exception("Unlock twice is unacceptable");
				}
				D3D11_BOX box;
				box.left = 0; box.right = X;
				box.top = 0; box.bottom = Y;
				box.front = 0; box.back = Z;
				pCommandList->GetContext()->UpdateSubresource(
					m_pTexture3D, 0, &box,
					m_pLock,
					UINT(X * pixel_size), UINT(X * Y * pixel_size));

				delete[] m_pLock;
				m_pLock = nullptr;
				break;
			case D3D11_USAGE_DYNAMIC:
			case D3D11_USAGE_STAGING:
				pCommandList->GetContext()->Unmap(m_pTexture3D, 0);
				break;
			case D3D11_USAGE_IMMUTABLE:
			default:
				break;
			}
		}
	}
}