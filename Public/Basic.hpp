#ifndef __SPACE_BASIC_HPP__
#define __SPACE_BASIC_HPP__
 
#include "Prerequisites.hpp"

namespace Space
{
	template <typename _Type>
	class SPACE_API TypeTraitBase
	{
	public:
		typedef typename std::remove_reference< _Type >::type*	RawPtr;
		typedef typename std::remove_reference< _Type >::type*	RawConstPtr;
		typedef typename std::remove_reference< _Type >::type&	Ref;
		typedef typename std::remove_reference< _Type >::type const&  ConstRef;
		typedef typename std::shared_ptr< typename std::remove_reference< _Type >::type > Ptr;
		typedef typename std::shared_ptr< typename std::remove_reference< _Type >::type const > ConstPtr;
	};

	template < typename _Type > class SPACE_API TypeTrait : public TypeTraitBase < _Type > {};

	class SPACE_API Object{
	public:
		virtual ~Object(){}
	};

	template <class _Type>
	class SPACE_API SharedPtrObject : virtual public std::enable_shared_from_this < _Type >
	{
	public:
		virtual ~SharedPtrObject(){}
	};

	class SPACE_API Uncopyable{
	protected:
		Uncopyable();
		virtual ~Uncopyable();
	private:
		Uncopyable(const Uncopyable&) = delete;
		Uncopyable& operator=(const Uncopyable&) = delete;
	};


	class SPACE_API Interface : private Uncopyable
	{
	protected:
		Interface(); 
	public:
		virtual ~Interface();
	};

	class SPACE_API RefCountObject : public Uncopyable
	{
	protected:
		std::atomic<int32> m_RefCount = 0;
		RefCountObject();
	public:
		virtual ~RefCountObject();
		inline int32 AddRef()
		{
			return m_RefCount++;
		}
		inline int32 Release()
		{
			int32 ret = m_RefCount--;
			if (m_RefCount == 0)
			{
				assert(nullptr != this);
				delete this;
			}
			return ret;
		}
	};

	template<typename Type>
	class SPACE_API RefCountPtr
	{
	public:
		RefCountPtr(Type* p) :p(p){}
		~RefCountPtr(){ if(p!= nullptr) p->Release(); }

		RefCountPtr(RefCountPtr<Type> const& Other)
		{
			p = Other.p;
			if (p != nullptr)
				p->AddRef();
		}

		template<typename TypeB>
		RefCountPtr(RefCountPtr<TypeB> const& Other)
		{
			if (dynamic_cast<Type*>(Other.p) != nullptr)
			{
				p = Other.p;
				if (p != nullptr)
					p->AddRef();
			}
		}

		template <typename TypeB>
		bool QueryInterface(TypeB** interf)
		{
			if (nullptr !=
				(*interf = dynamic_cast<typename std::remove_reference<TypeB>::type*>(p)))
			{
				return true;
			}
			return false;			
		}

		Type* p;
	};

	class SPACE_API Archiveable
	{
	public:
		virtual ~Archiveable();
		virtual std::ostream& Write(std::ostream& archiver) const = 0;
		virtual std::istream& Read(std::istream& archiver) = 0;
	};

#define TRY_CATCH_LOG(expression,result) try{\
	expression; }\
	catch(std::exception& e){\
	Log("%s\n",e.what());\
	result;}

#define TRY_CATCH_OUT_OF_RANGE(expression,result)  try{\
	expression; }\
	catch(std::out_of_range&){\
	result;}

}

#endif