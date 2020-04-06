#include "Prerequisites.hpp" 

#include "Utility.hpp"

#include <locale>
#include <codecvt>

namespace Space
{

	tstring toTs(const std::string &in)
	{
#ifdef SPACE_UNICODE
		return str2wstr(in);
#else
		return wstr2str(in);
#endif
	}
	tstring toTs(const std::wstring &in)
	{
#ifdef SPACE_UNICODE
		return str2wstr(in);
#else
		return wstr2str(in);
#endif
	}
	std::wstring str2wstr(const std::string &in){
		static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		return converter.from_bytes(in);
	}
	std::wstring str2wstr(const std::wstring &in)
	{
		return in;
	}
	std::string wstr2str(const std::wstring &in){
		static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		return converter.to_bytes(in);
	}
	std::string wstr2str(const std::string &in)
	{
		return in;
	}

	int32 GetVarArgsW(wchar_t* dest, uint32 size, const wchar_t* fmt, va_list args){
		int32 result = _vsnwprintf_s(dest, size, size - 1, fmt, args);
		va_end(args);
		return result;
	}

	int32 GetVarArgsA(char* dest, uint32 size, const char* fmt, va_list args)
	{
		int32 result = _vsnprintf_s(dest, size, size - 1, fmt, args);
		va_end(args);
		return result;
	}
	std::wstring StringFormatW(const wchar_t* format, ...)
	{
		va_list args;
		va_start(args, format);
		wchar_t buffer[2048];
		GetVarArgsW(buffer, 2048, format, args);
		return buffer;
	}
	std::string StringFormatA(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		char buffer[2048];
		GetVarArgsA(buffer, 2048, format, args);
		return buffer;
	}

	float TStoF(const std::wstring& str){
		return (float)_wtof(str.c_str());
	}

	float TStoF(const std::string& str){
		return (float)atof(str.c_str());
	}

	std::string FtoA(float f)
	{
		return StringFormatA("%f", f);
	}
	std::wstring FtoW(float f)
	{
		return StringFormatW(TEXT("%f"), f);
	}

	std::ostream& Write(std::ostream& archiver, std::string const& str)
	{
		std::string::size_type size = str.size();
		archiver.write((sbyte*)&size, sizeof(std::string::size_type));
		archiver.write((sbyte*)str.c_str(), sizeof(std::string::value_type) * (size + 1));
		return archiver;
	}
	std::istream& Read(std::istream& archiver, std::string& str)
	{
		std::string::size_type size;
		archiver.read((sbyte*)&size, sizeof(std::string::size_type));
		char* buf = new char[size + 1];
		archiver.read((sbyte*)buf, sizeof(std::string::value_type) * (size + 1));
		str = std::string(buf);
		delete [] buf;
		return archiver;
	}

	std::ostream& Write(std::ostream& archiver, std::wstring const& str)
	{
		std::string::size_type size = str.size();
		archiver.write((sbyte*)&size, sizeof(std::wstring::size_type));
		archiver.write((sbyte*)str.c_str(), sizeof(std::wstring::value_type) * (size + 1));
		return archiver;
	}
	std::istream& Read(std::istream& archiver, std::wstring& str)
	{
		std::wstring::size_type size;
		archiver.read((sbyte*)&size, sizeof(std::wstring::size_type));
		wchar_t* buf = new wchar_t[size + 1];
		archiver.read((sbyte*)buf, sizeof(std::wstring::value_type) * (size + 1));
		str = std::wstring(buf);
		delete [] buf;
		return archiver;
	}

#ifdef X86_64
#	define ALIGNMENT 32
#else
#	define ALIGNMENT 16
#endif

	size_t Alignment(size_t n){
		return (ALIGNMENT * (1 + (n - 1) / ALIGNMENT));
	}

	class Name::Impl
	{
	public:
		typedef std::list<std::weak_ptr<Name::Impl>> StringPool;

