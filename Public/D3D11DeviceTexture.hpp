#ifndef __SPACE_D3D11_DEVICETEXTURE_HPP__
#define __SPACE_D3D11_DEVICETEXTURE_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"
#include "DeviceTexture.hpp"
#include "D3D11Prerequisites.hpp" 

namespace Space
{ 

	class D3D11DeviceTexture1D : public DeviceTexture1D
	{
	public:
		static D3D11DeviceTexture1D* CreateArray(D3D11Device &device, int X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int arraySize, byte const* initialData);
		static D3D11DeviceTexture1D* Create(D3D11Device &device, int X, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData);

		//static D3D11DeviceTexture1D* CreateFromFile(std::string const &filename);

		virtual int GetArraySize() const = 0;
		virtual TextureType GetType() const = 0;
	};

	class D3D11DeviceTexture2D : public DeviceTexture2D
	{
	public:
		static D3D11DeviceTexture2D* CreateArray(D3D11Device &device, int X, int Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int arraySize, byte const* initialData);
		static D3D11DeviceTexture2D* Create(D3D11Device &device, int X, int Y, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData);

		static D3D11DeviceTexture2D* CreateFromFile(std::string const &filename);

		virtual RenderTarget* GetRenderTarget() const = 0;
		virtual DepthStencilView* GetDepthStencil() const = 0;

		virtual int GetArraySize() const = 0;
		virtual TextureType GetType() const = 0;
	};

	class D3D11DeviceTexture3D : public DeviceTexture3D
	{
	public:
		/*static D3D11DeviceTexture3D* CreateArray(D3D11Device &device, int X, int Y, int Z, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, int arraySize, byte const* initialData);*/
		static D3D11DeviceTexture3D* Create(D3D11Device &device, int X,int Y,int Z, DataFormat format, ResourceUsage usage, ResourceBindFlag flag, byte const* initialData);

		//static D3D11DeviceTexture3D* CreateFromFile(std::string const &filename);

		virtual int GetArraySize() const = 0;
		virtual TextureType GetType() const = 0;
	};

}

#endif