#ifndef __SPACE_EFFECTS_HPP__
#define __SPACE_EFFECTS_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"
#include "Utility.hpp"
#include "Shader.hpp"
#include "ShaderReflection.hpp"

namespace Space
{
	class SPACE_API StaticBoolParameter
	{
	private:
		Name m_Name;
		bool m_Value : 1;
	public:
		StaticBoolParameter();
		StaticBoolParameter(std::string const& name, bool defaultValue = false);
		StaticBoolParameter(std::wstring const& name, bool defaultValue = false);
		StaticBoolParameter(const Name& name, bool defaultValue = false);

		~StaticBoolParameter();

		Name GetName() const;
		void SetName(Name const& name);
		bool GetValue() const;
		void SetValue(bool value);
		uint32 GetHashCode() const;

		//	virtual std::ostream& Write(std::ostream& archiver) const;
		//	virtual std::istream& Read(std::istream& archiver);
		//bool operator == (StaticBoolParameter const& param) const;
		//bool operator != (StaticBoolParameter const& param) const;
	};

	class TStaticBoolParameter;
	class TStaticComponentMaskParameter;

	template <class Ty>
	class IsSwitchParameter
	{
	};

	template<>
	class IsSwitchParameter < TStaticBoolParameter >
	{
		static const bool value = true;
	};

	template<>
	class IsSwitchParameter < TStaticComponentMaskParameter >
	{
		static const bool value = false;
	};
	
	template <class Ty>
	class TStaticParameter
	{
	public:
		Name m_Name;

		typedef	typename std::enable_if <IsSwitchParameter<Ty>::value, bool> ::type
			SwitchBoolValue;
		typedef	typename std::enable_if <!IsSwitchParameter<Ty>::value, bool> ::type
			MaskBoolValue;

		inline SwitchBoolValue GetValue() const
		{
			return m_Value;
		}
		inline void SetValue(SwitchBoolValue value)
		{
			m_Value = value;
		}

		inline MaskBoolValue GetValueR() const
		{
			return m_ValueR;
		}
		inline MaskBoolValue GetValueG() const
		{
			return m_ValueG;
		}
		inline MaskBoolValue GetValueB() const
		{
			return m_ValueB;
		}
		inline MaskBoolValue GetValueA() const
		{
			return m_ValueA;
		}
		inline void SetValueR(MaskBoolValue value)
		{
			m_ValueR = value;
		}
		inline void SetValueG(MaskBoolValue value)
		{
			m_ValueG = value;
		}
		inline void SetValueB(MaskBoolValue value)
		{
			m_ValueB = value;
		}
		inline void SetValueA(MaskBoolValue value)
		{
			m_ValueA = value;
		}

	private:
		SwitchBoolValue m_Value = false;
		MaskBoolValue m_ValueR = false;
		MaskBoolValue m_ValueG = false;
		MaskBoolValue m_ValueB = false;
		MaskBoolValue m_ValueA = false;
	};

	class SPACE_API TStaticBoolParameter 
		: public TStaticParameter < TStaticBoolParameter >
	{
		
	};

	class SPACE_API TStaticComponentMaskParameter 
		: public TStaticParameter < TStaticComponentMaskParameter >
	{

	};

	class SPACE_API StaticComponentMaskParameter
	{
	private:
		Name ParamName;
		bool maskR : 1, maskG : 1, maskB : 1, maskA : 1;
	public:
		StaticComponentMaskParameter();
		StaticComponentMaskParameter(std::string const& name,
			bool maskR = false,bool maskG = false, bool maskB = false, bool maskA = false);
		StaticComponentMaskParameter(std::wstring const& name,
			bool maskR = false, bool maskG = false, bool maskB = false, bool maskA = false);
		StaticComponentMaskParameter(Name const& name,
			bool maskR = false, bool maskG = false, bool maskB = false, bool maskA = false);

		bool GetMaskR() const;
		bool GetMaskG() const;
		bool GetMaskB() const;
		bool GetMaskA() const;

		void SetMaskR(bool value);
		void SetMaskG(bool value);
		void SetMaskB(bool value);
		void SetMaskA(bool value);

		uint32 GetHashCode() const;

		//	virtual std::ostream& Write(std::ostream& archiver) const;
		//	virtual std::istream& Read(std::istream& archiver);
		//bool operator == (StaticComponentMaskParameter const& param) const;
		//bool operator != (StaticComponentMaskParameter const& param) const;
	};

	class SPACE_API StaticParameterSet
	{
	private:
		std::vector<StaticBoolParameter> SwitchParameters;
		std::vector<StaticComponentMaskParameter> ComponentMaskParameters;
	public:
		StaticParameterSet();
		StaticParameterSet(StaticParameterSet const& other);
		StaticParameterSet& operator=(StaticParameterSet const& other);
		StaticParameterSet(StaticParameterSet && other);
		StaticParameterSet& operator=(StaticParameterSet && other);

		std::vector<StaticParameterSet> GetAllStaticParameterSet();
		
		void Clear();

		void AddSwitch(Name const& name);
		void RemoveSwitch(Name const& name);
		void AddMask(Name const& name);
		void RemoveMask(Name const& name);
		
		int32 GetSwitchCount() const;
		int32 GetMaskCount() const;

		StaticBoolParameter& GetSwitchByIndex(int32 idx);
		StaticBoolParameter& GetSwitchByName(Name name);
		StaticComponentMaskParameter& GetMaskByIndex(int32 idx);
		StaticComponentMaskParameter& GetMaskByName(Name name);

		bool operator == (const StaticParameterSet& param) const;
		bool operator != (const StaticParameterSet& param) const;

		uint32 GetHashCode() const;
		//	virtual std::ostream& Write(std::ostream& archiver) const;
		//	virtual std::istream& Read(std::istream& archiver);
	};
}

namespace std
{
	template<>
	class std::hash < Space::StaticBoolParameter >
	{
	public:
		size_t operator()(Space::StaticBoolParameter const& param) const
		{
			return param.GetHashCode();
		}
	};

	template<>
	class std::hash < Space::StaticComponentMaskParameter >
	{
	public:
		size_t operator()(Space::StaticComponentMaskParameter const& param) const
		{
			return param.GetHashCode();
		}
	};

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

		Name m_Name;
		MaterialDomain m_Domain;
		MaterialBlendMode m_BlendMode;

		StaticParameterSet m_ParameterSet;
		StaticParameterSet m_DefaultParameterSet;

		std::shared_ptr<Shader> m_CurrentShader = nullptr;

		std::unordered_map<Name, std::shared_ptr<Shader>> m_Shaders;
		std::unordered_map<StaticParameterSet, Shader*> m_ShaderMap;
	};
}

#endif