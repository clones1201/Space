#ifndef __SPACE_RENDERSYSTEM_SHADERREFLECTION_HPP__
#define __SPACE_RENDERSYSTEM_SHADERREFLECTION_HPP__

#include "RenderSystem/Prerequisites.hpp"
#include "RenderSystem/Shared.hpp"

namespace Space
{
	class ShaderBase;
	class ShaderReflectionConstantBuffer;
	class ShaderReflectionBindResource;
	class ShaderReflectionVariable;

	class SPACE_RENDERSYSTEM_API ShaderReflection : private Uncopyable
	{
	public:
		virtual ~ShaderReflection();

		inline size_t GetConstantBufferCount() const
		{
			return m_ConstBuffers.size();
		}
		inline size_t GetBindResourceCount() const
		{
			return m_Resources.size();
		}
		inline size_t GetInstructionCount() const
		{
			return m_InstructionCount;
		}

		//static ShaderReflection* CreateD3D(VertexShader* pShader);
		//static ShaderReflection* CreateD3D(PixelShader* pShader);

		ShaderReflectionConstantBuffer* GetConstantBufferByIndex(int32 index) const;
		ShaderReflectionBindResource* GetBindResourceByIndex(int32 index) const;

	protected:
		ShaderReflection(ShaderBase* pShader);

		//VertexShader const* const pVS;
		//PixelShader const* const pPS;
		ShaderBase const* const pShader;

		std::vector<TypeTrait<ShaderReflectionConstantBuffer>::Ptr> m_ConstBuffers;
		std::vector<TypeTrait<ShaderReflectionBindResource>::Ptr> m_Resources;

		uint32 m_InstructionCount = 0;
	};

	enum class SPACE_RENDERSYSTEM_API ShaderInputType : uint8
	{
		SIT_CBuffer = 0,
		SIT_TBuffer = (SIT_CBuffer + 1),
		SIT_Texture = (SIT_TBuffer + 1),
		SIT_Sampler = (SIT_Texture + 1)
	};

	class SPACE_RENDERSYSTEM_API ShaderReflectionConstantBuffer : private Uncopyable
	{
	public:
		virtual ~ShaderReflectionConstantBuffer();

		ShaderReflectionVariable* GetVariableByIndex(int32 index) const;

		inline size_t GetSizeInBytes() const
		{
			return m_Size;
		}
		inline size_t GetVariableCount() const
		{
			return m_Variables.size();
		}
		inline Name GetName() const
		{
			return m_Name;
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
			/*for (auto iter = m_Variables.begin(); iter != m_Variables.end(); ++iter)
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
		Name m_Name;
		std::vector<TypeTrait<ShaderReflectionVariable>::Ptr> m_Variables;

		uint32 m_Size;
		ShaderInputType m_Type;
	};

	enum class SPACE_RENDERSYSTEM_API ShaderVariableClass : uint8
	{
		SVC_Scalar = 0,
		SVC_Vector = (SVC_Scalar + 1),
		SVC_Matrix_Rows = (SVC_Vector + 1),
		SVC_Matrix_Columns = (SVC_Matrix_Rows + 1),
		SVC_Object = (SVC_Matrix_Columns + 1),
		SVC_Struct = (SVC_Object + 1),
	};

	enum class SPACE_RENDERSYSTEM_API ShaderVariableType : uint8
	{
		SVT_Void = 0,
		SVT_Bool,
		SVT_Int,
		SVT_Float,
		SVT_String,
		SVT_Texture,
		SVT_Texture1D,
		SVT_Texture2D,
		SVT_Texture3D,
		//SVT_TextureCube,
		SVT_Sampler,
		SVT_Sampler1D,
		SVT_Sampler2D,
		SVT_Sampler3D,
		//SVT_SamplerCube,
		SVT_Uint,
		SVT_Uint8,
		//SVT_DepthStencil = 23,
		SVT_Blend,
		SVT_Buffer,
		SVT_CBuffer,
		SVT_TBuffer,
		SVT_Texture1DArray,
		SVT_Texture2DArray,
		//SVT_RenderTargetView = 30,
		//SVT_DepthStencilView = 31,
		//SVT_Texture2DMS,
		//SVT_Texture2DMSArray,
		//SVT_TextureCubeArray,
		SVT_Double,
		//SVT_RWTexture1D = 40,
		//SVT_RWTexture1Darray = 41,
		//SVT_RWTexture2D = 42,
		//SVT_RWTexture2Darray = 43,
		//SVT_RWTexture3D = 44,
		//SVT_RWBuffer = 45,
		//SVT_ByteAddress_Buffer = 46,
		//SVT_RWByteAddress_Buffer = 47,
		//SVT_Structured_Buffer = 48,
		//SVT_RWStructured_Buffer = 49,
		//SVT_Append_Structured_Buffer = 50,
		//SVT_Consume_Structured_Buffer = 51,
		//SVT_Min8Float = 52,
		//SVT_Min10Float = 53,
		//SVT_Min16Float = 54,
		//SVT_Min12Int = 55,
		//SVT_Min16Int = 56,
		//SVT_Min16Uint = 57,
	};

	class SPACE_RENDERSYSTEM_API ShaderReflectionVariable : private Uncopyable
	{
	public:
		virtual ~ShaderReflectionVariable();

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
			return m_StartOffset;
		}
		size_t GetSizeInBytes() const
		{
			return m_Size;
		}
		inline size_t GetMemberCount() const
		{
			return m_Members.size();
		}
		/* bool, float, sampler, etc.*/
		inline ShaderVariableType GetType() const
		{
			return m_Type;
		}
		/* matrix, vector, struct, etc. */
		inline ShaderVariableClass GetClass() const
		{
			return m_Class;
		}

		/* for matrices, 1 for other numeric, 0 if not applicable */
		inline size_t GetColumnsCount() const
		{
			return m_Columns;
		}
		/* for vectors & matrices, 1 for other numeric, 0 if not applicable */
		inline size_t GetRowsCount() const
		{
			return m_Rows;
		}
		inline Name GetName() const
		{
			return m_Name;
		}

		std::string AsString() const;
		bool AsBool() const;
		int32 AsInt() const;
		uint32 AsUint() const;
		Float1 AsFloat1() const;
		Float2 AsFloat2() const;
		Float3 AsFloat3() const;
		Float4 AsFloat4() const;
		Float4x4 AsFloat4x4() const;
		Float4x3 AsFloat4x3() const;
		Float3x3 AsFloat3x3() const;

		void SetBool(bool value);
		void SetInt(int32 value);
		void SetUint(uint32 value);
		void SetFloat1(Float1 value);
		void SetFloat2(Float2 value);
		void SetFloat3(Float3 value);
		void SetFloat4(Float4 value);
		void SetFloat4x4(Float4x4 value);
		void SetFloat4x3(Float4x3 value);
		void SetFloat3x3(Float3x3 value);
		void SetStruct(byte const* pData, size_t lengthInBytes);

	protected:
		ShaderReflectionVariable(ShaderReflectionConstantBuffer* pCB);
		ShaderReflectionVariable(ShaderReflectionVariable *pVariable);
				
		ShaderReflectionConstantBuffer* const pParentCB;
		ShaderReflectionVariable* const pParentVariable;
		std::vector<TypeTrait<ShaderReflectionVariable>::Ptr> m_Members;
		Name m_Name;
		size_t m_StartOffset;
		size_t m_Size = 0;
		ShaderVariableClass m_Class;
		ShaderVariableType m_Type;
		size_t m_Columns;
		size_t m_Rows;
	};

