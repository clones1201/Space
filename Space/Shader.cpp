#include "Log.h"
#include "Utility.h"
#include "SharedUtility.hpp"
#include "RenderSystem.hpp"
#include "Shader.hpp"

namespace Space
{	
	VertexShader* VertexShader::LoadBinaryFromMemory(RenderSystem* pRenderSys,
		byte const* byteCodes, uint32 sizeInBytes)
	{
		return pRenderSys->LoadVertexShaderFromMemory(byteCodes, sizeInBytes);
	}

	VertexShader::VertexShader()
	{}

	VertexShader::~VertexShader()
	{}


	PixelShader* PixelShader::LoadBinaryFromMemory(RenderSystem* pRenderSys,
		byte const* byteCodes, uint32 sizeInBytes)
	{
		return pRenderSys->LoadPixelShaderFromMemory(byteCodes, sizeInBytes);
	}

	PixelShader::PixelShader()
	{}

	PixelShader::~PixelShader()
	{}

}