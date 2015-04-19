#include "Log.h"
#include "Utility.hpp"

#include "DeviceBuffer.hpp"
#include "ShaderReflection.hpp"
#include "Shader.hpp"
#include "D3D11Device.hpp"
#include "D3D11ShaderReflection.hpp"

namespace Space
{

	ShaderReflection::ShaderReflection(VertexShader* pShader)
		:m_pPS(nullptr), m_pVS(pShader)
	{
	}
	ShaderReflection::ShaderReflection(PixelShader* pShader)
		: m_pPS(pShader), m_pVS(nullptr)
	{
	}

	ShaderReflection::~ShaderReflection()
	{}

	uint32 ShaderReflection::GetConstantBufferCount() const
	{
		return m_ConstBuffers.size();
	}
	uint32 ShaderReflection::GetBindResourceCount() const
	{
		return m_Resources.size();
	}
	uint32 ShaderReflection::GetInstructionCount() const
	{
		return m_InstructionCount;
	}
	ShaderReflection* ShaderReflection::CreateD3D(
		VertexShader* pShader)
	{
		TRY_CATCH_LOG(return new D3D11ShaderReflection(pShader), return nullptr);
	}
	ShaderReflection* ShaderReflection::CreateD3D(
		PixelShader* pShader)
	{
		TRY_CATCH_LOG(return new D3D11ShaderReflection(pShader), return nullptr);
	}
	
	//
	// ShaderReflectionConstantBuffer Implement
	//

	ShaderReflectionConstantBuffer::ShaderReflectionConstantBuffer(
		ShaderReflection* pReflection)
		:m_pReflection(pReflection){
	}

	ShaderReflectionConstantBuffer::~ShaderReflectionConstantBuffer()
	{}
	
	ShaderReflectionVariable* ShaderReflectionConstantBuffer::GetVariableByIndex(int32 index) const
	{
		TRY_CATCH_LOG(return m_Variables.at(index).get(), return nullptr);
	}

	ShaderReflection* ShaderReflectionConstantBuffer::GetShaderReflection()
	{
		return m_pReflection;
	}
	uint32 ShaderReflectionConstantBuffer::GetSizeInBytes() const
	{
		return m_Size;
	}
	uint32 ShaderReflectionConstantBuffer::GetVariableCount() const
	{
		return m_Variables.size();
	}
	Name ShaderReflectionConstantBuffer::GetName() const
	{
		return m_Name;
	}

	void ShaderReflectionConstantBuffer::SetBuffer(ConstantBuffer* pBuffer)
	{
		if (pBuffer != nullptr)
			m_pBuffer = pBuffer;
	}

	void ShaderReflectionConstantBuffer::UnSetBuffer()
	{
		m_pBuffer = nullptr;
		/*for (auto iter = m_Variables.begin(); iter != m_Variables.end(); ++iter)
		{
			(*iter)->UnSetBuffer();
		}*/
	}

	ConstantBuffer* ShaderReflectionConstantBuffer::GetBuffer() const
	{
		return m_pBuffer;
	}
	
	//
	// ShaderReflectionVariable Implement
	//

	ShaderReflectionVariable::ShaderReflectionVariable(
		ShaderReflectionConstantBuffer *pCB)
		:m_pParentCB(pCB), m_pParentVariable(nullptr)
	{}

	ShaderReflectionVariable::ShaderReflectionVariable(
		ShaderReflectionVariable* pVariable)
		: m_pParentVariable(pVariable), m_pParentCB(nullptr)
	{}

	ShaderReflectionVariable::~ShaderReflectionVariable()
	{}

	ShaderReflectionConstantBuffer* ShaderReflectionVariable::GetParentConstantBuffer() const
	{
		return m_pParentCB;
	}
	ShaderReflectionVariable* ShaderReflectionVariable::GetParentVariable() const
	{
		return m_pParentVariable;
	}	 
	uint32 ShaderReflectionVariable::GetStartOffset() const
	{
		return m_StartOffset;
	}
	uint32 ShaderReflectionVariable::GetMemberCount() const
	{
		return m_Members.size();
	}
	ShaderVariableType ShaderReflectionVariable::GetType() const
	{
		return m_Type;
	}
	ShaderVariableClass ShaderReflectionVariable::GetClass() const
	{
		return m_Class;
	}
	uint32 ShaderReflectionVariable::GetColumnsCount() const
	{
		return m_Columns;
	}
	uint32 ShaderReflectionVariable::GetRowsCount() const
	{
		return m_Rows;
	}
	Name ShaderReflectionVariable::GetName() const
	{
		return m_Name;
	}

