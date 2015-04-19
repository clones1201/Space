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
		TRY_CATCH_LOG(return pRenderSys->CreateBuffer(type, usage, initialData, lengthInBytes),
			return nullptr);
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

	DeviceBuffer* VertexBuffer::GetBuffer()
	{
		return m_pBuffer.get();
	}

	IndexBuffer* IndexBuffer::Create(RenderSystem* pRenderSys, byte const* initialData, uint32 lengthInBytes)
	{
		TRY_CATCH_LOG(
			return new IndexBuffer(
				DeviceBuffer::Create(pRenderSys, BT_IndexBuffer, RU_Default, initialData, lengthInBytes)),
			return nullptr);
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

	DeviceBuffer* IndexBuffer::GetBuffer()
	{
		return m_pBuffer.get();
	}

	ConstantBuffer* ConstantBuffer::Create(RenderSystem* pRenderSys, byte const* initialData, uint32 lengthInBytes)
	{
		TRY_CATCH_LOG(
			return new ConstantBuffer(
			DeviceBuffer::Create(pRenderSys, BT_ConstantBuffer, RU_Default, initialData, lengthInBytes)),
			return nullptr);
	}

	ConstantBuffer::ConstantBuffer(DeviceBuffer* pBuffer)
		:m_pBuffer(pBuffer)
	{
		m_pShadowData = 
			(byte*)_aligned_malloc(pBuffer->GetLengthInBytes(),16);
	}
	ConstantBuffer::~ConstantBuffer()
	{
		if (m_pShadowData != nullptr)
			delete [] m_pShadowData;
	}

	void ConstantBuffer::UpdateToDevice()
	{
		m_pBuffer->Update(0, m_pBuffer->GetLengthInBytes(), m_pShadowData);
	}

	bool ConstantBuffer::Update(uint32 startOffset, uint32 lengthInBytes, byte const* pData)
	{
		auto error = memcpy_s(m_pShadowData + startOffset, m_pBuffer->GetLengthInBytes() - startOffset, pData, lengthInBytes);

		if (error == 0)
			return true;
		return false;
	}
	
	byte const* ConstantBuffer::GetBufferPointer() const
	{
		return m_pShadowData;
	}

	DeviceBuffer* ConstantBuffer::GetBuffer()
	{
		return m_pBuffer.get();
	}
	
	TextureBuffer* TextureBuffer::Create(RenderSystem* pRenderSys,
		byte const* initialData, uint32 lengthInBytes,
		uint32 sizeOfElem, uint32 numElements)
	{
		TRY_CATCH_LOG(return new TextureBuffer(
			DeviceBuffer::Create(pRenderSys,
			BT_TextureBuffer, RU_Default, initialData, lengthInBytes),
			sizeOfElem, numElements),
			return nullptr);
	}

	TextureBuffer::TextureBuffer(DeviceBuffer* pBuffer,
		uint32 sizeOfElem, uint32 numElements)
		:m_pBuffer(pBuffer), m_SizeOfElem(sizeOfElem), m_NumElements(numElements)
	{
	}
	TextureBuffer::~TextureBuffer()
	{}

	bool TextureBuffer::Update(uint32 startOffset, uint32 lengthInBytes, byte const* pData)
	{
		return m_pBuffer->Update(startOffset, lengthInBytes, pData);
	}
		
	DeviceBuffer* TextureBuffer::GetBuffer()
	{
		return m_pBuffer.get();
	}

	uint32 TextureBuffer::GetElementCount() const
	{
		return m_NumElements;
	}
	uint32 TextureBuffer::GetSizeOfElement() const
	{
		return m_SizeOfElem;
	}
}