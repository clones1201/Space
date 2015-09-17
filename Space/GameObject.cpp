#include "GameObject.hpp"

namespace Space
{
	GameObject * GameObject::AddChildren()
	{
		GameObjectPtr pNewChild(new GameObject);
		m_Children.push_back(pNewChild);
		return pNewChild.get();
	}


	void GameObject::RemoveChildren(GameObject* pChild)
	{
		
	}
	
}