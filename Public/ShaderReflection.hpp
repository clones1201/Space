#ifndef __SPACE_SHADERREFLECTION_HPP__
#define __SPACE_SHADERREFLECTION_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"
#include "Utility.hpp"

namespace Space
{
	class ShaderReflectionConstantBuffer;
	class ShaderReflectionBindResource;
	class ShaderReflectionVariable;

	class ShaderReflection : virtual public Uncopyable
	{
	public:
		virtual ~ShaderReflection();

		uint32 GetConstantBufferCount() const;
		uint32 GetBindResourceCount() const;
		uint32 GetInstructionCount() const;

		static ShaderReflection* CreateD3D(VertexShader* pShader);
		static ShaderReflection* CreateD3D(PixelShader* pShader);

		ShaderReflectionConstantBuffer* GetConstantBufferByIndex(int32 index) const;
		ShaderReflectionBindResource* GetBindResourceByIndex(int32 index) const;

	protected:
		ShaderReflection(VertexShader* pShader);
		ShaderReflection(PixelShader* pShader);

		VertexShader const* const pVS;
		PixelShader const* const pPS;

		std::vector<TypeTrait<ShaderReflectionConstantBuffer>::Ptr> m_ConstBuffers;
		std::vector<TypeTrait<ShaderReflectionBindResource>::Ptr> m_Resources;

		uint32 m_InstructionCount = 0;
	};

	typedef enum _ShaderInputType
	{
		SIT_CBuffer = 0,
		SIT_TBuffer = (SIT_CBuffer + 1),
		SIT_Texture = (SIT_TBuffer + 1),
		SIT_Sampler = (SIT_Texture + 1)
	}ShaderInputType;

	class ShaderReflectionConstantBuffer : virtual public Uncopyable
	{
	public:
		virtual ~ShaderReflectionConstantBuffer();

		ShaderReflectionVariable* GetVariableByIndex(int32 index) const;
		uint32 GetSizeInBytes() const;
		uint32 GetVariableCount() const;
		Name GetName() const;

		ShaderReflection* GetShaderReflection();

		void SetBuffer(ConstantBuffer* pBuffer);
		void UnSetBuffer();
		ConstantBuffer* GetBuffer() const;
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

	typedef enum _ShaderVariableClass
	{
		SVC_Scalar = 0,
		SVC_Vector = (SVC_Scalar + 1),
		SVC_Matrix_Rows = (SVC_Vector + 1),
		SVC_Matrix_Columns = (SVC_Matrix_Rows + 1),
		SVC_Object = (SVC_Matrix_Columns + 1),
		SVC_Struct = (SVC_Object + 1),
	}ShaderVariableClass;

	typedef enum _ShaderVariableType
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
	}ShaderVariableType;

	class ShaderReflectionVariable : virtual public Uncopyable
	{
	public:
		virtual ~ShaderReflectionVariable();

		ShaderReflectionConstantBuffer* GetParentConstantBuffer() const;
		ShaderReflectionVariable* GetParentVariable() const;
		uint32 GetStartOffset() const;
		uint32 GetSizeInBytes() const;
		uint32 GetMemberCount() const;
		/* bool, float, sampler, etc.*/
		ShaderVariableType GetType() const;
		/* matrix, vector, struct, etc. */
		ShaderVariableClass GetClass() const;

		/* for matrices, 1 for other numeric, 0 if not applicable */
		uint32 GetColumnsCount() const;
		/* for vectors & matrices, 1 for other numeric, 0 if not applicable */
		uint32 GetRowsCount() const;

		Name GetName() const;

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
		void SetStruct(byte const* pData, uint32 lengthInBytes);

	protected:
		ShaderReflectionVariable(ShaderReflectionConstantBuffer* pCB);
		ShaderReflectionVariable(ShaderReflectionVariable *pVariable);
				
		ShaderReflectionConstantBuffer* const pParentCB;
		ShaderReflectionVariable* const pParentVariable;
		std::vector<TypeTrait<ShaderReflectionVariable>::Ptr> m_Members;
		Name m_Name;
		uint32 m_StartOffset;
		uint32 m_Size = 0;
		ShaderVariableClass m_Class;
		ShaderVariableType m_Type;
		uint32 m_Columns;
		uint32 m_Rows;
	};

	typedef enum _ResourceReturnType
	{
		RRT_Unknown = 0,
		RRT_Unorm = 1,
		RRT_Snorm = 2,
		RRT_Sint = 3,
		RRT_Uint = 4,
		RRT_Float = 5,
		RRT_Mixed = 6,
		RRT_Double = 7,
	}ResourceReturnType;

	typedef enum _ResourceDimension
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
	}ResourceDimension;

	class ShaderReflectionBindResource : virtual public Uncopyable
	{
	public:
		virtual ~ShaderReflectionBindResource();

		ShaderReflection* GetShaderReflection() const;

		Name GetName() const;
		ShaderInputType GetType() const;
		uint32 GetBindPoint() const;
		uint32 GetBindCount() const;
		ResourceReturnType GetReturnType() const;
		ResourceDimension GetDimension() const;

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