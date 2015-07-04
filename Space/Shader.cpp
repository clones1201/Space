#include "Log.h"
#include "Utility.hpp"
#include "RenderSystem.hpp"
#include "Shader.hpp"

namespace Space
{	
	ShaderBase::ShaderBase()
	{
	}

	ShaderBase::~ShaderBase()
	{
	}

	void ShaderBase::ClearAllSlots()
	{
		m_ConstantBufferTable.clear();
		m_ShaderResourceTable.clear();
		m_MaxCBSlot = 0;
		m_MaxSRSlot = 0;
	}

	void ShaderBase::SetConstantBuffer(uint slots, ConstantBuffer* buffer)
	{
		assert(m_MaxCBSlot >= 0 && slots < 4096);
		m_MaxCBSlot = std::max(slots, m_MaxCBSlot);
		m_ConstantBufferTable[slots] = buffer;
	}

	void ShaderBase::SetShaderResource(uint slots, ShaderResource* resource)
	{
		assert(m_MaxSRSlot >= 0 && slots < 4096);
		m_MaxSRSlot = std::max(slots, m_MaxSRSlot);
		m_ShaderResourceTable[slots] = resource;
	}
	
	ConstantBuffer* ShaderBase::GetConstantBuffer(uint slots) const
	{
		TRY_CATCH_OUT_OF_RANGE(
			return m_ConstantBufferTable.at(slots),
			return nullptr
			);
	}

	ShaderResource* ShaderBase::GetShaderResource(uint slots) const
	{
		TRY_CATCH_OUT_OF_RANGE(
			return m_ShaderResourceTable.at(slots),
			return nullptr
			);
	}

	/* VertexShader implement */

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