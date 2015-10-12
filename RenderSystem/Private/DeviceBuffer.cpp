#include "RenderSystem/RenderSystem.hpp"

namespace Space
{
	DeviceBuffer::DeviceBuffer(BufferType type, ResourceUsage usage, uint32 lengthInBytes)
		:m_Type(type), m_Usage(usage), m_LengthInBytes(lengthInBytes)
	{}

	DeviceBuffer::~DeviceBuffer()
	{}
	
	DeviceBuffer* DeviceBuffer::Create(
		RenderSystem* pRenderSys, BufferType type, ResourceUsage usage,
		byte const* initialData, uint32 lengthInBytes)
	{
		TRY_CATCH_LOG(
			return pRenderSys->CreateBuffer(
			type, usage, initialData, lengthInBytes),
			return nullptr);
	}

	VertexBuffer* VertexBuffer::Create
		(RenderSystem* pRenderSys, byte const* initialData, uint32 lengthInBytes,
		uint32 stride)
	{
		try
		{
			return new VertexBuffer(
				DeviceBuffer::Create(pRenderSys, BufferType::VertexBuffer, ResourceUsage::Default, initialData, lengthInBytes),
				stride);
		}
		catch (std::exception &e)
		{
			Log(e.what());
			return nullptr;
		}
	}

	VertexBuffer::VertexBuffer(DeviceBuffer* pBuffer, uint32 stride)
		:m_pBuffer(pBuffer), m_Stride(stride)
	{}

	VertexBuffer::~VertexBuffer()
	{}

	bool VertexBuffer::Update(CommandList* pList, uint32 startOffset, uint32 lengthInBytes, byte const* pData)
	{
		return m_pBuffer->Update(pList, startOffset, lengthInBytes, pData);
	}
	
	IndexBuffer* IndexBuffer::Create(RenderSystem* pRenderSys, byte const* initialData, uint32 lengthInBytes, DataFormat format)
	{
		TRY_CATCH_LOG(
			return new IndexBuffer(
			DeviceBuffer::Create(pRenderSys, BufferType::IndexBuffer, ResourceUsage::Default, initialData, lengthInBytes),
			format),
			return nullptr);
	}

	IndexBuffer::IndexBuffer(DeviceBuffer* pBuffer,DataFormat format)
		:m_pBuffer(pBuffer), m_Format(format)
	{
	}
	IndexBuffer::~IndexBuffer()
	{}

	bool IndexBuffer::Update(CommandList* pList, uint32 startOffset, uint32 lengthInBytes, byte const* pData)
	{
		return m_pBuffer->Update(pList, startOffset, lengthInBytes, pData);
	}
	
	ConstantBuffer* ConstantBuffer::Create(RenderSystem* pRenderSys, byte const* initialData, uint32 lengthInBytes)
	{
		TRY_CATCH_LOG(
			return new ConstantBuffer(
			DeviceBuffer::Create(pRenderSys, BufferType::ConstantBuffer, ResourceUsage::Default, initialData, lengthInBytes)),
			return nullptr);
	}

	ConstantBuffer::ConstantBuffer(DeviceBuffer* pBuffer)
		:m_pBuffer(pBuffer)
	{
		m_pShadowData =
			(byte*)_aligned_malloc(pBuffer->GetLengthInBytes(), 16);
	}
	ConstantBuffer::~ConstantBuffer()
	{
		if (m_pShadowData != nullptr)
			_aligned_free(m_pShadowData);
	}

	void ConstantBuffer::UpdateToDevice(CommandList* pList)
	{
		m_pBuffer->Update(pList, 0, m_pBuffer->GetLengthInBytes(), m_pShadowData);
	}

	bool ConstantBuffer::Update(uint32 startOffset, uint32 lengthInBytes, byte const* pData)
	{
		auto error = memcpy_s(m_pShadowData + startOffset, m_pBuffer->GetLengthInBytes() - startOffset, pData, lengthInBytes);

		if (error == 0)
			return true;
		return false;
	}
		
	TextureBuffer* TextureBuffer::Create(RenderSystem* pRenderSys,
		byte const* initialData, uint32 lengthInBytes,
		uint32 sizeOfElem, uint32 numElements)
	{
		TRY_CATCH_LOG(return new TextureBuffer(
			DeviceBuffer::Create(pRenderSys,
			BufferType::TextureBuffer, ResourceUsage::Default, initialData, lengthInBytes),
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

	bool TextureBuffer::Update(CommandList* pList, uint32 startOffset, uint32 lengthInBytes, byte const* pData)
	{
		return m_pBuffer->Update(pList, startOffset, lengthInBytes, pData);
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