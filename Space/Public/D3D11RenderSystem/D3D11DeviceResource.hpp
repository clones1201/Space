#ifndef __SPACE_D3D11_DEVICEBUFFER_HPP__
#define __SPACE_D3D11_DEVICEBUFFER_HPP__

#include "D3D11RenderSystem/D3D11Prerequisites.hpp"
#include "D3D11RenderSystem/D3D11Device.hpp"

#include "RenderSystem/DeviceResource.hpp"

namespace Space
{
	class SPACE_D3D11_API D3D11DeviceBuffer : public DeviceBuffer
	{
	public:
		static D3D11DeviceBuffer* Create(D3D11DevicePtr device, BufferType type, ResourceUsage usage, byte const* initialData, size_t lengthInBytes);
				
		virtual bool Update(CommandList* pList, size_t startOffset, size_t lengthInBytes, byte const* pData);

		CComPtr<ID3D11Buffer> GetRawBuffer() const;
	protected:
		D3D11DeviceBuffer(D3D11DevicePtr device, BufferType type,
			ResourceUsage usage, byte const* initialData, size_t lengthInBytes);
		
	private:
		CComPtr<ID3D11Buffer> m_pBuffer = nullptr;

		D3D11DevicePtr mDevice;
		friend class DeviceBuffer;
	};
	typedef std::shared_ptr<D3D11DeviceBuffer> D3D11DeviceBufferPtr;

	
	class SPACE_D3D11_API D3D11DeviceTexture1D : public DeviceTexture1D
	{
	public:
		static D3D11DeviceTexture1D* CreateArray(
			D3D11DevicePtr device, 
			int32 X, 
			DataFormat format, ResourceUsage usage, ResourceBindFlag flag, 
			int32 arraySize, byte const* initialData);
		static D3D11DeviceTexture1D* Create(
			D3D11DevicePtr device,
			int32 X, 
			DataFormat format, ResourceUsage usage, ResourceBindFlag flag, 
			byte const* initialData);
		static D3D11DeviceTexture1D* Create(D3D11DevicePtr device, ID3D11Texture1D* pTexture);

		virtual ID3D11Texture1D* GetD3DTexture1D() = 0;
	protected:
		D3D11DeviceTexture1D(int32 X,
			DataFormat format,
			ResourceUsage usage,
			ResourceBindFlag flag,
			int32 arraySize);
	};
	typedef std::shared_ptr<D3D11DeviceTexture1D> D3D11DeviceTexture1DPtr;

	class SPACE_D3D11_API D3D11DeviceTexture2D : public DeviceTexture2D
	{
	public:
		static D3D11DeviceTexture2D* CreateArray(
			D3D11DevicePtr device, 
			int32 X, int32 Y, 
			DataFormat format, ResourceUsage usage, ResourceBindFlag flag, 
			int32 arraySize, byte const* initialData);
		static D3D11DeviceTexture2D* Create(
			D3D11DevicePtr device, 
			int32 X, int32 Y, 
			DataFormat format, ResourceUsage usage, ResourceBindFlag flag, 
			byte const* initialData);
		static D3D11DeviceTexture2D* CreateFromFile(D3D11DevicePtr device, std::string const &filename
			,DataFormat format, ResourceUsage usage, ResourceBindFlag flag);

		static D3D11DeviceTexture2D* CreateFromFile(D3D11DevicePtr device, std::wstring const &filename
			, DataFormat format, ResourceUsage usage, ResourceBindFlag flag);

		static D3D11DeviceTexture2D* Create(D3D11DevicePtr device, ID3D11Texture2D* pTexture);

		virtual ID3D11Texture2D* GetD3DTexture2D() = 0;
	protected:
		D3D11DeviceTexture2D(int32 X,int32 Y,
			DataFormat format,
			ResourceUsage usage,
			ResourceBindFlag flag,
			int32 arraySize);
	};
	typedef std::shared_ptr<D3D11DeviceTexture2D> D3D11DeviceTexture2DPtr;

	class SPACE_D3D11_API D3D11DeviceTexture3D : public DeviceTexture3D
	{
	public:
		/*static D3D11DeviceTexture3D* CreateArray(D3D11DevicePtr device, int32 X, int32 Y, int32 Z, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int32 arraySize, byte const* initialData);*/
		static D3D11DeviceTexture3D* Create(
			D3D11DevicePtr device, 
			int32 X,int32 Y,int32 Z, 
			DataFormat format, ResourceUsage usage, ResourceBindFlag flag, 
			byte const* initialData);
		 
		static D3D11DeviceTexture3D* Create(D3D11DevicePtr device, ID3D11Texture3D* pTexture);

		virtual ID3D11Texture3D* GetD3DTexture3D() = 0;
	protected:
		D3D11DeviceTexture3D(int32 X,int32 Y,int32 Z,
			DataFormat format,
			ResourceUsage usage,
			ResourceBindFlag flag);
	}; 
	typedef std::shared_ptr<D3D11DeviceTexture3D> D3D11DeviceTexture3DPtr;
}

#endif