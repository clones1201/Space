#ifndef __SPACE_D3D11_DEVICEBUFFER_HPP__
#define __SPACE_D3D11_DEVICEBUFFER_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"
#include "D3D11Prerequisites.hpp"
#include "D3D11Device.hpp"

#include "DeviceBuffer.hpp"

namespace Space
{
	class D3D11DeviceBuffer : public DeviceBuffer
	{
	protected:
		D3D11DeviceBuffer(BufferType type, ResourceUsage usage, uint32 lengthInBytes);
	public:
		static D3D11DeviceBuffer* Create(D3D11Device& device, BufferType type, ResourceUsage usage, byte const* initialData, size_t lengthInBytes);
		
		virtual ID3D11Buffer* GetRawBuffer() const = 0;

		friend class DeviceBuffer;
	};
}

#endif