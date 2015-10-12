#ifndef __SPACE_RENDERSYSTEM_DEVICEBUFFER_HPP
#define __SPACE_RENDERSYSTEM_DEVICEBUFFER_HPP

#include "RenderSystem/Prerequisites.hpp"
#include "RenderSystem/Shared.hpp"

namespace Space
{
	class SPACE_RENDERSYSTEM_API DeviceBuffer : private Interface
	{
	public:
		virtual ~DeviceBuffer();

		static DeviceBuffer* Create(RenderSystem* pRenderSys,BufferType type,ResourceUsage usage, byte const* initialData, uint32 lengthInBytes);

		inline uint32 GetLengthInBytes() const
		{
			return m_LengthInBytes;
		}
		inline ResourceUsage GetUsage() const
		{
			return m_Usage;
		}
		inline BufferType GetBufferType() const
		{
			return m_Type;
		}
		virtual bool Update(uint32 startOffset, uint32 lengthInBytes, byte const* pData) = 0;
	
	protected:
		DeviceBuffer(BufferType type, ResourceUsage usage, uint32 lengthInBytes);

		BufferType m_Type;
		ResourceUsage m_Usage;
		uint32 m_LengthInBytes;
	}; 
	typedef std::shared_ptr<DeviceBuffer> DeviceBufferPtr;

	class SPACE_RENDERSYSTEM_API VertexBuffer : private Uncopyable
	{
	public:
		virtual ~VertexBuffer();

		static VertexBuffer* Create(
			RenderSystem* pRenderSys, byte const* initialData, uint32 lengthInBytes,
			uint stride);

		bool Update(uint32 startOffset, uint32 lengthInBytes, byte const* pData);

		inline uint GetStride() const
		{
			return m_Stride;
		}
		inline uint GetOffest() const
		{
			return m_Offset;
		}
		inline void SetStride(uint stride)
		{
			assert(stride >= 0);
			m_Stride = stride;
		}
		inline void SetOffest(uint offset)
		{
			assert(offset >= 0);
			m_Offset = offset;
		}
		inline DeviceBuffer* GetBuffer() const
		{
			return m_pBuffer.get();
		}
		inline uint32 GetLengthInBytes() const
		{
			return m_pBuffer->GetLengthInBytes();
		}
	protected:
		VertexBuffer(DeviceBuffer* pBuffer, uint32 stride);

		std::unique_ptr<DeviceBuffer> m_pBuffer = nullptr;
		uint m_Offset = 0;
		uint m_Stride = 0;
	};
	typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;

	class SPACE_RENDERSYSTEM_API IndexBuffer : private Uncopyable
	{
	public:
		virtual ~IndexBuffer();

		static IndexBuffer* Create(
			RenderSystem* pRenderSys, byte const* initialData, uint32 lengthInBytes,
			DataFormat format);

		bool Update(uint32 startOffset, uint32 lengthInBytes, byte const* pData);


		inline DataFormat GetFormat() const
		{
			return m_Format;
		}
		inline uint GetOffest() const
		{
			return m_Offset;
		}
		inline void SetDataFormat(DataFormat format)
		{
			m_Format = format;
		}
		inline void SetOffest(uint offset)
		{
			m_Offset = offset;
		}

		inline DeviceBuffer* GetBuffer() const
		{
			return m_pBuffer.get();
		}

		inline uint32 GetLengthInBytes() const
		{
			return m_pBuffer->GetLengthInBytes();
		}
	protected:
		IndexBuffer(DeviceBuffer* pBuffer, DataFormat format);

		std::unique_ptr<DeviceBuffer> m_pBuffer = nullptr;
		uint m_Offset = 0;
		DataFormat m_Format = DataFormat::UNKNOWN;
	};
	typedef std::shared_ptr<IndexBuffer> IndexBufferPtr;

	class SPACE_RENDERSYSTEM_API ConstantBuffer : private Uncopyable
	{ 
	public:
		virtual ~ConstantBuffer();

		static ConstantBuffer* Create(RenderSystem* pRenderSys, byte const* initialData, uint32 lengthInBytes);

		void UpdateToDevice();
		bool Update(uint32 startOffset, uint32 lengthInBytes, byte const* pData);		
		inline byte const* GetBufferPointer() const
		{
			return m_pShadowData;
		}
		inline DeviceBuffer* GetBuffer()
		{
			return m_pBuffer.get();
		}

		inline uint32 GetLengthInBytes() const
		{
			return m_pBuffer->GetLengthInBytes();
		}
	protected:
		ConstantBuffer(DeviceBuffer* pBuffer);
		
		std::unique_ptr<DeviceBuffer> m_pBuffer = nullptr;
		byte *m_pShadowData = nullptr;

		friend class ShaderVariableAccessor;
		friend class ConstantBufferAccessor;
	};
	typedef std::shared_ptr<ConstantBuffer> ConstantBufferPtr;

	class SPACE_RENDERSYSTEM_API TextureBuffer : private Uncopyable
	{
	public:
		virtual ~TextureBuffer();

		static TextureBuffer* Create(
			RenderSystem* pRenderSys, byte const* initialData,
			uint32 lengthInBytes, uint32 sizeOfElem, uint32 numElements);

		uint32 GetSizeOfElement() const;
		uint32 GetElementCount() const;
		bool Update(uint32 index, uint32 lengthInBytes, byte const* pData);
		
		DeviceBuffer* GetBuffer();	
	protected:
		TextureBuffer(DeviceBuffer* pBuffer,
			uint32 sizeOfElem, uint32 numElements);

		std::unique_ptr<DeviceBuffer> m_pBuffer = nullptr;
		uint32 m_SizeOfElem;
		uint32 m_NumElements;
	};
	typedef std::shared_ptr<TextureBuffer> TextureBufferPtr;
}

#endif