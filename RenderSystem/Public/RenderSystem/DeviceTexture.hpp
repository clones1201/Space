#ifndef __SPACE_RENDERSYSTEM_DEVICETEXTURE_HPP__
#define __SPACE_RENDERSYSTEM_DEVICETEXTURE_HPP__

#include "RenderSystem/Prerequisites.hpp"
#include "RenderSystem/Shared.hpp"

namespace Space
{
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