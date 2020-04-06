#pragma once

#include "RenderCommon.hpp"

#include <d3d11_2.h>
#include <d3dcompiler.h>

namespace Space
{
	namespace Render {
		class D3D11Device;
		class D3D11RenderSystem;
		class D3D11RenderTarget;
		class D3D11DepthStencilView;
		class D3D11DeviceBuffer;
		class D3D11DeviceTexture1D;
		class D3D11DeviceTexture2D;
		class D3D11DeviceTexture3D; 
		class D3D11VertexShader;
		class D3D11PixelShader;
		class D3D11ShaderResource;
		class D3D11ShaderReflection;
		class D3D11CommandList;
		class D3D11PipelineState;
	}
}

#ifdef RENDERCORE_EXPORTS
#define SPACE_D3D11_API __declspec(dllexport)
#else
#define SPACE_D3D11_API __declspec(dllimport)
#endif
