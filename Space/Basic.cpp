#include "Basic.hpp"
#include <atomic>

namespace Space
{
	Object::~Object(){}

	Uncopyable::Uncopyable(){}
	Uncopyable::~Uncopyable(){}
	//Uncopyable::Uncopyable(const Uncopyable&){}
	//Uncopyable& Uncopyable::operator=(const Uncopyable&){ return *this; }

	Interface::Interface(){}
	Interface::~Interface(){}

	RefCountObject::RefCountObject()
	{}

	RefCountObject::~RefCountObject()
	{}

	int32 RefCountObject::AddRef()
	{
		return m_RefCount++;
	}

	int32 RefCountObject::Release()
	{
		int32 ret = m_RefCount--;
		if (m_RefCount < 0)
		{
			assert(this != nullptr);
			delete this;
		}
		return ret;
	}

	Archiveable::~Archiveable(){}
}