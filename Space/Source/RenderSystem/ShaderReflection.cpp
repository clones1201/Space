#include "RenderSystem/RenderSystem.hpp"

namespace Space
{

	ShaderReflection::ShaderReflection(ShaderBase* pShader)
		:pShader(pShader)
	{
	}

	ShaderReflection::~ShaderReflection()
	{}

	//
	// ShaderReflectionConstantBuffer Implement
	//

	ShaderReflectionConstantBuffer::ShaderReflectionConstantBuffer(
		ShaderReflection* pReflection)
		:pReflection(pReflection){
	}

	ShaderReflectionConstantBuffer::~ShaderReflectionConstantBuffer()
	{}
	
	ShaderReflectionVariable* ShaderReflectionConstantBuffer::GetVariableByIndex(int32 index) const
	{
		TRY_CATCH_LOG(return m_Variables.at(index).get(), return nullptr);
	}

	//
	// ShaderReflectionVariable Implement
	//

	ShaderReflectionVariable::ShaderReflectionVariable(
		ShaderReflectionConstantBuffer *pCB)
		:pParentCB(pCB), pParentVariable(nullptr)
	{}

	ShaderReflectionVariable::ShaderReflectionVariable(
		ShaderReflectionVariable* pVariable)
		: pParentVariable(pVariable), pParentCB(nullptr)
	{}

	ShaderReflectionVariable::~ShaderReflectionVariable()
	{}

	std::string ShaderReflectionVariable::AsString() const
	{
		return std::string(
			pParentCB->GetBuffer()->GetBufferPointer() + m_StartOffset,
			pParentCB->GetBuffer()->GetBufferPointer() + m_StartOffset + m_Size
			);
	}
	bool ShaderReflectionVariable::AsBool() const
	{
		if (m_Size < sizeof(bool)) return bool();
		return *(bool*)(pParentCB->GetBuffer()->GetBufferPointer() + m_StartOffset);
	}
	int32 ShaderReflectionVariable::AsInt() const
	{
		if (m_Size < sizeof(int32)) return int32();
		return *(int32*)(pParentCB->GetBuffer()->GetBufferPointer() + m_StartOffset);
	}
	uint32 ShaderReflectionVariable::AsUint() const
	{
		if (m_Size < sizeof(uint32)) return uint32();
		return *(uint32*)(pParentCB->GetBuffer()->GetBufferPointer() + m_StartOffset);
	}
	Float1 ShaderReflectionVariable::AsFloat1() const
	{
		if (m_Size < sizeof(Float1)) return Float1();
		return *(Float1*)(pParentCB->GetBuffer()->GetBufferPointer() + m_StartOffset);
	}
	Float2 ShaderReflectionVariable::AsFloat2() const
	{
		if (m_Size < sizeof(Float2)) return Float2();
		return *(Float2*)(pParentCB->GetBuffer()->GetBufferPointer() + m_StartOffset);
	}
	Float3 ShaderReflectionVariable::AsFloat3() const
	{
		if (m_Size < sizeof(Float3)) return Float3();
		return *(Float3*)(pParentCB->GetBuffer()->GetBufferPointer() + m_StartOffset);
	}
	Float4 ShaderReflectionVariable::AsFloat4() const
	{
		if (m_Size < sizeof(Float4)) return Float4();
		return *(Float4*)(pParentCB->GetBuffer()->GetBufferPointer() + m_StartOffset);
	}
	Float4x4 ShaderReflectionVariable::AsFloat4x4() const
	{
		if (m_Size < sizeof(Float4x4)) return Float4x4();
		return *(Float4x4*)(pParentCB->GetBuffer()->GetBufferPointer() + m_StartOffset);
	}
	Float4x3 ShaderReflectionVariable::AsFloat4x3() const
	{
		if (m_Size < sizeof(Float4x3)) return Float4x3();
		return *(Float4x3*)(pParentCB->GetBuffer()->GetBufferPointer() + m_StartOffset);
	}
	Float3x3 ShaderReflectionVariable::AsFloat3x3() const
	{
		if (m_Size < sizeof(Float3x3)) return Float3x3();
		return *(Float3x3*)(pParentCB->GetBuffer()->GetBufferPointer() + m_StartOffset);
	}

	void ShaderReflectionVariable::SetBool(bool value)
	{
		pParentCB->GetBuffer()->Update(m_StartOffset, std::min(sizeof(value), (size_t)m_Size), (byte*)&value);
	}
	void ShaderReflectionVariable::SetInt(int32 value)
	{
		pParentCB->GetBuffer()->Update(m_StartOffset, std::min(sizeof(value), (size_t)m_Size), (byte*)&value);
	}
	void ShaderReflectionVariable::SetUint(uint32 value)
	{
		pParentCB->GetBuffer()->Update(m_StartOffset, std::min(sizeof(value), (size_t)m_Size), (byte*)&value);
	}
	void ShaderReflectionVariable::SetFloat1(Float1 value)
	{
		pParentCB->GetBuffer()->Update(m_StartOffset, std::min(sizeof(value), (size_t)m_Size), (byte*)&value);
	}
	void ShaderReflectionVariable::SetFloat2(Float2 value)
	{
		pParentCB->GetBuffer()->Update(m_StartOffset, std::min(sizeof(value), (size_t)m_Size), (byte*)&value);
	}
	void ShaderReflectionVariable::SetFloat3(Float3 value)
	{
		pParentCB->GetBuffer()->Update(m_StartOffset, std::min(sizeof(value), (size_t)m_Size), (byte*)&value);
	}
	void ShaderReflectionVariable::SetFloat4(Float4 value)
	{
		pParentCB->GetBuffer()->Update(m_StartOffset, std::min(sizeof(value), (size_t)m_Size), (byte*)&value);
	}
	void ShaderReflectionVariable::SetFloat4x4(Float4x4 value)
	{
		pParentCB->GetBuffer()->Update(m_StartOffset, std::min(sizeof(value), (size_t)m_Size), (byte*)&value);
	}
	void ShaderReflectionVariable::SetFloat4x3(Float4x3 value)
	{
		pParentCB->GetBuffer()->Update(m_StartOffset, std::min(sizeof(value), (size_t)m_Size), (byte*)&value);
	}
	void ShaderReflectionVariable::SetFloat3x3(Float3x3 value)
	{
		pParentCB->GetBuffer()->Update(m_StartOffset, std::min(sizeof(value), (size_t)m_Size), (byte*)&value);
	}
	void ShaderReflectionVariable::SetStruct(byte const* pData, size_t lengthInBytes)
	{
		pParentCB->GetBuffer()->Update(m_StartOffset, std::min(lengthInBytes, m_Size), (byte*)pData);
	}

	//
	// ShaderReflectionBindResource Implement
	//

	ShaderReflectionBindResource::ShaderReflectionBindResource(
		ShaderReflection* pReflection)
		:m_pReflection(pReflection)
	{}
	
	ShaderReflectionBindResource::~ShaderReflectionBindResource()
	{}
		
	void SetSampler()
	{}
	ShaderResource* ShaderReflectionBindResource::GetShaderResource() const
	{
		return m_pResourceView;
	}
	void ShaderReflectionBindResource::UnSetShaderResource()
	{
		m_pResourceView = nullptr;
	}
}