#include "Log.h"
#include "D3D11Shared.hpp"
#include "D3D11DeviceBuffer.hpp"

namespace Space
{
	D3D11DeviceBuffer::D3D11DeviceBuffer(D3D11Device& device, BufferType type,
		ResourceUsage usage, byte const* initialData, uint32 lengthInBytes)
		:mDevice(device), DeviceBuffer(type, usage, lengthInBytes)
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ByteWidth = m_LengthInBytes;
		desc.Usage = GetD3D11Usage(m_Usage);
		desc.BindFlags = GetD3D11BufferBindFlags(type);

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = initialData;

		ID3D11Buffer* pBuffer = nullptr;
		HRESULT hr = mDevice->CreateBuffer(
			&desc,
			initialData != nullptr ? &data : nullptr,
			&pBuffer);
		if (FAILED(hr))
		{
			throw std::exception("D3D11DeviceBufferImpl CreateBuffer failed.");
		}
	}

	bool D3D11DeviceBuffer::Update(uint32 startOffset, uint32 lengthInBytes, byte const* pData)
	{
		if (pData == nullptr)
			return false;

		if (startOffset + lengthInBytes > m_LengthInBytes)
			return false;

		bool isSuccess = false;
		HRESULT hr = S_OK;
		switch (m_Usage)
		{
		case RU_Default:
		{
			D3D11_BOX box; //buffer is a 1D dimension memory.
			box.left = startOffset;
			box.right = startOffset + lengthInBytes;
			box.front = 0; box.back = 1;
			box.top = 0; box.bottom = 1;

			mDevice.GetImmediateContext()->UpdateSubresource(m_pBuffer, 0, &box, pData, 0, 0);

			isSuccess = true;
		}
		break;
		case RU_Staging:
		case RU_Dynamic:
		{
			D3D11_MAPPED_SUBRESOURCE mapped;
			hr = mDevice.GetImmediateContext()->Map(m_pBuffer, 0, D3D11_MAP_WRITE, 0, &mapped);
			if (SUCCEEDED(hr))
			{
				auto error = memcpy_s((byte*)mapped.pData + startOffset, m_LengthInBytes, pData, lengthInBytes);

				if (error == 0) isSuccess = true;

				mDevice.GetImmediateContext()->Unmap(m_pBuffer, 0);
			}
		}
		break;
		case RU_Immutable:
		default:
			break;
		}
		return isSuccess;
	}

	CComPtr<ID3D11Buffer> D3D11DeviceBuffer::GetRawBuffer() const
	{
		return m_pBuffer.p;
	}

	D3D11DeviceBuffer* D3D11DeviceBuffer::Create(D3D11Device& device, BufferType type, ResourceUsage usage, byte const* initialData, uint32 lengthInBytes)
	{
		TRY_CATCH_LOG(
			return new D3D11DeviceBuffer(device, type, usage, initialData, lengthInBytes),
			return nullptr;
		);
	}

}