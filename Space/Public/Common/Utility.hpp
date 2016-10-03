#ifndef __SPACE_COMMON_UTILITY_H__
#define __SPACE_COMMON_UTILITY_H__

#include "Common/Prerequisites.hpp" 
#include "Common/Basic.hpp"

namespace Space
{
	SPACE_COMMON_API tstring toTs(const std::string &in);
	SPACE_COMMON_API tstring toTs(const std::wstring &in);
	SPACE_COMMON_API std::wstring str2wstr(const std::string &in);
	SPACE_COMMON_API std::wstring str2wstr(const std::wstring &in);
	SPACE_COMMON_API std::string wstr2str(const std::wstring &in);
	SPACE_COMMON_API std::string wstr2str(const std::string &in);
	
	SPACE_COMMON_API int32 GetVarArgsW(wchar_t* dest, uint32 size, const wchar_t* fmt, va_list args);
	SPACE_COMMON_API int32 GetVarArgsA(char* dest, uint32 size, const char* fmt, va_list args);

#if (defined _UNICODE) || (defined UNICODE)
#define GetVarArgs GetVarArgsW
#else
#define GetVarArgs GetVarArgsA
#endif

	SPACE_COMMON_API std::wstring StringFormatW(const wchar_t* format, ...);
	SPACE_COMMON_API std::string StringFormatA(const char* format, ...);
#if (defined _UNICODE) || (defined UNICODE)
#define StringFormat StringFormatW
#else
#define StringFormat StringFormatW
#endif

	SPACE_COMMON_API float TStoF(const std::wstring& str);
	SPACE_COMMON_API float TStoF(const std::string& str);

	SPACE_COMMON_API std::string FtoA(float f);
	SPACE_COMMON_API std::wstring FtoW(float f);

	template < class T , class Container = std::vector<T> >
	int32 AddUnique(Container& array, T elem){
		int32 index = 0;
		for (auto i = array.begin(); i != array.end(); ++i)
		{
			if (*i == elem)
				return index;
			++index;
		}
		array.push_back(elem);
		return index;
	}

	template< class T, class Container = std::vector<T> >
	uint32 FindPosition(Container const& array, T elem)
	{
		return std::distance(array.cbegin(), std::find(array.cbegin(), array.cend(), elem));
	}

	template<class Key, class T>
	std::vector<T> MapToVector(const std::map<Key, T>& map)
	{
		std::vector<T> ret;
		std::for_each(map.begin(), map.end(),
			[&ret](std::map<Key, T>::value_type elem)
		{
			ret.push_back(elem.second);
		});
		return std::move(ret);
	}

	std::ostream& Write(std::ostream& out, std::string const& str);
	std::istream& Read(std::istream& in, std::string& str);

	std::ostream& Write(std::ostream& out, std::wstring const& str);
	std::istream& Read(std::istream& in, std::wstring& str);

#ifdef X86_64
#	define ALIGNMENT 32
#else
#	define ALIGNMENT 16
#endif

	SPACE_COMMON_API int32 Alignment(int32 n);
	
	class SPACE_COMMON_API Name
	{
	private:
		class Impl;
		TypeTrait<Impl>::Ptr m_impl;
	public:
		Name();
		Name(std::string const& name);
		Name(std::wstring const& name);
		Name(Name const& param);
		Name(Name && param);
		Name& operator=(Name const& param);
		Name& operator=(Name && param);

		std::wstring ToWString() const;
		std::string ToString() const;
		inline bool operator==(const Name&param) const
		{
			return m_impl == param.m_impl;
		}
		inline bool operator<(const Name&param) const
		{
			return m_impl < param.m_impl;
		}
		inline bool operator>(const Name&param) const
		{
			return m_impl > param.m_impl;
		}
		inline uint32 GetHashCode() const
		{
			std::hash<std::shared_ptr<Name::Impl>> hasher;
			return (uint32)hasher(m_impl);
		}

		class Hasher
		{
		public:
			inline size_t operator()(Name const& param) const
			{
				return param.GetHashCode();
			}
		};

