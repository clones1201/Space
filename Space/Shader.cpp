#include "Log.h"
#include "Utility.hpp"
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

	byte const* VertexShader::GetByteCodes() const
	{
		return m_ByteCodes;
	}

	uint32 VertexShader::GetSizeInBytes() const
	{
		return m_SizeInBytes;
	}

	PixelShader* PixelShader::LoadBinaryFromMemory(RenderSystem* pRenderSys,
		byte const* byteCodes, uint32 sizeInBytes)
	{
		return pRenderSys->LoadPixelShaderFromMemory(byteCodes, sizeInBytes);
	}

	PixelShader::PixelShader()
	{}

	PixelShader::~PixelShader()
	{}

	byte const* PixelShader::GetByteCodes() const
	{
		return m_ByteCodes;
	}

	uint32 PixelShader::GetSizeInBytes() const
	{
		return m_SizeInBytes;
	}

}