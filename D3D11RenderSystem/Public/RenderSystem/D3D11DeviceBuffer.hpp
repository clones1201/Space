#ifndef __SPACE_D3D11_DEVICEBUFFER_HPP__
#define __SPACE_D3D11_DEVICEBUFFER_HPP__

#include "RenderSystem/D3D11Prerequisites.hpp"
#include "RenderSystem/D3D11Device.hpp"

#include "RenderSystem/DeviceBuffer.hpp"

namespace Space
{
	class SPACE_D3D11_API D3D11DeviceBuffer : public DeviceBuffer
	{
	public:
		static D3D11DeviceBuffer* Create(D3D11DevicePtr device, BufferType type, ResourceUsage usage, byte const* initialData, uint32 lengthInBytes);
				
		virtual bool Update(uint32 startOffset, uint32 lengthInBytes, byte const* pData);

		CComPtr<ID3D11Buffer> GetRawBuffer() const;
	protected:
		D3D11DeviceBuffer(D3D11DevicePtr device, BufferType type,
			ResourceUsage usage, byte const* initialData, uint32 lengthInBytes);
		
	private:
		CComPtr<ID3D11Buffer> m_pBuffer = nullptr;

		D3D11DevicePtr mDevice;
		friend class DeviceBuffer;
	};
	typedef std::shared_ptr<D3D11DeviceBuffer> D3D11DeviceBufferPtr;
}

#endif