		static StringPool g_StringPool;

		std::weak_ptr<Name::Impl> pSelf;
		std::unique_ptr<tstring> pStr = nullptr;

		Impl(){}
		~Impl()
		{
			g_StringPool.erase(
				std::find_if(g_StringPool.begin(),g_StringPool.end(), 
				[&](std::weak_ptr<Name::Impl> const& param1)->bool
			{
				return pSelf.lock() == param1.lock();
			}));
		}
	};

	Name::Impl::StringPool Name::Impl::g_StringPool;

	Name::Name()
		:Name(L"")
	{
	}
	
#ifdef SPACE_UNICODE
	Name::Name(std::string const& name)
		:Name(str2wstr(name))
#else
	Name::Name(std::wstring const& name)
		:Name(wstr2str(name))
#endif
	{
	}

	Name::Name(tstring const& name)
	{
		bool found = false;
		std::for_each(Impl::g_StringPool.begin(), Impl::g_StringPool.end(),
			[&name, this, &found](Impl::StringPool::value_type& elem)
		{			
			if (*(elem.lock()->pStr) == name){
				this->m_impl = elem.lock();
				found = true;
			}
		});
		if (!found){
			m_impl.reset(new Name::Impl());
			m_impl->pSelf = m_impl;
			m_impl->pStr.reset(new tstring(name));
			Impl::g_StringPool.push_back(m_impl);
		}
	}
	Name::Name(Name const& param) noexcept
		:m_impl(param.m_impl)
	{}
	Name::Name(Name&& param) noexcept
	{
		std::swap(param.m_impl, this->m_impl);
	}
	Name & Name::operator=(Name const & param)
	{
		m_impl = param.m_impl;
		return (*this);
	}
	Name & Name::operator=(Name && param)
	{
		std::swap(param.m_impl, this->m_impl);
		return (*this);
	}
	std::wstring Name::ToWString() const
	{
#ifdef SPACE_UNICODE
		return *(m_impl->pStr);
#else
		return str2wstr(*(m_impl->pStr));
#endif
	}
	std::string Name::ToString() const
	{
#ifdef SPACE_UNICODE
		return wstr2str(*(m_impl->pStr));
#else
		return *(m_impl->pStr);
#endif
	} 
	
	std::ostream& Name::Write(std::ostream& archiver) const
	{
		Space::Write(archiver, ToString());
		return archiver;
	}
	std::istream& Name::Read(std::istream& archiver)
	{
		std::string name;
		Space::Read(archiver, name);
		(*this) = Name(name);
		return archiver;
	}

	UidGenerator::UidGenerator()
	{
		m_UidList.push_back(std::pair<uint, uint>(0, ((uint)-1) - 1));
	}
	uint32 UidGenerator::GetNextValidId()
	{
		auto region = m_UidList.begin();
		uint32 ret = region->first;
		if (region->first == region->second){
			m_UidList.erase(region);
		}
		else{
			region->first++;
		}
		return ret;
	}
	void UidGenerator::RevokeId(uint32 id)
	{
		auto i1 = m_UidList.begin();
		auto i2 = m_UidList.begin(); ++i2;
		for (; i1 != m_UidList.end(); ++i1, ++i2)
		{
			if (id < i1->first && id != i1->first - 1)
			{
				m_UidList.insert(i1, std::pair<uint, uint>(id, id));
				return;
			}
			else if (id == i1->first - 1)
			{
				i1->first--;
				return;
			}
			else if (id == i1->second + 1 && i2 != m_UidList.end() && id != i2->first)
			{
				i1->second++;
				return;
			}
			else// if (id >= i1->first && id <= i1->second)
			{
				assert(0);
			}
		}
	}

	void PropertyManager::AddProperty(PropertyBase* prop)
	{
		m_Properties.push_back(prop);
		m_PropertiesMap.insert(
			std::make_pair(
				prop->GetName(), prop
				));
	}

}
