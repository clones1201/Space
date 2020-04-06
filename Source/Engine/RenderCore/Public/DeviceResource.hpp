#pragma once

#include "RenderCommon.hpp"
#include "Shared.hpp"

#include "RenderSystem.hpp"
#include "Rendering.hpp"

namespace Space {
namespace Render {
	namespace Details {

		template <class RenderSystem>
		class RENDERCORE_API BufferImpl :
			protected SharedPtrObject<BufferImpl<RenderSystem>>
		{
		public:
			typedef typename std::remove_reference<typename RenderSystem::DeviceBuffer>::type DeviceBuffer;
			typedef typename std::remove_reference<typename RenderSystem::CommandList>::type CommandList;
			virtual ~BufferImpl()
			{}

			BufferImpl(DeviceImpl<RenderSystem>* device,BufferType type, ResourceUsage usage, size_t lengthInBytes, byte const* initialData)
				:_Type(type), _Usage(usage), _LengthInBytes(lengthInBytes)
			{
				_deviceBuffer = std::make_unique<DeviceBuffer>(
					device, type, usage, initialData, lengthInBytes);
			}

			DeviceBuffer* GetBuffer() const {
				return _deviceBuffer.get();
			}

			inline size_t GetLengthInBytes() const
			{
				return _LengthInBytes;
			}
			inline ResourceUsage GetUsage() const
			{
				return _Usage;
			}
			inline BufferType GetBufferType() const
			{
				return _Type;
			}
			virtual bool Update(CommandList* pList,
				size_t startOffset, size_t lengthInBytes, byte const* pData)
			{
				size_t lengthInTotal = _LengthInBytes;
				return _deviceBuffer->Update(
					pList,
					_Usage, _Type, lengthInTotal, startOffset, lengthInBytes, pData);
			}

		protected:
			BufferType _Type;
			ResourceUsage _Usage;
			size_t _LengthInBytes;

			typename std::unique_ptr<DeviceBuffer> _deviceBuffer = nullptr;
		};

		template<class RenderSystem, class Texture>
		class RENDERCORE_API TextureImpl :
			public SharedPtrObject<TextureImpl<RenderSystem, Texture>>,
			public Texture
		{
		public:
			typedef typename std::remove_reference<typename RenderSystem::Device>::type Device;
			virtual ~TextureImpl() {}
			inline DataFormat GetFormat() const
			{
				return _Format;
			}
			inline ResourceUsage GetUsage() const
			{
				return _Usage;
			}
			inline ResourceBindFlag GetBindFlag() const
			{
				return _Flag;
			}

			TextureImpl(Device* device, int32 X, int32 Y, int32 Z,
				DataFormat format,
				ResourceUsage usage,
				ResourceBindFlag flag)
				:Texture(device, X, Y, format, usage, flag, 1, nullptr)
			{
			}
		protected:
			DataFormat _Format = DataFormat::UNKNOWN;
			ResourceUsage _Usage = ResourceUsage::Default;
			ResourceBindFlag _Flag = ResourceBindFlag::None;
		};
	}

	class RENDERCORE_API VertexBuffer :
		SharedPtrObject<VertexBuffer>,
		public Buffer
	{
	public:
		virtual ~VertexBuffer();

		VertexBuffer(size_t lengthInBytes, size_t stride, byte const* initialData);

		inline size_t GetStride() const
		{
			return _Stride;
		}
		inline size_t GetOffset() const
		{
			return _Offset;
		}
		inline void SetStride(size_t stride)
		{
			assert(stride >= 0);
			_Stride = stride;
		}
		inline void SetOffest(size_t offset)
		{
			assert(offset >= 0);
			_Offset = offset;
		}
	protected:
		size_t _Offset = 0;
		size_t _Stride = 0;
	};

	class RENDERCORE_API IndexBuffer :
		public SharedPtrObject<IndexBuffer>,
		public Buffer
	{
	public:
		virtual ~IndexBuffer();

		IndexBuffer(size_t lengthInBytes, DataFormat format, byte const* initialData);
		inline DataFormat GetFormat() const
		{
			return _Format;
		}
		inline size_t GetOffset() const
		{
			return _Offset;
		}
		inline void SetDataFormat(DataFormat format)
		{
			_Format = format;
		}
		inline void SetOffest(size_t offset)
		{
			_Offset = offset;
		}

	protected:
		size_t _Offset = 0;
		DataFormat _Format = DataFormat::UNKNOWN;
	};

	class RENDERCORE_API ConstantBuffer :
		public SharedPtrObject<ConstantBuffer>,
		public Buffer
	{
	public:
		ConstantBuffer(
			size_t lengthInBytes, byte const* initialData);

		~ConstantBuffer();
		void UpdateToDevice(CommandList* pList);

		bool Update(CommandList* pList, size_t startOffset, size_t lengthInBytes, byte const* pData);
		inline byte const* GetBufferPointer() const
		{
			return _pShadowData;
		}
	protected:
		byte *_pShadowData = nullptr;

		friend class ShaderVariableAccessor;
		friend class ConstantBufferAccessor;
	};

	class RENDERCORE_API TextureBuffer :
		public SharedPtrObject<TextureBuffer>,
		public Buffer
	{
	public:
		virtual ~TextureBuffer();

		TextureBuffer(
			size_t lengthInBytes, size_t sizeOfElem, size_t numElements, byte const* initialData);
		inline size_t GetSizeOfElement() const
		{
			return _SizeOfElem;
		}
		size_t GetElementCount() const
		{
			return _NumElements;
		}
	protected:
		size_t _SizeOfElem;
		size_t _NumElements;
	};
}
}