	std::string ShaderReflectionVariable::AsString() const
	{
		return std::string(
			m_pParentCB->GetBuffer()->GetBufferPointer() + m_StartOffset,
			m_pParentCB->GetBuffer()->GetBufferPointer() + m_StartOffset + m_Size
			);
	}
	bool ShaderReflectionVariable::AsBool() const
	{
		if (m_Size < sizeof(bool)) return bool();
		return *(bool*)(m_pParentCB->GetBuffer()->GetBufferPointer() + m_StartOffset);
	}
	int32 ShaderReflectionVariable::AsInt() const
	{
		if (m_Size < sizeof(int32)) return int32();
		return *(int32*)(m_pParentCB->GetBuffer()->GetBufferPointer() + m_StartOffset);
	}
	uint32 ShaderReflectionVariable::AsUint() const
	{
		if (m_Size < sizeof(uint32)) return uint32();
		return *(uint32*)(m_pParentCB->GetBuffer()->GetBufferPointer() + m_StartOffset);
	}
	Float1 ShaderReflectionVariable::AsFloat1() const
	{
		if (m_Size < sizeof(Float1)) return Float1();
		return *(Float1*)(m_pParentCB->GetBuffer()->GetBufferPointer() + m_StartOffset);
	}
	Float2 ShaderReflectionVariable::AsFloat2() const
	{
		if (m_Size < sizeof(Float2)) return Float2();
		return *(Float2*)(m_pParentCB->GetBuffer()->GetBufferPointer() + m_StartOffset);
	}
	Float3 ShaderReflectionVariable::AsFloat3() const
	{
		if (m_Size < sizeof(Float3)) return Float3();
		return *(Float3*)(m_pParentCB->GetBuffer()->GetBufferPointer() + m_StartOffset);
	}
	Float4 ShaderReflectionVariable::AsFloat4() const
	{
		if (m_Size < sizeof(Float4)) return Float4();
		return *(Float4*)(m_pParentCB->GetBuffer()->GetBufferPointer() + m_StartOffset);
	}
	Float4x4 ShaderReflectionVariable::AsFloat4x4() const
	{
		if (m_Size < sizeof(Float4x4)) return Float4x4();
		return *(Float4x4*)(m_pParentCB->GetBuffer()->GetBufferPointer() + m_StartOffset);
	}
	Float4x3 ShaderReflectionVariable::AsFloat4x3() const
	{
		if (m_Size < sizeof(Float4x3)) return Float4x3();
		return *(Float4x3*)(m_pParentCB->GetBuffer()->GetBufferPointer() + m_StartOffset);
	}
	Float3x3 ShaderReflectionVariable::AsFloat3x3() const
	{
		if (m_Size < sizeof(Float3x3)) return Float3x3();
		return *(Float3x3*)(m_pParentCB->GetBuffer()->GetBufferPointer() + m_StartOffset);
	}

	void ShaderReflectionVariable::SetBool(bool value)
	{
		m_pParentCB->GetBuffer()->Update(m_StartOffset, std::min(sizeof(value), m_Size), (byte*)&value);
	}
	void ShaderReflectionVariable::SetInt(int32 value)
	{
		m_pParentCB->GetBuffer()->Update(m_StartOffset, std::min(sizeof(value), m_Size), (byte*)&value);
	}
	void ShaderReflectionVariable::SetUint(uint32 value)
	{
		m_pParentCB->GetBuffer()->Update(m_StartOffset, std::min(sizeof(value), m_Size), (byte*)&value);
	}
	void ShaderReflectionVariable::SetFloat1(Float1 value)
	{
		m_pParentCB->GetBuffer()->Update(m_StartOffset, std::min(sizeof(value), m_Size), (byte*)&value);
	}
	void ShaderReflectionVariable::SetFloat2(Float2 value)
	{
		m_pParentCB->GetBuffer()->Update(m_StartOffset, std::min(sizeof(value), m_Size), (byte*)&value);
	}
	void ShaderReflectionVariable::SetFloat3(Float3 value)
	{
		m_pParentCB->GetBuffer()->Update(m_StartOffset, std::min(sizeof(value), m_Size), (byte*)&value);
	}
	void ShaderReflectionVariable::SetFloat4(Float4 value)
	{
		m_pParentCB->GetBuffer()->Update(m_StartOffset, std::min(sizeof(value), m_Size), (byte*)&value);
	}
	void ShaderReflectionVariable::SetFloat4x4(Float4x4 value)
	{
		m_pParentCB->GetBuffer()->Update(m_StartOffset, std::min(sizeof(value), m_Size), (byte*)&value);
	}
	void ShaderReflectionVariable::SetFloat4x3(Float4x3 value)
	{
		m_pParentCB->GetBuffer()->Update(m_StartOffset, std::min(sizeof(value), m_Size), (byte*)&value);
	}
	void ShaderReflectionVariable::SetFloat3x3(Float3x3 value)
	{
		m_pParentCB->GetBuffer()->Update(m_StartOffset, std::min(sizeof(value),m_Size), (byte*)&value);
	}
	void ShaderReflectionVariable::SetStruct(byte const* pData, uint32 lengthInBytes)
	{
		m_pParentCB->GetBuffer()->Update(m_StartOffset, std::min(lengthInBytes,m_Size), (byte*)pData);
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

	ShaderReflection* ShaderReflectionBindResource::GetShaderReflection() const
	{
		return m_pReflection;
	}

	Name ShaderReflectionBindResource::GetName() const
	{
		return m_Name;
	}
	ShaderInputType ShaderReflectionBindResource::GetType() const
	{
		return m_Type;
	}
	uint32 ShaderReflectionBindResource::GetBindPoint() const
	{
		return m_BindPoint;
	}
	uint32 ShaderReflectionBindResource::GetBindCount() const
	{
		return m_BindCount;
	}
	ResourceReturnType ShaderReflectionBindResource::GetReturnType() const
	{
		return m_ReturnType;
	}
	ResourceDimension ShaderReflectionBindResource::GetDimension() const
	{
		return m_Dimension;
	}
	void ShaderReflectionBindResource::SetShaderResource(ShaderResource* pResource)
	{
		if (pResource != nullptr)
			m_pResourceView = pResource;
	}
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