		std::ostream& Write(std::ostream& archiver) const;
		std::istream& Read(std::istream& archiver);
	};
	
	class UidGenerator{
	private:
		std::list<std::pair<uint, uint>> m_UidList;

	public:
		UidGenerator();
		uint32 GetNextValidId();
		void RevokeId(uint32 id);
	};


	class PropertyBase;

	class SPACE_COMMON_API PropertyManager
	{
	public:
		inline size_t GetPropertyCount() const
		{
			assert(m_Properties.size() >= 0);
			return m_Properties.size();
		}

		void AddProperty(PropertyBase* prop);

	protected:
		std::vector<PropertyBase*> m_Properties;
		std::map<Name, PropertyBase*> m_PropertiesMap;

	};

	class SPACE_COMMON_API PropertyBase
	{
	public:
		enum class Type : int16
		{
			Float, Float2, Float3, Float4,
			Matrix,
			Int, Bool,
			Name, String,
			Object
		};

		inline Name GetName() const
		{
			return m_Name;
		}

		inline Type GetType() const
		{
			return m_Type;
		}

		inline tstring GetCategory() const
		{
			return m_Category;
		}

	protected:
		PropertyBase(Name name, Type type, tchar const* category, PropertyManager* host)
			:m_Name(name), m_Type(type), m_Category(category)
		{
			host->AddProperty(this);
		}

		Name m_Name;
		tstring m_Category;
		Type m_Type;
	};

	template<typename ValueType>
	class Property;

	template<typename ValueType>
	class Variant
	{
	public:
		inline typename ValueType Get() const
		{
			return m_Value;
		}
		inline void Set(typename ValueType value)
		{
			m_Value = value;
		}
		// TODO: using std::enable_if to fix it
		//private:
		//	friend class Property<ValueType>;
		//	friend class Property<ValueType::element_type>;
		//  friend class Property< std::is_same<std::shared_ptr<

		Variant(typename ValueType defaultValue)
			:m_Value(defaultValue) {}
	private:
		typename ValueType m_Value;
	};

	template<typename ObjectType>
	class Property : public PropertyBase, public Variant<std::shared_ptr<ObjectType>>
	{
	public:
		Property(Name const& name, std::shared_ptr<ObjectType> defaultValue,
			tchar const* category, PropertyManager* host)
			:PropertyBase(name, PropertyBase::Type::Object, category, host),
			Variant<std::shared_ptr<ObjectType>>(defaultValue) {}
	};

	template<>
	class SPACE_COMMON_API Property<float> : public PropertyBase, public Variant<float>
	{
	public:
		Property(Name const& name, float const& defaultValue, tchar const* category, PropertyManager* host)
			:PropertyBase(name, PropertyBase::Type::Float, category, host),
			Variant<float>(defaultValue) {}
	};

	typedef Property<float> FloatProperty;

	template<>
	class SPACE_COMMON_API Property<Float2> : public PropertyBase, public Variant<Float2>
	{
	public:
		Property(Name const& name, Float2 const& defaultValue, tchar const* category, PropertyManager* host)
			:PropertyBase(name, PropertyBase::Type::Float2, category, host),
			Variant<Float2>(defaultValue) {}
	};

	typedef Property<Float2> Float2Property;

	template<>
	class SPACE_COMMON_API Property<Float3> : public PropertyBase, public Variant<Float3>
	{
	public:
		Property(Name const& name, Float3 const& defaultValue, tchar const* category, PropertyManager* host)
			:PropertyBase(name, PropertyBase::Type::Float3, category, host),
			Variant<Float3>(defaultValue) {}
	};

	typedef Property<Float3> Float3Property;

	template<>
	class SPACE_COMMON_API Property<Float4> : public PropertyBase, public Variant<Float4>
	{
	public:
		Property(Name const& name, Float4 const& defaultValue, tchar const* category, PropertyManager* host)
			:PropertyBase(name, PropertyBase::Type::Float4, category, host),
			Variant<Float4>(defaultValue) {}
	};