	enum class SPACE_RENDERSYSTEM_API ResourceReturnType : uint8
	{
		RRT_Unknown = 0,
		RRT_Unorm = 1,
		RRT_Snorm = 2,
		RRT_Sint = 3,
		RRT_Uint = 4,
		RRT_Float = 5,
		RRT_Mixed = 6,
		RRT_Double = 7,
	};

	enum class SPACE_RENDERSYSTEM_API ResourceDimension : uint8
	{
		RD_Unknown = 0,
		RD_Buffer = 1,
		RD_Texture1D = 2,
		RD_Texture1DArray = 3,
		RD_Texture2D = 4,
		RD_Texture2DArray = 5,
		RD_Texture2DMS = 6,
		RD_Texture2DMSArray = 7,
		RD_Texture3D = 8,
		RD_TextureCube = 9,
		RD_TextureCubeArray = 10,
		RD_BufferEx = 11,
	};

	class SPACE_RENDERSYSTEM_API ShaderReflectionBindResource : private Uncopyable
	{
	public:
		virtual ~ShaderReflectionBindResource();

		inline ShaderReflection* GetShaderReflection() const
		{
			return m_pReflection;
		}
		inline Name GetName() const
		{
			return m_Name;
		}
		inline ShaderInputType GetType() const
		{
			return m_Type;
		}
		inline uint32 GetBindPoint() const
		{
			return m_BindPoint;
		}
		inline uint32 GetBindCount() const
		{
			return m_BindCount;
		}
		inline ResourceReturnType GetReturnType() const
		{
			return m_ReturnType;
		}
		inline ResourceDimension GetDimension() const
		{
			return m_Dimension;
		}

		void SetShaderResource(ShaderResource* pResource);
		void SetSampler();
		
		ShaderResource* GetShaderResource() const;
		void UnSetShaderResource();

	protected:
		ShaderReflectionBindResource(ShaderReflection* pReflection);

		ShaderReflection* const m_pReflection;
		ShaderResource* m_pResourceView = nullptr;
		//Sampler const* m_pSamplerView = nullptr;

		Name m_Name;
		ShaderInputType				m_Type;           // Type of resource (e.g. texture, cbuffer, etc.)
		uint32                      m_BindPoint;      // Starting bind point
		uint32                      m_BindCount;      // Number of contiguous bind points (for arrays)

		//UINT                        uFlags;         // Input binding flags
		ResourceReturnType			m_ReturnType;     // Return type (if texture)
		ResourceDimension           m_Dimension;      // Dimension (if texture)

	};

}

#endif