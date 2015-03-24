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
	public:
		static D3D11DeviceBuffer* Create(D3D11Device& device, BufferType type, ResourceUsage usage, byte const* initialData, size_t lengthInBytes);
		
		ID3D11Buffer* GetRawBuffer() const;

		friend class DeviceBuffer;
	};
/*
	class D3D11VertexBuffer : public VertexBuffer
	{

	public:
		ID3D11Buffer* GetRawBuffer() const;
	};
	
	class D3D11IndexBuffer : public IndexBuffer
	{

		ID3D11Buffer* GetRawBuffer() const;
	};

	class D3D11ConstantBuffer : public ConstantBuffer
	{

		ID3D11Buffer* GetRawBuffer() const;
	};

	class D3D11TextureBuffer : public TextureBuffer
	{

		ID3D11Buffer* GetRawBuffer() const;
	};*/
}

#endif