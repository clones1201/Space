#include "RenderSystem/RenderSystem.hpp"

namespace Space
{
	DeviceBuffer::DeviceBuffer(BufferType type, ResourceUsage usage, size_t lengthInBytes)
		:m_Type(type), m_Usage(usage), m_LengthInBytes(lengthInBytes)
	{}

	DeviceBuffer::~DeviceBuffer()
	{}
	
	DeviceBuffer* DeviceBuffer::Create(
		RenderSystem* pRenderSys, BufferType type, ResourceUsage usage,
		byte const* initialData, size_t lengthInBytes)
	{
		TRY_CATCH_LOG(
			return pRenderSys->CreateBuffer(
			type, usage, initialData, lengthInBytes),
			return nullptr);
	}

	VertexBuffer* VertexBuffer::Create
		(RenderSystem* pRenderSys, byte const* initialData, size_t lengthInBytes,
		size_t stride)
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

	VertexBuffer::VertexBuffer(DeviceBuffer* pBuffer, size_t stride)
		:m_pBuffer(pBuffer), m_Stride(stride)
	{}

	VertexBuffer::~VertexBuffer()
	{}

	bool VertexBuffer::Update(CommandList* pList, size_t startOffset, size_t lengthInBytes, byte const* pData)
	{
		return m_pBuffer->Update(pList, startOffset, lengthInBytes, pData);
	}
	
	IndexBuffer* IndexBuffer::Create(RenderSystem* pRenderSys, byte const* initialData, size_t lengthInBytes, DataFormat format)
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

	bool IndexBuffer::Update(CommandList* pList, size_t startOffset, size_t lengthInBytes, byte const* pData)
	{
		return m_pBuffer->Update(pList, startOffset, lengthInBytes, pData);
	}
	
	ConstantBuffer* ConstantBuffer::Create(RenderSystem* pRenderSys, byte const* initialData, size_t lengthInBytes)
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

	bool ConstantBuffer::Update(size_t startOffset, size_t lengthInBytes, byte const* pData)
	{
		auto error = memcpy_s(m_pShadowData + startOffset, m_pBuffer->GetLengthInBytes() - startOffset, pData, lengthInBytes);

		if (error == 0)
			return true;
		return false;
	}
		
	TextureBuffer* TextureBuffer::Create(RenderSystem* pRenderSys,
		byte const* initialData, size_t lengthInBytes,
		size_t sizeOfElem, size_t numElements)
	{
		TRY_CATCH_LOG(return new TextureBuffer(
			DeviceBuffer::Create(pRenderSys,
			BufferType::TextureBuffer, ResourceUsage::Default, initialData, lengthInBytes),
			sizeOfElem, numElements),
			return nullptr);
	}

	TextureBuffer::TextureBuffer(DeviceBuffer* pBuffer,
		size_t sizeOfElem, size_t numElements)
		:m_pBuffer(pBuffer), m_SizeOfElem(sizeOfElem), m_NumElements(numElements)
	{
	}
	TextureBuffer::~TextureBuffer()
	{}

	bool TextureBuffer::Update(CommandList* pList, size_t startOffset, size_t lengthInBytes, byte const* pData)
	{
		return m_pBuffer->Update(pList, startOffset, lengthInBytes, pData);
	}

	DeviceBuffer* TextureBuffer::GetBuffer()
	{
		return m_pBuffer.get();
	}

	size_t TextureBuffer::GetElementCount() const
	{
		return m_NumElements;
	}
	size_t TextureBuffer::GetSizeOfElement() const
	{
		return m_SizeOfElem;
	}

	
	DeviceTextureBase::DeviceTextureBase()
	{}

	DeviceTextureBase::~DeviceTextureBase()
	{}

	DeviceTexture1D::DeviceTexture1D(
		int32 X,
		DataFormat format,
		ResourceUsage usage,
		ResourceBindFlag flag,
		int32 arraySize) :
		m_Width(X), m_ArraySize(arraySize)
	{
		m_Type = arraySize > 1 ? TextureType::Texture1DArray : TextureType::Texture1D;
		m_Format = format;
		m_Usage = usage;
		m_Flag = flag;
	}
	DeviceTexture1D::~DeviceTexture1D()
	{}

	DeviceTexture1D* DeviceTexture1D::CreateArray(
		RenderSystem* pRenderSys,
		int32 X,
		DataFormat format,
		ResourceUsage usage,
		ResourceBindFlag flag,
		int32 arraySize,
		byte const* initialData)
	{
		return pRenderSys->CreateTexture1DArray(X, format, usage, flag, arraySize, initialData);
	}
	DeviceTexture1D* DeviceTexture1D::Create(
		RenderSystem* pRenderSys,
		int32 X,
		DataFormat format,
		ResourceUsage usage,
		ResourceBindFlag flag,
		byte const* initialData)
	{
		return pRenderSys->CreateTexture1D(X, format, usage, flag, initialData);
	}
	
	DeviceTexture2D::DeviceTexture2D(
		int32 X, int32 Y,
		DataFormat format,
		ResourceUsage usage,
		ResourceBindFlag flag,
		int32 arraySize) :
		m_Width(X), m_Height(Y), m_ArraySize(arraySize)
	{
		m_Type = arraySize > 1 ? TextureType::Texture2DArray : TextureType::Texture2D;
		m_Format = format;
		m_Usage = usage;
		m_Flag = flag;
	}
	DeviceTexture2D::~DeviceTexture2D()
	{}

	DeviceTexture2D* DeviceTexture2D::CreateArray(
		RenderSystem* pRenderSys,
		int32 X, int32 Y,
		DataFormat format,
		ResourceUsage usage,
		ResourceBindFlag flag,
		int32 arraySize,
		byte const* initialData)
	{
		return pRenderSys->CreateTexture2DArray(X, Y, format, usage, flag, arraySize, initialData);
	}
	DeviceTexture2D* DeviceTexture2D::Create(
		RenderSystem* pRenderSys,
		int32 X, int32 Y,
		DataFormat format,
		ResourceUsage usage,
		ResourceBindFlag flag,
		byte const* initialData)
	{
		return pRenderSys->CreateTexture2D(X, Y, format, usage, flag, initialData);
	}
	DeviceTexture2D* DeviceTexture2D::CreateFromFile(
		RenderSystem* pRenderSys, std::string const &filename, DataFormat format, ResourceUsage usage, ResourceBindFlag flag)
	{
		return pRenderSys->CreateTexture2DFromFile(filename, format, usage, flag);
	}

	DeviceTexture3D::DeviceTexture3D(
		int32 X, int32 Y, int32 Z,
		DataFormat format,
		ResourceUsage usage,
		ResourceBindFlag flag
		) :
		m_Width(X), m_Height(Y), m_Depth(Z)
	{
		m_Type = TextureType::Texture3D;
		m_Format = format;
		m_Usage = usage;
		m_Flag = flag;
	}
	DeviceTexture3D::~DeviceTexture3D()
	{}

	/*DeviceTexture3D* DeviceTexture3D::CreateArray(RenderSystem* pRenderSys, int32 X, int32 Y, int32 Z, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int32 arraySize, byte const* initialData);*/
	DeviceTexture3D* DeviceTexture3D::Create(
		RenderSystem* pRenderSys,
		int32 X, int32 Y, int32 Z,
		DataFormat format,
		ResourceUsage usage,
		ResourceBindFlag flag,
		byte const* initialData)
	{
		return pRenderSys->CreateTexture3D(X, Y, Z, format, usage, flag, initialData);
	}
}