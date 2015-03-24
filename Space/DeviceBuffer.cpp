#include "Log.h"

#include "DeviceBuffer.hpp"
#include "D3D11RenderSystem.hpp"
#include "D3D11DeviceBuffer.hpp"

namespace Space
{ 
	DeviceBuffer::DeviceBuffer()
	{}

	DeviceBuffer::~DeviceBuffer()
	{}

	DeviceBuffer* DeviceBuffer::Create(RenderSystem* pRenderSys, BufferType type, ResourceUsage usage, byte const* initialData, size_t lengthInBytes)
	{
		try
		{
			return pRenderSys->CreateBuffer(type,usage, initialData, lengthInBytes);
		}
		catch (std::exception& e)
		{
			Log(e.what());
			return nullptr;
		}
	}

	VertexBuffer* VertexBuffer::Create(RenderSystem* pRenderSys, byte const* initialData, size_t lengthInBytes)
	{
		try
		{
			return new VertexBuffer(DeviceBuffer::Create(pRenderSys, BT_VertexBuffer, RU_Default, initialData, lengthInBytes));
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}

	VertexBuffer::VertexBuffer(DeviceBuffer* pBuffer)
		:m_pBuffer(pBuffer)
	{}  

	VertexBuffer::~VertexBuffer()
	{}

	bool VertexBuffer::Update(uint startOffset, uint lengthInBytes, byte const* pData)
	{
		return m_pBuffer->Update(startOffset, lengthInBytes, pData);
	}

	IndexBuffer* IndexBuffer::Create(RenderSystem* pRenderSys, byte const* initialData, size_t lengthInBytes)
	{
		try
		{
			return new IndexBuffer(DeviceBuffer::Create(pRenderSys, BT_IndexBuffer, RU_Default, initialData, lengthInBytes));
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}

	IndexBuffer::IndexBuffer(DeviceBuffer* pBuffer)
		:m_pBuffer(pBuffer)
	{
	}
	IndexBuffer::~IndexBuffer()
	{}

	bool IndexBuffer::Update(uint startOffset, uint lengthInBytes, byte const* pData)
	{
		return m_pBuffer->Update(startOffset, lengthInBytes, pData);
	}
	
	ConstantBuffer* ConstantBuffer::Create(RenderSystem* pRenderSys, byte const* initialData, size_t lengthInBytes)
	{
		try
		{
			return new ConstantBuffer(DeviceBuffer::Create(pRenderSys, BT_ConstantBuffer, RU_Default, initialData, lengthInBytes));
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}

	ConstantBuffer::ConstantBuffer(DeviceBuffer* pBuffer)
		:m_pBuffer(pBuffer)
	{
	}
	ConstantBuffer::~ConstantBuffer()
	{}

	bool ConstantBuffer::Update(uint lengthInBytes, byte const* pData)
	{
		return m_pBuffer->Update(0, lengthInBytes, pData);
	}

	TextureBuffer* TextureBuffer::Create(RenderSystem* pRenderSys, byte const* initialData, size_t lengthInBytes)
	{
		try
		{
			return new TextureBuffer(DeviceBuffer::Create(pRenderSys, BT_TextureBuffer, RU_Default, initialData, lengthInBytes));
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}

	TextureBuffer::TextureBuffer(DeviceBuffer* pBuffer)
		:m_pBuffer(pBuffer)
	{
	}
	TextureBuffer::~TextureBuffer()
	{}

	bool TextureBuffer::Update(uint startOffset, uint lengthInBytes, byte const* pData)
	{
		return m_pBuffer->Update(startOffset, lengthInBytes, pData);
	}
	

}