	typedef Property<Float4> Float4Property;

	template<>
	class SPACE_COMMON_API Property<Float4x4> : public PropertyBase, public Variant<Float4x4>
	{
	public:
		Property(Name const& name, Float4x4 const& defaultValue, tchar const* category, PropertyManager* host)
			:PropertyBase(name, PropertyBase::Type::Matrix, category, host),
			Variant<Float4x4>(defaultValue) {}
	};

	typedef Property<Float4x4> MatrixProperty;

	template<>
	class SPACE_COMMON_API Property<tstring> : public PropertyBase, public Variant<tstring>
	{
	public:
		Property(Name const& name, tstring const& defaultValue, tchar const* category, PropertyManager* host)
			:PropertyBase(name, PropertyBase::Type::String, category, host),
			Variant<tstring>(defaultValue) {}
	};

	typedef Property<tstring> StringProperty;

	template<>
	class SPACE_COMMON_API Property<Name> : public PropertyBase, public Variant<Name>
	{
	public:
		Property(Name const& name, Name const& defaultValue, tchar const* category, PropertyManager* host)
			:PropertyBase(name, PropertyBase::Type::Name, category, host),
			Variant<Name>(defaultValue) {}
	};

	typedef Property<Name> NameProperty;

	template<>
	class SPACE_COMMON_API Property<int> : public PropertyBase, public Variant<int>
	{
	public:
		Property(Name const& name, int const& defaultValue, tchar const* category, PropertyManager* host)
			:PropertyBase(name, PropertyBase::Type::Int, category, host),
			Variant<int>(defaultValue) {}
	};

	typedef Property<int> IntProperty;

	template<>
	class SPACE_COMMON_API Property<bool> : public PropertyBase, public Variant<bool>
	{
	public:
		Property(Name const& name, bool const& defaultValue, tchar const* category, PropertyManager* host)
			:PropertyBase(name, PropertyBase::Type::Matrix, category, host),
			Variant<bool>(defaultValue) {}
	};

	typedef Property<bool> BoolProperty;

#define DEF_PROPERTY(name,type,category,defaultValue) \
	Property<type> name = Property<type>(Name(#name),defaultValue,category,this);	 

#define DEF_FLOAT_PROPERTY(name,category,defaultValue) \
	FloatProperty name = FloatProperty(Name(#name),defaultValue,category,this);

#define DEF_FLOAT2_PROPERTY(name,category,defaultValue) \
	Float2Property name = Float2Property(Name(#name),defaultValue,category,this);

#define DEF_FLOAT3_PROPERTY(name,category,defaultValue) \
	Float3Property name = Float3Property(Name(#name),defaultValue,category,this);

#define DEF_FLOAT4_PROPERTY(name,category,defaultValue) \
	Float4Property name = Float4Property(Name(#name),defaultValue,category,this);

#define DEF_MATRIX_PROPERTY(name,category,defaultValue) \
	MatrixProperty name = MatrixProperty(Name(#name),defaultValue,category,this);

#define DEF_INT_PROPERTY(name,category,defaultValue) \
	IntProperty name = IntProperty(Name(#name),defaultValue,category,this);

#define DEF_BOOL_PROPERTY(name,category,defaultValue) \
	BoolProperty name = BoolProperty(Name(#name),defaultValue,category,this);

#define DEF_NAME_PROPERTY(name,category,defaultValue) \
	NameProperty name = NameProperty(Name(#name),defaultValue,category,this);

#define DEF_STRING_PROPERTY(name,category,defaultValue) \
	StringProperty name = StringProperty(Name(#name),defaultValue,category,this);

#define DEF_OBJECT_PROPERTY(name,objecttype, category,defaultValue) \
	Property<objecttype> name = Property<objecttype>(Name(#name),std::shared_ptr<objecttype>(defaultValue), category, this);

	//return path of Assets folder
	SPACE_COMMON_API std::string GetAssetsPath();

	// convert degree to radian
	inline long double operator""_deg(long double deg)
	{
		return Pi * (deg / 180.0);
	}

}

#endif