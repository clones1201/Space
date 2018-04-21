#pragma once

#include "Prerequisites.hpp"
#include "Shared.hpp"

#include "Shader.hpp"

namespace Space
{
	namespace Render {
		
		namespace Details
		{
			template<class Impl>
			class SPACE_RENDERSYSTEM_API ShaderReflectionImpl :
				public SharedPtrObject<Impl>,
				public PImpl<Impl>
			{
			public:
				virtual ~ShaderReflectionImpl(){}

				inline size_t GetConstantBuffersCount() const
				{
					return _impl->GetConstantBuffersCount();
				}
				inline size_t GetBindResourceCount() const
				{
					return _impl->GetBindResourceCount();
				}
				inline size_t GetInstructionCount() const
				{
					return _impl->GetInstructionCount();
				}

				//static ShaderReflection* CreateD3D(VertexShader* pShader);
				//static ShaderReflection* CreateD3D(PixelShader* pShader);

				ShaderReflectionConstantBuffer* GetConstantBufferByIndex(int32 index) const;
				ShaderReflectionBindResource* GetBindResourceByIndex(int32 index) const;

			protected:
				ShaderReflectionImpl(ShaderBase* pShader)
					:pShader(pShader)
				{}

				ShaderBase const* const pShader;

				std::vector<TypeTrait<ShaderReflectionConstantBuffer>::Ptr> _ConstBuffers;
				std::vector<TypeTrait<ShaderReflectionBindResource>::Ptr> _Resources;

				uint32 _InstructionCount = 0;
			};

			class SPACE_RENDERSYSTEM_API ShaderReflectionConstantBuffer : private Uncopyable
			{
			public:
				virtual ~ShaderReflectionConstantBuffer();

				ShaderReflectionVariable* GetVariableByIndex(int32 index) const;

				inline size_t GetSizeInBytes() const
				{
					return _Size;
				}
				inline size_t GetVariableCount() const
				{
					return _Variables.size();
				}
				inline Name GetName() const
				{
					return _Name;
				}

				inline ShaderReflection* GetShaderReflection()
				{
					return pReflection;
				}

				void SetBuffer(ConstantBuffer* pBuffer)
				{
					if (pBuffer != nullptr)
						this->pBuffer = pBuffer;
				}
				void UnSetBuffer()
				{
					pBuffer = nullptr;
					/*for (auto iter = _Variables.begin(); iter != _Variables.end(); ++iter)
					{
					(*iter)->UnSetBuffer();
					}*/
				}
				inline ConstantBuffer* GetBuffer() const
				{
					return pBuffer;
				}
			protected:
				ShaderReflectionConstantBuffer(
					ShaderReflection* pReflection);

				ShaderReflection* const pReflection;
				ConstantBuffer* pBuffer;
				Name _Name;
				std::vector<TypeTrait<ShaderReflectionVariable>::Ptr> _Variables;

				uint32 _Size;
				ShaderInputType _Type;
			};

			template<class Impl>			
			class SPACE_RENDERSYSTEM_API ShaderReflectionVariableImpl : 
				public SharedPtrObject<Impl>,
				public PImpl<Impl>
			{
			public:
				virtual ~ShaderReflectionVariableImpl(){}

				inline ShaderReflectionConstantBuffer* GetParentConstantBuffer() const
				{
					return pParentCB;
				}
				inline ShaderReflectionVariable* GetParentVariable() const
				{
					return pParentVariable;
				}
				inline size_t GetStartOffset() const
				{
					return _StartOffset;
				}
				size_t GetSizeInBytes() const
				{
					return _Size;
				}
				inline size_t GetMemberCount() const
				{
					return _Members.size();
				}
				/* bool, float, sampler, etc.*/
				inline ShaderVariableType GetType() const
				{
					return _Type;
				}
				/* matrix, vector, struct, etc. */
				inline ShaderVariableClass GetClass() const
				{
					return _Class;
				}

				/* for matrices, 1 for other numeric, 0 if not applicable */
				inline size_t GetColumnsCount() const
				{
					return _Columns;
				}
				/* for vectors & matrices, 1 for other numeric, 0 if not applicable */
				inline size_t GetRowsCount() const
				{
					return _Rows;
				}
				inline Name GetName() const
				{
					return _Name;
				}

				template<class Type>
				typename Type As() const
				{
					if (_Size < sizeof(Type)) return Type();
					return *(Type*)(pParentCB->GetBuffer());
				}

				template<>
				typename std::string ShaderReflectionVariableImpl::As<typename std::string>() const
				{
					return std::string(
						pParentCB->GetBuffer()->GetBufferPointer() + _StartOffset,
						pParentCB->GetBuffer()->GetBufferPointer() + _StartOffset + _Size
					);
				}
				template<class Type>
				void Set(Type val) const
				{
					if (_Size < sizeof(Type)) return Type();
					pParentCB->GetBuffer()->Update(_StartOffset, std::min(sizeof(value), (size_t)_Size), (byte*)&val);
				}
				
				void SetStruct(byte const* pData, size_t lengthInBytes);

			protected:
				ShaderReflectionVariable(ShaderReflectionConstantBuffer* pCB);
				ShaderReflectionVariable(ShaderReflectionVariable *pVariable);

				//ShaderReflectionConstantBuffer* const pParentCB;
				//ShaderReflectionVariable* const pParentVariable;
				std::vector<TypeTrait<ShaderReflectionVariable>::Ptr> _Members;
				Name _Name;
				size_t _StartOffset;
				size_t _Size = 0;
				ShaderVariableClass _Class;
				ShaderVariableType _Type;
				size_t _Columns;
				size_t _Rows;
			};

			class SPACE_RENDERSYSTEM_API ShaderReflectionBindResource : private Uncopyable
			{
			public:
				virtual ~ShaderReflectionBindResource();

				inline ShaderReflection* GetShaderReflection() const
				{
					return _pReflection;
				}
				inline Name GetName() const
				{
					return _Name;
				}
				inline ShaderInputType GetType() const
				{
					return _Type;
				}
				inline uint32 GetBindPoint() const
				{
					return _BindPoint;
				}
				inline uint32 GetBindCount() const
				{
					return _BindCount;
				}
				inline ResourceReturnType GetReturnType() const
				{
					return _ReturnType;
				}
				inline ResourceDimension GetDimension() const
				{
					return _Dimension;
				}

				void SetShaderResource(ShaderResource* pResource);
				void SetSampler();

				ShaderResource* GetShaderResource() const;
				void UnSetShaderResource();

			protected:
				ShaderReflectionBindResource(ShaderReflection* pReflection);

				ShaderReflection* const _pReflection;
				ShaderResource* _pResourceView = nullptr;
				//Sampler const* _pSamplerView = nullptr;

				Name _Name;
				ShaderInputType				_Type;           // Type of resource (e.g. texture, cbuffer, etc.)
				uint32                      _BindPoint;      // Starting bind point
				uint32                      _BindCount;      // Number of contiguous bind points (for arrays)

				//UINT                        uFlags;         // Input binding flags
				ResourceReturnType			_ReturnType;     // Return type (if texture)
				ResourceDimension           _Dimension;      // Dimension (if texture)

			};
		}

	}
}