#pragma once

#include "RenderCommon.hpp"
#include "D3D11Prerequisites.hpp"
#include "D3D11Device.hpp"

namespace Space 
{
namespace Render 
{
	class SPACE_D3D11_API D3D11DeviceBuffer : public Uncopyable
	{
	public:
		typedef CComPtr<ID3D11Buffer> RawPtrType;

		D3D11DeviceBuffer(D3D11Device* device, BufferType type,
			ResourceUsage usage, byte const* initialData, size_t lengthInBytes);

		bool Update(
			D3D11CommandList* pCommandList,
			ResourceUsage usage,
			BufferType type,
			size_t lengthInTotal, size_t startOffset, size_t lengthInBytes,
			byte const* pData);
			
		typedef CComPtr<ID3D11Buffer> RawBufferPtr;
		inline CComPtr<ID3D11Buffer> GetRawPtr() const
		{
			return m_pBuffer;
		}
	private:
		CComPtr<ID3D11Buffer> m_pBuffer = nullptr;
	};

	class SPACE_D3D11_API D3D11DeviceTexture1D : public Uncopyable
	{
	public:
		inline CComPtr<ID3D11Texture1D> GetD3DTexture1D() const
		{
			return m_pTexture1D;
		}

		D3D11DeviceTexture1D(
			D3D11Device* device,
			int32 X,
			DataFormat format,
			ResourceUsage usage,
			ResourceBindFlag flag,
			int32 arraySize,
			byte const* initialData);
		D3D11DeviceTexture1D(
			D3D11Device* device,
			D3D11_TEXTURE1D_DESC desc,
			CComPtr<ID3D11Texture1D> pTexture);

		byte* Lock(D3D11CommandList* pCommandList);
		void Unlock(D3D11CommandList* pCommandList);

		inline int32 GetArraySize() const
		{
			return m_Desc.ArraySize;
		}
		inline int32 GetWidth() const
		{
			return m_Desc.Width;
		}
		inline int32 GetHeight() const
		{
			return 1;
		}
		inline int32 GetDepth() const
		{
			return 1;
		}
		inline DataFormat GetFormat() const
		{
			return m_DataFormat;
		}
		inline ResourceUsage GetUsage() const
		{
			return (ResourceUsage)m_Desc.Usage;
		}
		inline ResourceBindFlag GetBindFlags() const
		{
			return (ResourceBindFlag)m_Desc.BindFlags;
		}
	private:
		CComPtr<ID3D11Texture1D> m_pTexture1D = nullptr;

		DataFormat m_DataFormat;
		D3D11Device* m_Device;
		D3D11_TEXTURE1D_DESC m_Desc;
		byte* m_pLock = nullptr;
		D3D11_MAPPED_SUBRESOURCE m_Subres;
	};

	class SPACE_D3D11_API D3D11DeviceTexture2D : public Uncopyable
	{
	public:
		inline CComPtr<ID3D11Texture2D> GetRawTexture2D() const
		{
			return m_pTexture2D;
		}
		D3D11DeviceTexture2D(
			D3D11Device* device, 
			int32 X, int32 Y,
			DataFormat format,
			ResourceUsage usage,
			ResourceBindFlag flag,
			int32 arraySize,
			byte const* initialData);
		D3D11DeviceTexture2D(
			D3D11Device* device,
			CComPtr<ID3D11Texture2D> pTexture);
		byte* Lock(D3D11CommandList* pCommandList);
		void Unlock(D3D11CommandList* pCommandList);

		inline int32 GetArraySize() const
		{
			return m_Desc.ArraySize;
		}
		inline int32 GetWidth() const 
		{
			return m_Desc.Width;
		}
		inline int32 GetHeight() const
		{
			return m_Desc.Height;
		}
		inline int32 GetDepth() const
		{
			return 1;
		}
		inline DataFormat GetFormat() const
		{
			return m_DataFormat;
		}
		inline ResourceUsage GetUsage() const
		{
			return (ResourceUsage)m_Desc.Usage;
		}
		inline ResourceBindFlag GetBindFlags() const
		{
			return (ResourceBindFlag)m_Desc.BindFlags;
		}
	private:
		CComPtr<ID3D11Texture2D> m_pTexture2D = nullptr;

		D3D11_TEXTURE2D_DESC m_Desc;

		DataFormat m_DataFormat;
		D3D11Device* m_Device;
		byte* m_pLock = nullptr;
		D3D11_MAPPED_SUBRESOURCE m_Subres;
	};

	class SPACE_D3D11_API D3D11DeviceTexture3D : public Uncopyable
	{
	public:
		inline ID3D11Texture3D* GetD3DTexture3D()
		{
			return m_pTexture3D;
		}
		byte* Lock(D3D11CommandList* pCommandList);
		void Unlock(D3D11CommandList* pCommandList);
		inline int32 GetArraySize() const
		{
			return 1;
		}
		inline int32 GetWidth() const
		{
			return m_Desc.Width;
		}
		inline int32 GetHeight() const
		{
			return m_Desc.Height;
		}
		inline int32 GetDepth() const
		{
			return m_Desc.Depth;
		}
		inline DataFormat GetFormat() const
		{
			return m_DataFormat;
		}
		inline ResourceUsage GetUsage() const
		{
			return (ResourceUsage)m_Desc.Usage;
		}
		inline ResourceBindFlag GetBindFlags() const
		{
			return (ResourceBindFlag)m_Desc.BindFlags;
		}
		D3D11DeviceTexture3D(
			D3D11Device* device,
			int32 X, int32 Y, int32 Z,
			DataFormat format,
			ResourceUsage usage,
			ResourceBindFlag flag,
			byte const* initialData
		);
		
		D3D11DeviceTexture3D(
			D3D11Device* device,
			D3D11_TEXTURE3D_DESC desc,
			ID3D11Texture3D* pTex);
	
	private:	
		CComPtr<ID3D11Texture3D> m_pTexture3D = nullptr;

		D3D11_TEXTURE3D_DESC m_Desc;

		DataFormat m_DataFormat;
		D3D11Device* m_Device;
		byte* m_pLock = nullptr;
		D3D11_MAPPED_SUBRESOURCE m_Subres;
	};
}
}