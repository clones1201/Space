#include "Log.h"

#include "DeviceBuffer.hpp"
#include "D3D11RenderSystem.hpp"
#include "D3D11DeviceBuffer.hpp"

namespace Space
{ 
	DeviceBuffer::DeviceBuffer(BufferType type, ResourceUsage usage, uint32 lengthInBytes)
		:m_Type(type), m_Usage(usage), m_LengthInBytes(lengthInBytes)
	{}

	DeviceBuffer::~DeviceBuffer()
	{}
	
	uint32 DeviceBuffer::GetLengthInBytes() const
	{
		return m_LengthInBytes;
	}
	ResourceUsage DeviceBuffer::GetUsage() const
	{
		return m_Usage;
	}
	BufferType DeviceBuffer::GetBufferType() const
	{
		return m_Type;
	}

	DeviceBuffer* DeviceBuffer::Create(RenderSystem* pRenderSys, BufferType type, ResourceUsage usage, byte const* initialData, uint32 lengthInBytes)
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

	VertexBuffer* VertexBuffer::Create(RenderSystem* pRenderSys, byte const* initialData, uint32 lengthInBytes)
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

	bool VertexBuffer::Update(uint32 startOffset, uint32 lengthInBytes, byte const* pData)
	{
		return m_pBuffer->Update(startOffset, lengthInBytes, pData);
	}

	IndexBuffer* IndexBuffer::Create(RenderSystem* pRenderSys, byte const* initialData, uint32 lengthInBytes)
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

	bool IndexBuffer::Update(uint32 startOffset, uint32 lengthInBytes, byte const* pData)
	{
		return m_pBuffer->Update(startOffset, lengthInBytes, pData);
	}
	
	ConstantBuffer* ConstantBuffer::Create(RenderSystem* pRenderSys, byte const* initialData, uint32 lengthInBytes)
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
		:m_pBuffer(pBuffer), m_Size(pBuffer->GetLengthInBytes())
	{
		m_pShadowData = new byte[m_Size];
	}
	ConstantBuffer::~ConstantBuffer()
	{
		if (m_pShadowData != nullptr)
			delete [] m_pShadowData;
	}

	void ConstantBuffer::UpdateToDevice()
	{
		m_pBuffer->Update(0, m_Size, m_pShadowData);
	}

	bool ConstantBuffer::Update(uint32 startOffset, uint32 lengthInBytes, byte const* pData)
	{
		auto error = memcpy_s(m_pShadowData + startOffset, m_Size - startOffset, pData, lengthInBytes);

		if (error == 0)
			return true;
		return false;
	}

	TextureBuffer* TextureBuffer::Create(RenderSystem* pRenderSys, byte const* initialData, uint32 lengthInBytes)
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

	bool TextureBuffer::Update(uint32 startOffset, uint32 lengthInBytes, byte const* pData)
	{
		return m_pBuffer->Update(startOffset, lengthInBytes, pData);
	}
	

}