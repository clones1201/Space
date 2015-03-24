#ifndef __SPACE_DEVICETEXTURE_HPP__
#define __SPACE_DEVICETEXTURE_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp" 

namespace Space
{ 
	class DeviceTexture1D : virtual public Interface 
	{
	public:
		virtual ~DeviceTexture1D();

		static DeviceTexture1D* CreateArray(RenderSystem* pRenderSys,int X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int arraySize, byte const* initialData);
		static DeviceTexture1D* Create(RenderSystem* pRenderSys, int X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData);
		//static DeviceTexture1D* CreateFromFile(std::string const &filename);

		virtual int GetArraySize() const = 0;
		virtual TextureType GetType() const = 0;
	};

	class DeviceTexture2D : virtual public Interface
	{
	public:
		virtual ~DeviceTexture2D();

		static DeviceTexture2D* CreateArray(RenderSystem* pRenderSys, int X, int Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int arraySize, byte const* initialData);
		static DeviceTexture2D* Create(RenderSystem* pRenderSys, int X, int Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData);
		static DeviceTexture2D* CreateFromFile(RenderSystem* pRenderSys, std::string const &filename);

		virtual RenderTarget* GetRenderTarget() const = 0;
		virtual DepthStencilView* GetDepthStencil() const = 0;

		virtual int GetArraySize() const = 0;
		virtual TextureType GetType() const = 0;
	};

	class DeviceTexture3D : virtual public Interface
	{
	public:
		virtual ~DeviceTexture3D();

		static DeviceTexture3D* CreateArray(RenderSystem* pRenderSys, int X, int Y, int Z, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int arraySize, byte const* initialData);
		static DeviceTexture3D* Create(RenderSystem* pRenderSys, int X,int Y,int Z, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData);
		//static DeviceTexture3D* CreateFromFile(std::string const &filename);
		
		virtual int GetArraySize() const = 0;
		virtual TextureType GetType() const = 0;
	};

}

#endif