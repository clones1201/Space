#ifndef __SPACE_EFFECTS_HPP__
#define __SPACE_EFFECTS_HPP__

#include "Space/Prerequisites.hpp"

namespace Space
{
	class StaticParameterSet;

	class SPACE_API StaticBoolParameter
	{
	public:
		~StaticBoolParameter();

		inline Name GetName() const
		{
			return m_Name;
		}
		inline void SetName(Name const& name)
		{
			m_Name = name;
		}
		bool GetValue() const;
		void SetValue(bool value);

		inline bool operator==(StaticBoolParameter const& other) const
		{
			return (m_Bit == other.m_Bit) && (m_Name == other.m_Name);
		}
	private:
		StaticParameterSet* pSet;
		Name m_Name;
		uint8 m_Bit = -1;
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

		inline Name GetName() const
		{
			return m_Name;
		}
		inline void SetName(Name const& name)
		{
			m_Name = name;
		}
		bool GetMaskR() const;
		bool GetMaskG() const;
		bool GetMaskB() const;
		bool GetMaskA() const;

		void SetMaskR(bool value);
		void SetMaskG(bool value);
		void SetMaskB(bool value);
		void SetMaskA(bool value);

		inline bool operator==(StaticMaskParameter const& other) const
		{
			return
				(m_BitR == other.m_BitR) &&
				(m_BitG == other.m_BitG) &&
				(m_BitB == other.m_BitB) &&
				(m_BitA == other.m_BitA) &&
				(m_Name == other.m_Name);
		}
	private:
		StaticParameterSet* pSet = nullptr;
		Name m_Name;
		uint8 m_BitR = -1;
		uint8 m_BitG = -1;
		uint8 m_BitB = -1;
		uint8 m_BitA = -1;
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

		inline int32 GetSwitchCount() const
		{
			return m_SwitchParameters.size();
		}
		inline int32 GetMaskCount() const
		{
			return m_MaskParameters.size();
		}

		// throw exception if no parameter has this name
		inline StaticBoolParameter& GetSwitchByName(Name const& name)
		{
			return m_SwitchParameters.at(name);
		}
		// throw exception if no parameter has this name
		inline StaticMaskParameter& GetMaskByName(Name const& name)
		{
			return m_MaskParameters.at(name);
		}
		typedef std::unordered_map<Name, StaticBoolParameter, Name::Hasher> SwitchSet;
		typedef SwitchSet::iterator SwitchIterator;
		typedef SwitchSet::const_iterator ConstSwitchIterator;
		inline SwitchIterator SwitchBegin()
		{
			return m_SwitchParameters.begin();
		}
		inline ConstSwitchIterator CSwitchBegin() const
		{
			return m_SwitchParameters.cbegin();
		}
		inline SwitchIterator SwitchEnd()
		{
			return m_SwitchParameters.end();
		}
		inline ConstSwitchIterator CSwitchEnd() const
		{
			return m_SwitchParameters.cend();
		}
		typedef std::unordered_map<Name, StaticMaskParameter, Name::Hasher> MaskSet;
		typedef MaskSet::iterator MaskIterator;
		typedef MaskSet::const_iterator ConstMaskIterator;
		inline MaskIterator MaskBegin()
		{
			return m_MaskParameters.begin();
		}
		inline ConstMaskIterator CMaskBegin() const
		{
			return m_MaskParameters.cbegin();
		}
		inline MaskIterator MaskEnd()
		{
			return m_MaskParameters.end();
		}
		inline ConstMaskIterator CMaskEnd() const
		{
			return m_MaskParameters.cend();
		}

		// hash code only depent on values, without names
		inline uint32 GetHashCode() const
		{
			std::hash<uint32> hasher;
			return hasher(m_Bits);
		}

		class Hasher
		{
		public:
			size_t operator()(StaticParameterSet const& s) const
			{
				return s.GetHashCode();
			}
		};

		inline uint32 _GetBits() const
		{
			return m_Bits;
		}
		inline void _SetBits(uint32 bits)
		{
			m_Bits = bits;
		}
		inline bool _GetBoolValueFromBits(uint8 bit) const
		{
			return (bool)((m_Bits >> bit) & 0x00000001);
		}
		inline void _SetBoolValueToBits(uint8 bit, bool value)
		{
			value ?
				m_Bits &= (1 << bit) :
				m_Bits |= ~(1 << bit);
		}

		inline bool operator==(StaticParameterSet const& other) const
		{
			return
				(m_BitCount == other.m_BitCount) &&
				(m_Bits == m_Bits) &&
				(m_SwitchParameters == other.m_SwitchParameters) &&
				(m_MaskParameters == other.m_MaskParameters);
		}

	private:
		SwitchSet m_SwitchParameters;
		MaskSet m_MaskParameters;

		uint32 m_Bits = 0;
		uint8 m_BitCount = 0;
	};

	typedef std::shared_ptr<StaticParameterSet> StaticParameterSetPtr;


	

	enum class SPACE_API MaterialDomain : uint8
	{
		Surface = 0,
		PostProcess,
		Max
	};

	enum class SPACE_API MaterialBlendMode : uint8
	{
		Opaque = 0,
		Masked,
		Translucent,
		Additive,
		Max
	};

	class SPACE_API Material : virtual public Uncopyable
	{
	public:
		static Material* Create(RenderSystem* pRenderSys, tstring const& name);

		StaticParameterSet const& GetStaticParameterSet() const;
		StaticParameterSet& GetStaticParameterSet();

		Name GetName() const;
		MaterialDomain GetDomain() const;
		MaterialBlendMode GetBlendMode() const;

		inline void SetWorld(Float4x4 world)
		{
			m_World = world;
		}
		inline void SetView(Float4x4 view)
		{
			m_View = view;
		}
		inline void SetProjection(Float4x4 projection)
		{
			m_Projection = projection;
		}
		inline void SetGameTime(float time)
		{
			m_Time = time;
		}

		// TODO 
		//TextureParameter& GetTextureParameter(Name const& name);
		//ScalarParameter& GetScalarParameter(Name const& name);
		//VectorParameter& GetVectorParameter(Name const& name);

		//void Apply();
		Shader* GetShader(CommandList* pList);

		virtual ~Material();
	protected:
		Material(RenderSystem* pRenderSys, tstring const& name);

		void SelectShader();
		RenderSystem* pRenderSystem;

		Name m_Name;
		MaterialDomain m_Domain;
		MaterialBlendMode m_BlendMode;

		StaticParameterSet m_ParameterSet;
		StaticParameterSet m_DefaultParameterSet;

		std::unique_ptr<ConstantBuffer> m_CommonVariablesBuffer;
		Float4x4 m_World;
		Float4x4 m_View;
		Float4x4 m_Projection;
		float m_Time;

		Shader* m_CurrentShader = nullptr;

		std::unordered_map<Name, std::unique_ptr<Shader>, Name::Hasher> m_Shaders;
		std::unordered_map<StaticParameterSet, Shader*, StaticParameterSet::Hasher> m_ShaderMap;

		PipelineStatePtr m_pPipelineState = nullptr;
	};

	typedef std::shared_ptr<Material> MaterialPtr;
}

#endif