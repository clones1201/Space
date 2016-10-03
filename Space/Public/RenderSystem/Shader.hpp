#ifndef __SPACE_RENDERSYSTEM_SHADER_HPP__
#define __SPACE_RENDERSYSTEM_SHADER_HPP__

#include "RenderSystem/Prerequisites.hpp"
#include "RenderSystem/Shared.hpp"

#include "RenderSystem/ShaderReflection.hpp"

namespace Space
{

	/*class ConstantBufferMap
	{
	public:
		
		static ConstantBufferMap* Create(RenderSystem* pRenderSys,std::string const& name);

		void AddVariable(std::string const& name, uint32 size, uint32 offset);
		ShaderVariableDesc GetVariableDescByIndex(uint32 index) const;
		ShaderVariableDesc GetVariableDescByName(std::string const& name) const;
		bool SetVariableValue(std::string const& name, byte const* pData);
		uint32 GetVariablesCount() const;

		void Complete();
		void Update();

		std::string GetName() const;
		bool IsComplete() const;
		uint32 GetBufferSize() const;

		~ConstantBufferMap();
	protected:
		ConstantBufferMap(RenderSystem* pRenderSys, std::string const& name);

		std::unique_ptr<ConstantBuffer> m_pBuffer;

		std::unordered_map<Name, ShaderVariableDesc> m_vLayout;
		std::string m_Name;
		uint32 m_Size = 0;
		std::vector<byte> m_pRaw;

		bool m_IsComplete = false;

		RenderSystem* m_pRenderSystem = nullptr;
	};*/
	
	enum class SPACE_RENDERSYSTEM_API ShaderStage : uint8
	{
		SS_VertexShader = 0,
		SS_GeometryShader = 1,
		SS_PixelShader = 2,
		SS_Num = 3
	}; 

	class SPACE_RENDERSYSTEM_API ShaderBase : private Uncopyable
	{
	public:
		virtual ~ShaderBase();

		void ClearAllSlots();

		void SetConstantBuffer(uint slots, ConstantBuffer* buffer);
		void SetShaderResource(uint slots, ShaderResource* resource);

		// get the max number of the slot
		inline size_t GetConstantBufferCount() const
		{
			assert(m_MaxCBSlot >= 0);
			return m_MaxCBSlot + 1;
		}
		// get the max number of the slot
		inline size_t GetShaderResourceCount() const
		{
			assert(m_MaxSRSlot >= 0);
			return m_MaxSRSlot + 1;
		}

		ConstantBuffer* GetConstantBuffer(uint slot) const;
		ShaderResource* GetShaderResource(uint slot) const;

		inline byte const* GetByteCodes() const
		{
			return m_ByteCodes.data();
		}
		inline size_t GetSizeInBytes() const
		{
			return m_ByteCodes.size();
		}

		virtual ShaderReflection* CreateReflection() = 0;
	protected:
		ShaderBase();

		std::vector<byte> m_ByteCodes;

		typedef std::map<uint, ConstantBuffer*> ConstantBufferTable;
		typedef std::map<uint, ShaderResource*> ShaderResourceTable;
		ConstantBufferTable m_ConstantBufferTable;
		ShaderResourceTable m_ShaderResourceTable;

		int32 m_MaxCBSlot = 0;
		int32 m_MaxSRSlot = 0;
	};

	class SPACE_RENDERSYSTEM_API VertexShader : private Interface, virtual public ShaderBase
	{  
	public:
		static VertexShader* LoadBinaryFromMemory(RenderSystem* pRenderSys, byte const* byteCodes, size_t sizeInBytes);
				 
		virtual ~VertexShader();
	protected:		
		VertexShader();  
	};
	typedef std::shared_ptr<VertexShader> VertexShaderPtr;
	/*class GeometryShader : virtual public Interface
	{
	public:

		virtual ~GeometryShader();
	protected:
		GeometryShader();
	};*/

	class SPACE_RENDERSYSTEM_API PixelShader : private Interface, virtual public ShaderBase
	{
	public:
		static PixelShader* LoadBinaryFromMemory(RenderSystem* pRenderSys, byte const* byteCodes, size_t sizeInBytes);
				 
		virtual ~PixelShader();
	protected:
		PixelShader(); 
	};
	typedef std::shared_ptr<PixelShader> PixelShaderPtr;

	class SPACE_RENDERSYSTEM_API Shader : private Uncopyable
	{
	public:
		static Shader* Create(RenderSystem* pRenderSys, std::string const& name);
		static Shader* Create(RenderSystem* pRenderSys, std::wstring const& name);

		void SetShaderResourcePool(ShaderResourcePool* pPool);

		VertexShader* GetVertexShader() const;
		PixelShader* GetPixelShader() const;

		virtual ~Shader();
	protected:
		Shader(RenderSystem* pRenderSys, std::string const& name);

		std::unique_ptr<VertexShader> m_pVS = nullptr;
		std::unique_ptr<PixelShader> m_pPS = nullptr;

		std::unique_ptr<ShaderReflection> m_pVSReflect = nullptr;
		std::unique_ptr<ShaderReflection> m_pPSReflect = nullptr;

		ShaderResourcePool * pResourcePool;
	};
}

#endif