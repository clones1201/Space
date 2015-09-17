#ifndef __SPACE_SCENE_HPP__
#define __SPACE_SCENE_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"
#include "Utility.hpp"
#include "GameObject.hpp"

namespace Space
{ 
	
	class SceneManager : virtual public Uncopyable
	{
	public:
		typedef std::list<GameObjectPtr> GArray;
		typedef std::list<GameObjectPtr>::iterator GArrayIterator;
		typedef std::list<GameObjectPtr>::const_iterator GArrayConstIterator;

		/*
		Archiver& operator<<(Archiver& archiver);
		Archiver& operator>>(Archiver& archiver);
		*/
		
		void Tick(float elaspTime);

	protected:
		
	private:
		GArray m_GameObjects;

	};
}

#endif