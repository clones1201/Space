#include "Log.h"
#include "D3D11Shared.hpp"
#include "D3D11DeviceBuffer.hpp"

namespace Space
{	
	class D3D11DeviceBufferImpl : public D3D11DeviceBuffer
	{
	private:
		CComPtr<ID3D11Buffer> m_pBuffer = nullptr;

		D3D11Device& mDevice;
	public:
		D3D11DeviceBufferImpl(D3D11Device& device, BufferType type, ResourceUsage usage, byte const* initialData, size_t lengthInBytes)
			:mDevice(device), D3D11DeviceBuffer(type,usage,lengthInBytes)
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

		bool Update(uint32 startOffset, uint32 lengthInBytes, byte const* pData)
		{
			bool isSuccess = false;;
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
				mDevice.GetImmediateContext()->Map(m_pBuffer, 0, D3D11_MAP_WRITE, 0, &mapped );

				auto error = memcpy_s((byte*)mapped.pData + startOffset, m_LengthInBytes, pData, lengthInBytes);

				if (error == 0) isSuccess = true;					 

				mDevice.GetImmediateContext()->Unmap(m_pBuffer, 0);
			}
			break;
			case RU_Immutable:
			default:
				break;
			}
			return isSuccess;
		}
		
		ID3D11Buffer* GetRawBuffer() const
		{
			return m_pBuffer.p;
		}		 
	};

	D3D11DeviceBuffer* D3D11DeviceBuffer::Create(D3D11Device& device, BufferType type, ResourceUsage usage, byte const* initialData, size_t lengthInBytes)
	{
		try
		{
			return new D3D11DeviceBufferImpl(device,type,usage,initialData, lengthInBytes);
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}

	D3D11DeviceBuffer::D3D11DeviceBuffer(BufferType type, ResourceUsage usage, uint32 lengthInBytes)
		:DeviceBuffer(type,usage,lengthInBytes)
	{}

}