#ifndef __SPACE_DEVICEBUFFER_HPP
#define __SPACE_DEVICEBUFFER_HPP

#include "Prerequisites.hpp"
#include "Basic.hpp"
#include "RenderSystem.hpp"

namespace Space
{
	class DeviceBuffer : virtual public Interface 
	{
	protected:
		DeviceBuffer(BufferType type, ResourceUsage usage, uint32 lengthInBytes);

		BufferType m_Type;
		ResourceUsage m_Usage;
		uint32 m_LengthInBytes;
	public:
		virtual ~DeviceBuffer();

		static DeviceBuffer* Create(RenderSystem* pRenderSys,BufferType type,ResourceUsage usage, byte const* initialData, uint32 lengthInBytes);

		uint32 GetLengthInBytes() const;
		ResourceUsage GetUsage() const;
		BufferType GetBufferType() const;
		virtual bool Update(uint32 startOffset, uint32 lengthInBytes, byte const* pData) = 0;
	}; 

	class VertexBuffer : public Uncopyable
	{
	protected:
		std::unique_ptr<DeviceBuffer> m_pBuffer = nullptr;

		VertexBuffer(DeviceBuffer* pBuffer);
	public:
		virtual ~VertexBuffer();

		static VertexBuffer* Create(RenderSystem* pRenderSys, byte const* initialData, uint32 lengthInBytes);

		bool Update(uint32 startOffset, uint32 lengthInBytes, byte const* pData);

		DeviceBuffer* GetBuffer();
	};

	class IndexBuffer : public Uncopyable
	{
	public:
		virtual ~IndexBuffer();

		static IndexBuffer* Create(RenderSystem* pRenderSys, byte const* initialData, uint32 lengthInBytes);

		bool Update(uint32 startOffset, uint32 lengthInBytes, byte const* pData);

		DeviceBuffer* GetBuffer();
	protected:
		IndexBuffer(DeviceBuffer* pBuffer);

		std::unique_ptr<DeviceBuffer> m_pBuffer = nullptr;
	};

	class ConstantBuffer : public Uncopyable
	{ 
	public:
		virtual ~ConstantBuffer();

		static ConstantBuffer* Create(RenderSystem* pRenderSys, byte const* initialData, uint32 lengthInBytes);

		void UpdateToDevice();
		bool Update(uint32 startOffset, uint32 lengthInBytes, byte const* pData);		
		byte const* GetBufferPointer() const;

		DeviceBuffer* GetBuffer();
	protected:
		ConstantBuffer(DeviceBuffer* pBuffer);
		
		std::unique_ptr<DeviceBuffer> m_pBuffer = nullptr;
		byte *m_pShadowData = nullptr;

		friend class ShaderVariableAccessor;
		friend class ConstantBufferAccessor;
	};

	class TextureBuffer : public Uncopyable
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
}

#endif