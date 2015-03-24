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
		DeviceBuffer();
	public:
		virtual ~DeviceBuffer();

		static DeviceBuffer* Create(RenderSystem* pRenderSys,BufferType type,ResourceUsage usage, byte const* initialData, uint lengthInBytes);

		virtual bool Update(uint startOffset, uint lengthInBytes, byte const* pData) = 0;
	}; 

	class VertexBuffer
	{
	protected:
		std::unique_ptr<DeviceBuffer> m_pBuffer = nullptr;

		VertexBuffer(DeviceBuffer* pBuffer);
	public:
		virtual ~VertexBuffer();

		typedef struct _VertexBufferDesc
		{
			uint32 Length	  :28;
			int8  numUVs	   :3;
			bool   withTangent :1;
		}Description;

		static VertexBuffer* Create(RenderSystem* pRenderSys, byte const* initialData, uint lengthInBytes);

		bool Update(uint startOffset, uint lengthInBytes, byte const* pData);
	};

	class IndexBuffer
	{
	protected:
		std::unique_ptr<DeviceBuffer> m_pBuffer = nullptr;

		IndexBuffer(DeviceBuffer* pBuffer);
	public:
		virtual ~IndexBuffer();

		static IndexBuffer* Create(RenderSystem* pRenderSys, byte const* initialData, uint lengthInBytes);

		bool Update(uint startOffset, uint lengthInBytes, byte const* pData);
	};

	class ConstantBuffer
	{ 
	protected:
		std::unique_ptr<DeviceBuffer> m_pBuffer = nullptr;

		ConstantBuffer(DeviceBuffer* pBuffer);
	public:
		virtual ~ConstantBuffer();

		static ConstantBuffer* Create(RenderSystem* pRenderSys, byte const* initialData, uint lengthInBytes);

		bool Update(uint lengthInBytes, byte const* pData);
	};

	class TextureBuffer
	{
	protected:
		std::unique_ptr<DeviceBuffer> m_pBuffer = nullptr;

		TextureBuffer(DeviceBuffer* pBuffer);
	public:
		virtual ~TextureBuffer();

		static TextureBuffer* Create(RenderSystem* pRenderSys, byte const* initialData, uint lengthInBytes);

		bool Update(uint startOffset, uint lengthInBytes, byte const* pData);

		ShaderResource* GetShaderResource();

	};
}

#endif