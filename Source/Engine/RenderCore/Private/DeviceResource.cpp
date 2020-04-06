#include "DeviceResource.hpp"

namespace Space
{
	namespace Render {

		VertexBuffer::~VertexBuffer()
		{
		}

		VertexBuffer::VertexBuffer(
			size_t lengthInBytes, size_t stride,
			byte const* initialData):
			_Stride(stride),
			_Offset(0),
			Buffer(Device::GetInstance(), BufferType::VertexBuffer, ResourceUsage::Default, lengthInBytes, initialData)
		{
		}

		IndexBuffer::~IndexBuffer()
		{
		}

		IndexBuffer::IndexBuffer(
			size_t lengthInBytes, DataFormat format, byte const* initialData):
			_Format(format),
			Buffer(Device::GetInstance(), BufferType::IndexBuffer, ResourceUsage::Default, lengthInBytes, initialData)
		{
		}

		ConstantBuffer::ConstantBuffer(
			size_t lengthInBytes, byte const* initialData) :
			_pShadowData((byte*)_aligned_malloc(GetLengthInBytes(), 16)),
			Buffer(Device::GetInstance(), BufferType::ConstantBuffer, ResourceUsage::Default, lengthInBytes, initialData)
		{
		}

		ConstantBuffer::~ConstantBuffer()
		{
			if (_pShadowData != nullptr)
				_aligned_free(_pShadowData);
		}

		void ConstantBuffer::UpdateToDevice(CommandList* pList)
		{
			Buffer::Update(pList, 0, GetLengthInBytes(), _pShadowData);
		}

		bool ConstantBuffer::Update(CommandList* pList, size_t startOffset, size_t lengthInBytes, byte const* pData)
		{
			auto error = memcpy_s(_pShadowData + startOffset, GetLengthInBytes() - startOffset, pData, lengthInBytes);

			if (error == 0)
				return true;
			return false;
		}

		TextureBuffer::~TextureBuffer()
		{}

		TextureBuffer::TextureBuffer(
			size_t lengthInBytes, size_t sizeOfElem, size_t numElements, byte const* initialData) :
			_SizeOfElem(sizeOfElem),
			_NumElements(numElements),
			Buffer(Device::GetInstance(), BufferType::TextureBuffer, ResourceUsage::Default, lengthInBytes, initialData)
		{}

		namespace Details {
			
		}
	}
}