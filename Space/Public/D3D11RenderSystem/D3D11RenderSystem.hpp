#pragma once

#include "D3D11Prerequisites.hpp" 
#include "D3D11Shared.hpp"
#include "D3D11Device.hpp"
#include "D3D11DeviceResource.hpp"
#include "D3D11PipelineState.hpp"
#include "D3D11CommandList.hpp"
#include "D3D11RenderTarget.hpp"
#include "D3D11ResourceView.hpp"
#include "D3D11Shader.hpp"
#include "D3D11ShaderReflection.hpp"

namespace Space {
	namespace Render {

		class D3D11RenderSystem
		{
		public:
			typedef typename D3D11Device Device;
			typedef typename D3D11RenderTarget RenderTarget;
			typedef typename D3D11DeviceBuffer DeviceBuffer;
			typedef typename D3D11DeviceTexture1D DeviceTexture1D;
			typedef typename D3D11DeviceTexture2D DeviceTexture2D;
			typedef typename D3D11DeviceTexture3D DeviceTexture3D;
			typedef typename D3D11VertexShader VertexShader;
			typedef typename D3D11PixelShader PixelShader;
			typedef typename D3D11ShaderResource ShaderResource;
			typedef typename D3D11CommandList CommandList;
			typedef typename D3D11PipelineState PipelineState;
			typedef typename D3D11RenderTarget RenderTarget;
			typedef typename D3D11DepthStencilView DepthStencilView;
			typedef typename D3D11RenderWindow RenderWindow;
		};
	}
}
