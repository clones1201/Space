#include "Event.hpp"

namespace Space
{
 

	void KeyboardListener::RaisePressedEvent(KeyCode code)
	{
		try
		{
			m_IsPressedMap.at(code) = true;			
			RaiseEvent(KeyPressed, KeyboardEventArgs{ code });
		}
		catch (std::out_of_range)
		{
		}
	}
	void KeyboardListener::RaiseReleaseEvent(KeyCode code)
	{
		try
		{
			m_IsPressedMap.at(code) = false;
			RaiseEvent(KeyReleased, KeyboardEventArgs{ code });
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

	void EventDispatcher::RaisePressedEvent(KeyCode code)
	{
		for (std::shared_ptr<EventListener> ptr : m_ListenerArray)
		{
			if (EventListener::Type::Keyboard == ptr->GetType())
			{
				std::static_pointer_cast<KeyboardListener>(ptr)
					->RaisePressedEvent(code);
			}
		}
	}
	void EventDispatcher::RaiseReleaseEvent(KeyCode code)
	{
		for (std::shared_ptr<EventListener> ptr : m_ListenerArray)
		{
			if (EventListener::Type::Keyboard == ptr->GetType())
			{
				std::static_pointer_cast<KeyboardListener>(ptr)
					->RaiseReleaseEvent(code);
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