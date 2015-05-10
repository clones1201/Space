#ifndef __SPACE_EFFECTS_HPP__
#define __SPACE_EFFECTS_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"
#include "Utility.hpp"
#include "Shader.hpp"
#include "ShaderReflection.hpp"

namespace Space
{
	class StaticParameterSet;

	class SPACE_API StaticBoolParameter
	{
	public:
		~StaticBoolParameter();

		Name GetName() const;
		void SetName(Name const& name);
		bool GetValue() const;
		void SetValue(bool value);

		bool operator==(StaticBoolParameter const& other) const;
	private:
		StaticParameterSet* pSet;
		Name m_Name;
		uint8 m_Bit;
		StaticBoolParameter(StaticParameterSet* pSet, std::string const& name, uint8 bit);
		StaticBoolParameter(StaticParameterSet* pSet, std::wstring const& name, uint8 bit);
		StaticBoolParameter(StaticParameterSet* pSet, const Name& name, uint8 bit);

		//uint32 GetHashCode() const;

		friend class StaticParameterSet;
	};
	
	class SPACE_API StaticMaskParameter
	{
	public:
		~StaticMaskParameter();

		Name GetName() const;
		void SetName(Name const& name);
		bool GetMaskR() const;
		bool GetMaskG() const;
		bool GetMaskB() const;
		bool GetMaskA() const;

		void SetMaskR(bool value);
		void SetMaskG(bool value);
		void SetMaskB(bool value);
		void SetMaskA(bool value);
		 
		bool operator==(StaticMaskParameter const& other) const;
	private:
		StaticParameterSet* pSet;
		Name m_Name;
		uint8 m_BitR;
		uint8 m_BitG;
		uint8 m_BitB;
		uint8 m_BitA;
		StaticMaskParameter(
			StaticParameterSet* pSet, std::string const& name, 
			uint8 bitR, uint8 bitG, uint8 bitB, uint8 bitA);
		StaticMaskParameter(
			StaticParameterSet* pSet, std::wstring const& name,
			uint8 bitR, uint8 bitG, uint8 bitB, uint8 bitA);
		StaticMaskParameter(
			StaticParameterSet* pSet, Name const& name,
			uint8 bitR, uint8 bitG, uint8 bitB, uint8 bitA);

		friend class StaticParameterSet;
	};

	/* can have 32 static switch at all or 8 static mask at all */
	class SPACE_API StaticParameterSet
	{
	public:
		StaticParameterSet();
		
		// return a vector of sets, which includes all possible value sets
		std::vector<StaticParameterSet> GetAllStaticParameterSet();
		
		void Clear();

		void AddSwitch(Name const& name, 
			bool defaultValue = false);
		void AddMask(Name const& name,
			bool maskR = false, bool maskG = false, 
			bool maskB = false, bool maskA = false);
				
		int32 GetSwitchCount() const;
		int32 GetMaskCount() const;

		// throw exception if no parameter has this name
		StaticBoolParameter& GetSwitchByName(Name name);		
		// throw exception if no parameter has this name
		StaticMaskParameter& GetMaskByName(Name name);

		typedef std::unordered_map<Name, StaticBoolParameter>::iterator SwitchIterator;
		typedef std::unordered_map<Name, StaticBoolParameter>::const_iterator ConstSwitchIterator;
		SwitchIterator SwitchBegin();
		ConstSwitchIterator CSwitchBegin() const;
		SwitchIterator SwitchEnd();
		ConstSwitchIterator CSwitchEnd() const;

		typedef std::unordered_map<Name, StaticMaskParameter>::iterator MaskIterator;
		typedef std::unordered_map<Name, StaticMaskParameter>::const_iterator ConstMaskIterator;
		MaskIterator MaskBegin();
		ConstMaskIterator CMaskBegin() const;
		MaskIterator MaskEnd();
		ConstMaskIterator CMaskEnd() const;
		
		// hash code only depent on values, without names
		uint32 GetHashCode() const;
	
		uint32 _GetBits() const;
		void _SetBits(uint32 bits);
		bool _GetBoolValueFromBits(uint8 bit) const;
		void _SetBoolValueToBits(uint8 bit,bool value);

		bool operator==(StaticParameterSet const& other) const;
	private: 
		std::unordered_map<Name, StaticBoolParameter> m_SwitchParameters;
		std::unordered_map<Name, StaticMaskParameter> m_MaskParameters;

		uint32 m_Bits = 0;
		uint8 m_BitCount = 0;
	};
}

namespace std
{ 
	template<>
	class std::hash < Space::StaticParameterSet >
	{
	public:
		size_t operator()(Space::StaticParameterSet const& param) const
		{
			return param.GetHashCode();
		}
	};
}

namespace Space
{
	class SPACE_API Shader : virtual public Uncopyable
	{
	public:
		static Shader* Create(RenderSystem* pRenderSys, std::string const& name);
		static Shader* Create(RenderSystem* pRenderSys, std::wstring const& name);

		void SetShaderResourcePool(ShaderResourcePool* pPool);

		void Apply();

		virtual ~Shader();
	protected:
		Shader(RenderSystem* pRenderSys, std::string const& name);
		
		std::unique_ptr<VertexShader> m_pVS = nullptr;
		std::unique_ptr<PixelShader> m_pPS = nullptr;

		std::unique_ptr<ShaderReflection> m_pVSReflect = nullptr;
		std::unique_ptr<ShaderReflection> m_pPSReflect = nullptr;

		ShaderResourcePool * pResourcePool;
	};

	typedef SPACE_API enum _MaterialDomain
	{
		MD_Surface = 0,
		MD_PostProcess,
		MD_Max
	}MaterialDomain;

	typedef SPACE_API enum _MaterialBlendMode
	{
		MB_Opaque = 0,
		MB_Masked,
		MB_Translucent,
		MB_Additive,
		MB_Max
	}MaterialBlendMode;
		
	class SPACE_API Material : virtual public Uncopyable
	{
	public:
		static Material* Create(RenderSystem* pRenderSys, std::string const& name);
		static Material* Create(RenderSystem* pRenderSys, std::wstring const& name);

		StaticParameterSet const& GetStaticParameterSet() const;
		StaticParameterSet& GetStaticParameterSet();

		Name GetName() const;
		void Apply(RenderSystem* pRenderSys);
		virtual ~Material();
	protected:
		Material(RenderSystem* pRenderSys, std::string const& name);
		
		void SelectShader();

		Name m_Name;
		MaterialDomain m_Domain;
		MaterialBlendMode m_BlendMode;

		StaticParameterSet m_ParameterSet;
		StaticParameterSet m_DefaultParameterSet;

		Shader* m_CurrentShader = nullptr;

		std::unordered_map<Name, std::unique_ptr<Shader>> m_Shaders;
		std::unordered_map<StaticParameterSet, Shader*> m_ShaderMap;
	};
}

#endif