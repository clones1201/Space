#ifndef __SPACE_COMMON_BASIC_HPP__
#define __SPACE_COMMON_BASIC_HPP__
 
#include "Prerequisites.hpp"

namespace Space
{
	template <typename _Type>
	class SPACE_COMMON_API TypeTraitBase
	{
	public:
		typedef typename std::remove_reference< _Type >::type*	RawPtr;
		typedef typename std::remove_reference< _Type >::type*	RawConstPtr;
		typedef typename std::remove_reference< _Type >::type&	Ref;
		typedef typename std::remove_reference< _Type >::type const&  ConstRef;
		typedef typename std::shared_ptr< typename std::remove_reference< _Type >::type > Ptr;
		typedef typename std::shared_ptr< typename std::remove_reference< _Type >::type const > ConstPtr;
	};

	template < typename _Type > class SPACE_COMMON_API TypeTrait : public TypeTraitBase < _Type > {};

	class SPACE_COMMON_API Object{
	public:
		virtual ~Object(){}
	};

	
	template<typename Type>
	class ObjectTrait
	{
	public:
		typedef std::shared_ptr<Type> Ptr;
		typedef std::shared_ptr<const Type> ConstPtr;
		typedef Type& Ref;
		typedef Type const& ConstRef;
	};
	
	template <class _Type>
	class SharedPtrObject : virtual public std::enable_shared_from_this < _Type >
	{
	public:
		virtual ~SharedPtrObject(){}

		typedef typename std::shared_ptr<_Type> SharedPtr;
	};

	template <class _Type>
	class SPACE_COMMON_API UniquePtrObject
	{
	public:
		virtual ~UniquePtrObject(){}

		//typedef typename std::unique_ptr<typename 
		//	std::enable_if<!std::is_base_of<typename SharedPtrObject<_Type>, _Type>::value,_Type>::type> UniquePtr;
	
		typedef typename std::unique_ptr<_Type> UniquePtr;

		template<class ...Args>
		static typename UniquePtr MakeUniquePtr(Args&&... args)
		{
			return std::make_unique<_Type>(std::forward<Args>(args)...);
		}
	};

	class SPACE_COMMON_API Uncopyable{
	protected:
		Uncopyable();
		virtual ~Uncopyable();
		Uncopyable(const Uncopyable&) = delete;
		Uncopyable& operator=(const Uncopyable&) = delete;
	};

	template<class Impl>
	class SPACE_COMMON_API PImpl: virtual public Uncopyable
	{
	public:
		typedef Impl ImplType;
		Impl* GetImpl()
		{
			return _impl.get();
		}
		const Impl* GetImpl() const
		{
			return _impl.get();
		}

		PImpl(const PImpl&) = delete;
		PImpl(PImpl&&) = delete;
		PImpl& operator=(const PImpl&) = delete;
		PImpl& operator=(PImpl&&) = delete;

		template<class ...Args>
		PImpl(Args&&... args)
		{
			_impl = std::make_unique<Impl>(std::forward<Args>(args)...);
		}

		PImpl(Impl* impl)
		{
			_impl.reset(impl);
		}

		virtual ~PImpl(){}
	protected:
		std::unique_ptr<Impl> _impl = nullptr;
	};

	class SPACE_COMMON_API Interface : private Uncopyable
	{
	protected:
		Interface(); 
	public:
		virtual ~Interface();
	};

	class SPACE_COMMON_API RefCountObject : public Uncopyable
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
	class SPACE_COMMON_API RefCountPtr
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

	template<class BitsType>
	class ComparableBits
	{
	public:
		bool operator<(BitsType const& other) const
		{
			for(size_t i = 0 ; i < sizeof(BitsType); ++i)
			{
				if (*(reinterpret_cast<byte const*>(this) + i) >= *(reinterpret_cast<byte const*>(&other) + i))
					return false;
			}
			return true;
		}
		
		bool operator==(BitsType const& other) const
		{
			for (size_t i = 0; i < sizeof(BitsType); ++i)
			{
				if (*(reinterpret_cast<byte const*>(this) + i) != *(reinterpret_cast<byte const*>(&other) + i))
					return false;
			}
			return true;
		}
	};

	template<class BitsType>
	class HashableBits
	{
	public:
		size_t Hash() const
		{
			byte const* _first = reinterpret_cast<byte const*>(this);
			size_t ret = 0;
			auto hasher = std::hash<byte>();
			for (auto _iter = _first; _iter < _first + sizeof(BitsType); ++_iter)
			{
				ret ^= hasher(*_iter);
			}
			return ret;
		}
	};
	
	class SPACE_COMMON_API Archiveable
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

#ifndef DEFINE_STD_HASH_SPECIALIZATION
#define DEFINE_STD_HASH_SPECIALIZATION(Hashable)                            \
namespace std {                                                             \
template<>                                                                  \
struct hash<Hashable> {                                                     \
  size_t operator()(const Hashable& obj) const {						\
    return obj.Hash();														\
  }                                                                         \
};                                                                          \
}
#endif

#endif