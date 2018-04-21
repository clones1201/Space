#include "Common/Log.h"
#include "Common/Utility.hpp"

#include <d3d11shader.h>
#include "D3D11RenderSystem/D3D11Shader.hpp"
#include "D3D11RenderSystem/D3D11ShaderReflection.hpp"

namespace Space
{
	namespace Render
	{
		ShaderVariableClass GetSVCFromD3DSVC(D3D_SHADER_VARIABLE_CLASS svc)
		{
			return (ShaderVariableClass)svc;
		}
		ShaderVariableType GetSVTFromD3DSVT(D3D_SHADER_VARIABLE_TYPE svt)
		{
			struct Table
			{
				typedef std::map<D3D_SHADER_VARIABLE_TYPE, ShaderVariableType> TableType;

				TableType _table;
				Table()
				{
#define ADD_ITEM(key,value) _table[key] = ShaderVariableType::value
					ADD_ITEM(D3D_SVT_VOID, SVT_Void);
					ADD_ITEM(D3D_SVT_BOOL, SVT_Bool);
					ADD_ITEM(D3D_SVT_INT, SVT_Int);
					ADD_ITEM(D3D_SVT_FLOAT, SVT_Float);
					ADD_ITEM(D3D_SVT_STRING, SVT_String);
					ADD_ITEM(D3D_SVT_TEXTURE, SVT_Texture);
					ADD_ITEM(D3D_SVT_TEXTURE1D, SVT_Texture1D);
					ADD_ITEM(D3D_SVT_TEXTURE2D, SVT_Texture2D);
					ADD_ITEM(D3D_SVT_TEXTURE3D, SVT_Texture3D);
					ADD_ITEM(D3D_SVT_SAMPLER, SVT_Sampler);
					ADD_ITEM(D3D_SVT_SAMPLER1D, SVT_Sampler1D);
					ADD_ITEM(D3D_SVT_SAMPLER2D, SVT_Sampler2D);
					ADD_ITEM(D3D_SVT_SAMPLER3D, SVT_Sampler3D);
					ADD_ITEM(D3D_SVT_UINT, SVT_Uint);
					ADD_ITEM(D3D_SVT_UINT8, SVT_Uint8);
					ADD_ITEM(D3D_SVT_BLEND, SVT_Blend);
					ADD_ITEM(D3D_SVT_BUFFER, SVT_Buffer);
					ADD_ITEM(D3D_SVT_CBUFFER, SVT_CBuffer);
					ADD_ITEM(D3D_SVT_TBUFFER, SVT_TBuffer);
					ADD_ITEM(D3D_SVT_TEXTURE1DARRAY, SVT_Texture1DArray);
					ADD_ITEM(D3D_SVT_TEXTURE2DARRAY, SVT_Texture2DArray);
					ADD_ITEM(D3D_SVT_DOUBLE, SVT_Double);
#undef ADD_ITEM
				}

				TableType::mapped_type operator[] (TableType::key_type key)
				{
					TRY_CATCH_OUT_OF_RANGE(return _table.at(key), return ShaderVariableType::SVT_Void);
				}
			};
			static Table table;
			return table[svt];
		}

		uint32 D3D11ShaderReflection::GetInstructionCount() const
		{
			return _Desc.InstructionCount;
		}		
		uint32 D3D11ShaderReflection::GetConstantBufferCount() const
		{
			return _Desc.ConstantBuffers;
		}
		uint32 D3D11ShaderReflection::GetShaderResourceCount() const
		{
			return _Desc.BoundResources - _Desc.ConstantBuffers;
		}

		D3D11ShaderReflection::D3D11ShaderReflection(
			byte const* byteCodes, uint32 lengthInBytes)
		{
			ID3D11ShaderReflection* pReflection = nullptr;
			HRESULT hr = D3DReflect(byteCodes, lengthInBytes,
				__uuidof(ID3D11ShaderReflection), (void**)&pReflection);
			if (FAILED(hr))
			{
				throw std::exception("D3DReflect failed");
			}
			m_pReflection = pReflection;

			m_pReflection->GetDesc(&_Desc);
		}

		D3D11ShaderReflectionConstantBuffer::D3D11ShaderReflectionConstantBuffer(
			ID3D11ShaderReflectionConstantBuffer* pCBReflection,
			D3D11ShaderReflection* pShaderReflection)
		{
			D3D11_SHADER_BUFFER_DESC desc;
			pCBReflection->GetDesc(&desc);

			/*m_Name = Name(desc.Name);
			m_Size = desc.Size;*/

			/*m_Variables.reserve(desc.Variables);
			for (uint32 i = 0; i < desc.Variables; ++i)
			{
				auto pVariable = pCBReflection->GetVariableByIndex(i);
				m_Variables.push_back(
					TypeTrait<ShaderReflectionVariable>::Ptr(
						new D3D11ShaderReflectionVariable(pVariable, this)
					));
			}*/
		}

		D3D11ShaderReflectionVariable::D3D11ShaderReflectionVariable(
			ID3D11ShaderReflectionVariable* pIVariable,
			D3D11ShaderReflectionConstantBuffer* pParent)
		{
			auto pType = pIVariable->GetType();
			pIVariable->GetDesc(&m_Desc);
			//m_Size = m_Desc.Size;

			_Initialize(pType);
		}

		D3D11ShaderReflectionVariable::D3D11ShaderReflectionVariable(
			ID3D11ShaderReflectionType* pType,
			D3D11ShaderReflectionVariable* pParent)
		{
			_Initialize(pType);
		}

		void D3D11ShaderReflectionVariable::_Initialize(
			ID3D11ShaderReflectionType* pType)
		{
			pType->GetDesc(&m_TypeDesc);
			/*m_Name = Name(m_TypeDesc.Name);

			m_Class = GetSVCFromD3DSVC(m_TypeDesc.Class);
			m_Type = GetSVTFromD3DSVT(m_TypeDesc.Type);
			m_StartOffset = m_TypeDesc.Offset;
			m_Columns = m_TypeDesc.Columns;
			m_Rows = m_TypeDesc.Rows;*/

			/*if (m_Class == ShaderVariableClass::SVC_Struct)
			{
				m_Members.reserve(m_TypeDesc.Members);
				for (uint32 i = 0; i < m_TypeDesc.Members; ++i)
				{
					auto pMember = pType->GetMemberTypeByIndex(i);
					m_Members.push_back(
						TypeTrait<ShaderReflectionVariable>::Ptr(
							new D3D11ShaderReflectionVariable(pMember, this)
						));
				}
			}
			else if (m_Class == ShaderVariableClass::SVC_Object)
			{
				Log("D3D11ShaderReflectionVariable: not supported variable class\n");
			}*/
		}

		D3D11ShaderReflectionBindResource::D3D11ShaderReflectionBindResource(
			D3D11_SHADER_INPUT_BIND_DESC desc,
			D3D11ShaderReflection* pParent)
		{
			/*m_Name = Name(desc.Name);
			m_Type = (ShaderInputType)desc.Type;
			m_BindPoint = desc.BindPoint;
			m_BindCount = desc.BindCount;
			m_ReturnType = (ResourceReturnType)desc.ReturnType;
			m_Dimension = (ResourceDimension)desc.Dimension;*/
		}

	}
}