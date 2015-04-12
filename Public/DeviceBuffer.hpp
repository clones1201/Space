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

	class VertexBuffer
	{
	protected:
		std::unique_ptr<DeviceBuffer> m_pBuffer = nullptr;

		VertexBuffer(DeviceBuffer* pBuffer);
	public:
		virtual ~VertexBuffer();

		static VertexBuffer* Create(RenderSystem* pRenderSys, byte const* initialData, uint32 lengthInBytes);

		bool Update(uint32 startOffset, uint32 lengthInBytes, byte const* pData);
	};

	class IndexBuffer
	{
	protected:
		std::unique_ptr<DeviceBuffer> m_pBuffer = nullptr;

		IndexBuffer(DeviceBuffer* pBuffer);
	public:
		virtual ~IndexBuffer();

		static IndexBuffer* Create(RenderSystem* pRenderSys, byte const* initialData, uint32 lengthInBytes);

		bool Update(uint32 startOffset, uint32 lengthInBytes, byte const* pData);
	};

	class ConstantBuffer
	{ 
	protected:
		std::unique_ptr<DeviceBuffer> m_pBuffer = nullptr;
		byte *m_pShadowData = nullptr;
		uint32 m_Size;
		
		ConstantBuffer(DeviceBuffer* pBuffer);
	public:
		virtual ~ConstantBuffer();

		static ConstantBuffer* Create(RenderSystem* pRenderSys, byte const* initialData, uint32 lengthInBytes);

		void UpdateToDevice();
		bool Update(uint32 startOffset, uint32 lengthInBytes, byte const* pData);
	};

	class TextureBuffer
	{
	protected:
		std::unique_ptr<DeviceBuffer> m_pBuffer = nullptr;

		TextureBuffer(DeviceBuffer* pBuffer);
	public:
		virtual ~TextureBuffer();

		static TextureBuffer* Create(RenderSystem* pRenderSys, byte const* initialData, uint32 lengthInBytes);

		bool Update(uint32 startOffset, uint32 lengthInBytes, byte const* pData);

		ShaderResource* GetShaderResource();

	};
}

#endif