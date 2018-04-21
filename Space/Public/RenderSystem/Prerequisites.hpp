#pragma once

#include "Common\Common.hpp"
#include "Common\RenderCommon.hpp"

#ifdef SPACE_EXPORTS
#define SPACE_RENDERSYSTEM_API __declspec(dllexport)
#else
#define SPACE_RENDERSYSTEM_API __declspec(dllimport)
#endif

#if SPACE_PLATFORM == SPACE_WIN32
#include "D3D11RenderSystem/D3D11RenderSystem.hpp"
#define DEFINE_IMPL(CLASS_NAME)\
		using CLASS_NAME = Details::CLASS_NAME##Impl<D3D11##CLASS_NAME>
#else
#error Unsupported platform
#endif

namespace Space
{
	namespace Render {
		namespace Details {
			template<class RenderSystem> class DeviceImpl;
			template<class RenderSystem> class BufferImpl;
			template<class RenderSystem, class Texture> class TextureImpl;
			/*template<class Impl> class DepthStencilViewImpl;
			template<class Impl> class ShaderResourceViewImpl;*/
			template<class RenderSystem> class CommandListImpl;
			template<class RenderSystem> class PipelineStateImpl;
			template<class RenderSystem> class RenderTargetImpl;
			template<class RenderSystem> class DepthStencilViewImpl;
			template<class RenderSystem> class RenderWindowImpl;
			template<class RenderSystem> class VertexShaderImpl;
			template<class RenderSystem> class PixelShaderImpl;
			/*template<class Impl> class ShaderReflectionImpl;
			template<class Impl> class ShaderReflectionConstantBufferImpl;
			template<class Impl> class ShaderReflectionBindResourceImpl;
			template<class Impl> class ShaderReflectionVariableImpl;*/

		}

#if SPACE_PLATFORM == SPACE_WIN32
		using Device = Details::DeviceImpl<D3D11RenderSystem>;
		using Buffer = Details::BufferImpl<D3D11RenderSystem>;
		using Texture1D = Details::TextureImpl<D3D11RenderSystem, D3D11DeviceTexture1D>;
		using Texture2D = Details::TextureImpl<D3D11RenderSystem, D3D11DeviceTexture2D>;
		using Texture3D = Details::TextureImpl<D3D11RenderSystem, D3D11DeviceTexture3D>;
		using CommandList = Details::CommandListImpl<D3D11RenderSystem>;
		using PipelineState = Details::PipelineStateImpl<D3D11RenderSystem>;
		using RenderTarget = Details::RenderTargetImpl<D3D11RenderSystem>;
		using DepthStencilView = Details::DepthStencilViewImpl<D3D11RenderSystem>;
		using RenderWindow = Details::RenderWindowImpl<D3D11RenderSystem>;
		using VertexShader = Details::VertexShaderImpl<D3D11RenderSystem>;
		using PixelShader = Details::PixelShaderImpl<D3D11RenderSystem>;
#endif

		class VertexBuffer;
		class IndexBuffer;
		class ConstantBuffer;
		class TextureBuffer;

		class Material;
		class Renderer;
	}
}
