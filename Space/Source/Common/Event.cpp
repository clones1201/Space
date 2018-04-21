#include "Common/Prerequisites.hpp"
#include "Common/Window.hpp"

namespace Space
{
	void KeyboardListener::RaisePressedEvent(KeyboardEventArgs e)
	{
		try
		{
			m_IsPressedMap.at(e.code) = true;			
			RaiseEvent(KeyPressed, e);
		}
		catch (std::out_of_range)
		{
		}
	}
	void KeyboardListener::RaiseReleaseEvent(KeyboardEventArgs e)
	{
		try
		{
			m_IsPressedMap.at(e.code) = false;
			RaiseEvent(KeyReleased, e);
		}
		catch (std::out_of_range)
		{
		}
	}

	void EventDispatcher::AddEventListener(EventListener* listener)
	{
		m_ListenerArray.push_back(listener->shared_from_this());
	}
	void EventDispatcher::RemoveEventListener(EventListener* listener)
	{
		m_ListenerArray.erase(
			std::find(m_ListenerArray.begin(), m_ListenerArray.end(), 
				listener->shared_from_this())
			);
	}

	void EventDispatcher::RaisePressedEvent(KeyboardEventArgs e)
	{
		for (std::shared_ptr<EventListener> ptr : m_ListenerArray)
		{
			if (EventListener::Type::Keyboard == ptr->GetType())
			{
				std::static_pointer_cast<KeyboardListener>(ptr)
					->RaisePressedEvent(e);
			}
		}
	}
	void EventDispatcher::RaiseReleaseEvent(KeyboardEventArgs e)
	{
		for (std::shared_ptr<EventListener> ptr : m_ListenerArray)
		{
			if (EventListener::Type::Keyboard == ptr->GetType())
			{
				std::static_pointer_cast<KeyboardListener>(ptr)
					->RaiseReleaseEvent(e);
			}
		}
	}

	void EventDispatcher::RaiseMouseDownEvent(MouseEventArgs e)
	{
		for (std::shared_ptr<EventListener> ptr : m_ListenerArray)
		{
			if (EventListener::Type::Mouse == ptr->GetType())
			{
				std::static_pointer_cast<MouseListener>(ptr)
					->RaiseMouseDownEvent(e);
			}
		}
	}
	void EventDispatcher::RaiseMouseUpEvent(MouseEventArgs e)
	{
		for (std::shared_ptr<EventListener> ptr : m_ListenerArray)
		{
			if (EventListener::Type::Mouse == ptr->GetType())
			{
				std::static_pointer_cast<MouseListener>(ptr)
					->RaiseMouseUpEvent(e);
			}
		}
	}
	void EventDispatcher::RaiseMouseMoveEvent(MouseEventArgs e)
	{
		for (std::shared_ptr<EventListener> ptr : m_ListenerArray)
		{
			if (EventListener::Type::Mouse == ptr->GetType())
			{
				std::static_pointer_cast<MouseListener>(ptr)
					->RaiseMouseMoveEvent(e);
			}
		}
	}
	void EventDispatcher::RaiseMouseScrollEvent(MouseEventArgs e)
	{
		for (std::shared_ptr<EventListener> ptr : m_ListenerArray)
		{
			if (EventListener::Type::Mouse == ptr->GetType())
			{
				std::static_pointer_cast<MouseListener>(ptr)
					->RaiseMouseScrollEvent(e);
			}
		}
	}

}