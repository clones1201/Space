#include "RenderSystem/RenderSystem.hpp"

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
		m_MaxCBSlot = std::max((int32)slots, m_MaxCBSlot);
		m_ConstantBufferTable[slots] = buffer;
	}

	void ShaderBase::SetShaderResource(uint slots, ShaderResource* resource)
	{
		assert(m_MaxSRSlot >= 0 && slots < 4096);
		m_MaxSRSlot = std::max((int32)slots, m_MaxSRSlot);
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
		byte const* byteCodes, size_t sizeInBytes)
	{
		return pRenderSys->LoadVertexShaderFromMemory(byteCodes, sizeInBytes);
	}

	VertexShader::VertexShader()
	{}

	VertexShader::~VertexShader()
	{}

	PixelShader* PixelShader::LoadBinaryFromMemory(RenderSystem* pRenderSys,
		byte const* byteCodes, size_t sizeInBytes)
	{
		return pRenderSys->LoadPixelShaderFromMemory(byteCodes, sizeInBytes);
	}

	PixelShader::PixelShader()
	{}

	PixelShader::~PixelShader()
	{}
	
	Shader* Shader::Create(RenderSystem* pRenderSys, std::wstring const& name)
	{
		return Create(pRenderSys, wstr2str(name));
	}

	Shader* Shader::Create(RenderSystem* pRenderSys, std::string const& name)
	{
		TRY_CATCH_LOG(
			return new Shader(pRenderSys, name),
			return nullptr);
	}

	Shader::Shader(RenderSystem* pRenderSys, std::string const& name)
	{
		std::fstream tempFile(name + ".ps.cso", std::ios_base::in | std::ios_base::binary);
		if (!tempFile.is_open())
		{
			throw std::exception("Missing Shader File");
		}
		std::vector<char> psCode(
			(std::istreambuf_iterator<char>(tempFile)),
			(std::istreambuf_iterator<char>()));

		tempFile.close();
		tempFile.open(name + ".vs.cso", std::ios_base::in | std::ios_base::binary);
		if (!tempFile.is_open())
		{
			throw std::exception("Missing Shader File");
		}
		std::vector<char> vsCode(
			(std::istreambuf_iterator<char>(tempFile)),
			(std::istreambuf_iterator<char>()));

		m_pVS.reset(
			VertexShader::LoadBinaryFromMemory(
				pRenderSys, (byte*)vsCode.data(), vsCode.size()));
		m_pPS.reset(
			PixelShader::LoadBinaryFromMemory(
				pRenderSys, (byte*)psCode.data(), psCode.size()));

		m_pVSReflect.reset( m_pVS->CreateReflection() );
		m_pPSReflect.reset( m_pPS->CreateReflection() );
	}

	Shader::~Shader()
	{}

	VertexShader* Shader::GetVertexShader() const
	{
		return m_pVS.get();
	}

	PixelShader* Shader::GetPixelShader() const
	{
		return m_pPS.get();
	}
}