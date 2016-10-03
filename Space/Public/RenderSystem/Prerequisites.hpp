#ifndef __SPACE_RENDERSYSTEM_PREREQUISITES_HPP__
#define __SPACE_RENDERSYSTEM_PREREQUISITES_HPP__

#include "Common\Common.hpp"

#ifdef SPACE_EXPORTS
#define SPACE_RENDERSYSTEM_API __declspec(dllexport)
#else
#define SPACE_RENDERSYSTEM_API __declspec(dllimport)
#endif

namespace Space
{
	class RenderSystem;
	class DeviceBuffer;
	class VertexBuffer;
	class IndexBuffer;
	class ConstantBuffer;
	class TextureBuffer;
	class DeviceTexture1D;
	class DeviceTexture2D;
	class DeviceTexture3D;
	class DepthStencilView;
	class RenderTarget;
	class RenderWindow;
	class VertexShader;
	class PixelShader;
	class Shader;
	class ShaderReflection;
	class ShaderResource;
	class ShaderResourcePool;

	class InputLayout;
	class CommandList;
	class PipelineState;
	class Renderable;
	class Mesh;
	class Material;
	class UniformExpression;
	class ResourceTexture;
	class SceneManager;

	
}

#endif