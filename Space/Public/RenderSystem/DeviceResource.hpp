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

		static DeviceBuffer* Create(RenderSystem* pRenderSys,BufferType type,ResourceUsage usage, byte const* initialData, size_t lengthInBytes);

		inline size_t GetLengthInBytes() const
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
		virtual bool Update(CommandList* pList, size_t startOffset, size_t lengthInBytes, byte const* pData) = 0;
	
	protected:
		DeviceBuffer(BufferType type, ResourceUsage usage, size_t lengthInBytes);

		BufferType m_Type;
		ResourceUsage m_Usage;
		size_t m_LengthInBytes;
	}; 
	typedef std::shared_ptr<DeviceBuffer> DeviceBufferPtr;

	class SPACE_RENDERSYSTEM_API VertexBuffer : private Uncopyable
	{
	public:
		virtual ~VertexBuffer();

		static VertexBuffer* Create(
			RenderSystem* pRenderSys, byte const* initialData, size_t lengthInBytes,
			size_t stride);

		bool Update(CommandList* pList, size_t startOffset, size_t lengthInBytes, byte const* pData);

		inline size_t GetStride() const
		{
			return m_Stride;
		}
		inline size_t GetOffest() const
		{
			return m_Offset;
		}
		inline void SetStride(size_t stride)
		{
			assert(stride >= 0);
			m_Stride = stride;
		}
		inline void SetOffest(size_t offset)
		{
			assert(offset >= 0);
			m_Offset = offset;
		}
		inline DeviceBuffer* GetBuffer() const
		{
			return m_pBuffer.get();
		}
		inline size_t GetLengthInBytes() const
		{
			return m_pBuffer->GetLengthInBytes();
		}
	protected:
		VertexBuffer(DeviceBuffer* pBuffer, size_t stride);

		std::unique_ptr<DeviceBuffer> m_pBuffer = nullptr;
		size_t m_Offset = 0;
		size_t m_Stride = 0;
	};
	typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;

	class SPACE_RENDERSYSTEM_API IndexBuffer : private Uncopyable
	{
	public:
		virtual ~IndexBuffer();

		static IndexBuffer* Create(
			RenderSystem* pRenderSys, byte const* initialData, size_t lengthInBytes,
			DataFormat format);

		bool Update(CommandList* pList, size_t startOffset, size_t lengthInBytes, byte const* pData);


		inline DataFormat GetFormat() const
		{
			return m_Format;
		}
		inline size_t GetOffest() const
		{
			return m_Offset;
		}
		inline void SetDataFormat(DataFormat format)
		{
			m_Format = format;
		}
		inline void SetOffest(size_t offset)
		{
			m_Offset = offset;
		}

		inline DeviceBuffer* GetBuffer() const
		{
			return m_pBuffer.get();
		}

		inline size_t GetLengthInBytes() const
		{
			return m_pBuffer->GetLengthInBytes();
		}
	protected:
		IndexBuffer(DeviceBuffer* pBuffer, DataFormat format);

		std::unique_ptr<DeviceBuffer> m_pBuffer = nullptr;
		size_t m_Offset = 0;
		DataFormat m_Format = DataFormat::UNKNOWN;
	};
	typedef std::shared_ptr<IndexBuffer> IndexBufferPtr;

	class SPACE_RENDERSYSTEM_API ConstantBuffer : private Uncopyable
	{ 
	public:
		virtual ~ConstantBuffer();

		static ConstantBuffer* Create(RenderSystem* pRenderSys, byte const* initialData, size_t lengthInBytes);

		void UpdateToDevice(CommandList* pList);
		bool Update(size_t startOffset, size_t lengthInBytes, byte const* pData);		
		inline byte const* GetBufferPointer() const
		{
			return m_pShadowData;
		}
		inline DeviceBuffer* GetBuffer()
		{
			return m_pBuffer.get();
		}

		inline size_t GetLengthInBytes() const
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
			size_t lengthInBytes, size_t sizeOfElem, size_t numElements);

		size_t GetSizeOfElement() const;
		size_t GetElementCount() const;
		bool Update(CommandList* pList, size_t startOffset, size_t lengthInBytes, byte const* pData);
		
		DeviceBuffer* GetBuffer();	
	protected:
		TextureBuffer(DeviceBuffer* pBuffer,
			size_t sizeOfElem, size_t numElements);

		std::unique_ptr<DeviceBuffer> m_pBuffer = nullptr;
		size_t m_SizeOfElem;
		size_t m_NumElements;
	};
	typedef std::shared_ptr<TextureBuffer> TextureBufferPtr;

	
	class SPACE_RENDERSYSTEM_API DeviceTextureBase : private Uncopyable
	{
	public:
		virtual ~DeviceTextureBase();

		inline TextureType GetType() const
		{
			return m_Type;
		}
		inline DataFormat GetFormat() const
		{
			return m_Format;
		}
		inline ResourceUsage GetUsage() const
		{
			return m_Usage;
		}
		inline ResourceBindFlag GetBindFlag() const
		{
			return m_Flag;
		}
	protected:
		DeviceTextureBase();

		TextureType m_Type = TextureType::None;
		DataFormat m_Format = DataFormat::UNKNOWN;
		ResourceUsage m_Usage = ResourceUsage::Default;
		ResourceBindFlag m_Flag = ResourceBindFlag::None;
	};

	class SPACE_RENDERSYSTEM_API DeviceTexture1D : private Interface, public DeviceTextureBase
	{
	public:
		virtual ~DeviceTexture1D();

		static DeviceTexture1D* CreateArray(
			RenderSystem* pRenderSys,
			int32 X,
			DataFormat format,
			ResourceUsage usage,
			ResourceBindFlag flag,
			int32 arraySize,
			byte const* initialData);
		static DeviceTexture1D* Create(
			RenderSystem* pRenderSys,
			int32 X,
			DataFormat format,
			ResourceUsage usage,
			ResourceBindFlag flag,
			byte const* initialData);
		
		virtual byte* Lock() = 0;
		virtual void Unlock() = 0;

		inline int32 GetArraySize() const
		{
			return m_ArraySize;
		}
		inline int32 GetWidth() const
		{
			return m_Width;
		}
	protected:
		DeviceTexture1D(int32 X, 
			DataFormat format, 
			ResourceUsage usage, 
			ResourceBindFlag flag,
			int32 arraySize);
		int32 m_Width = 0;
		int32 m_ArraySize = 0;
	};
	typedef std::shared_ptr<DeviceTexture1D> DeviceTexture1DPtr;

	class SPACE_RENDERSYSTEM_API DeviceTexture2D : private Interface, public DeviceTextureBase
	{
	public:
		virtual ~DeviceTexture2D();

		static DeviceTexture2D* CreateArray(
			RenderSystem* pRenderSys,
			int32 X, int32 Y,
			DataFormat format,
			ResourceUsage usage,
			ResourceBindFlag flag,
			int32 arraySize,
			byte const* initialData);
		static DeviceTexture2D* Create(
			RenderSystem* pRenderSys,
			int32 X, int32 Y,
			DataFormat format,
			ResourceUsage usage,
			ResourceBindFlag flag,
			byte const* initialData);
		static DeviceTexture2D* CreateFromFile(
			RenderSystem* pRenderSys, std::string const &filename, 
			DataFormat format, ResourceUsage usage, ResourceBindFlag flag);
		 
		virtual byte* Lock() = 0;
		virtual void Unlock() = 0;

		inline int32 GetArraySize() const
		{
			return m_ArraySize;
		}
		inline int32 GetWidth() const
		{
			return m_Width;
		}
		inline int32 GetHeight() const
		{
			return m_Height;
		}
		 
	protected:
		DeviceTexture2D(int32 X, int32 Y,
			DataFormat format,
			ResourceUsage usage,
			ResourceBindFlag flag,
			int32 arraySize);
		int32 m_Width = 0, m_Height = 0; 
		int32 m_ArraySize = 0;
	};
	typedef std::shared_ptr<DeviceTexture2D> DeviceTexture2DPtr;

	class SPACE_RENDERSYSTEM_API DeviceTexture3D : private Interface, public DeviceTextureBase
	{
	public:
		virtual ~DeviceTexture3D();

		static DeviceTexture3D* Create(
			RenderSystem* pRenderSys,
			int32 X, int32 Y, int32 Z,
			DataFormat format,
			ResourceUsage usage,
			ResourceBindFlag flag,
			byte const* initialData);
		
		virtual byte* Lock() = 0;
		virtual void Unlock() = 0;

		inline int32 GetWidth() const
		{
			return m_Width;
		}
		inline int32 GetHeight() const
		{
			return m_Height;
		}
		inline int32 GetDepth() const
		{
			return m_Depth;
		}

	protected:
		DeviceTexture3D(int32 X, int32 Y, int32 Z,
			DataFormat format,
			ResourceUsage usage,
			ResourceBindFlag flag);
		int32 m_Width = 0, m_Height = 0, m_Depth = 0; 
	};
	typedef std::shared_ptr<DeviceTexture3D> DeviceTexture3DPtr;
}

#endif