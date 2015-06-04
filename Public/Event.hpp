#ifndef __SPACE_EVENT_HPP__
#define __SPACE_EVENT_HPP__

#include "Prerequisites.hpp"
#include "Basic.hpp"

namespace Space
{	  
	template <class _EventArgs>
	using Delegate = std::function < void(Object* const, _EventArgs&) >;
	template <class _EventArgs>
	using Event = std::vector < Delegate<_EventArgs> >;

	class SPACE_API EventListener : public Object, public SharedPtrObject<EventListener>
	{
	public:
		enum class Type
		{
			Unknown,
			Mouse,
			Keyboard
		};

		Type GetType() const
		{
			return m_Type;
		}

		template<class _EventArgs>
		inline void RaiseEvent(Event<_EventArgs> &e,_EventArgs args)
		{
			for (Delegate<_EventArgs> Del : e)
			{
				Del(this, args);
			}
		}

	protected:
		EventListener(Type type)
			:m_Type(type){}

		Type m_Type = Type::Unknown;

	};

	struct SPACE_API MouseEventArgs
	{
		int32 X, Y, Delta;
		bool LeftButton;
		bool RightButton;
		bool MiddleButton;
	};

	class SPACE_API MouseListener : public EventListener
	{
	public:
		Event<MouseEventArgs> MouseDown;
		Event<MouseEventArgs> MouseUp;
		Event<MouseEventArgs> MouseMove;
		Event<MouseEventArgs> MouseScroll;

		inline void RaiseMouseDownEvent(MouseEventArgs e)
		{
			RaiseEvent(MouseScroll, std::forward<MouseEventArgs>(e));
		}
		inline void RaiseMouseUpEvent(MouseEventArgs e)
		{
			RaiseEvent(MouseScroll, std::forward<MouseEventArgs>(e));
		}
		inline void RaiseMouseMoveEvent(MouseEventArgs e)
		{
			RaiseEvent(MouseScroll, std::forward<MouseEventArgs>(e));
		}
		inline void RaiseMouseScrollEvent(MouseEventArgs e)
		{
			RaiseEvent(MouseScroll, std::forward<MouseEventArgs>(e));
		}

	protected:
		MouseListener()
			:EventListener(Type::Mouse){}
	private:

	};

	enum class SPACE_API KeyCode
	{
		A,B,C,D,E,F
	};

	struct SPACE_API KeyboardEventArgs
	{
		KeyCode code;
	};

	class SPACE_API KeyboardListener : public EventListener
	{
	public:
		Event<KeyboardEventArgs> KeyPressed;
		Event<KeyboardEventArgs> KeyReleased;

		inline bool IsPressed(KeyCode code) const
		{
			return m_IsPressedMap.at(code);
		}
		
		void RaisePressedEvent(KeyCode code);
		void RaiseReleaseEvent(KeyCode code);
	private:
		KeyboardListener()
			:EventListener(Type::Keyboard){}
		std::map<KeyCode, bool> m_IsPressedMap;
	};
	
	class EventDispatcher : virtual public Uncopyable
	{
	public:
		void AddEventListener(EventListener* listener);
		void RemoveEventListener(EventListener* listener);


		void RaisePressedEvent(KeyCode code);
		void RaiseReleaseEvent(KeyCode code);

		void RaiseMouseDownEvent(MouseEventArgs e);
		void RaiseMouseUpEvent(MouseEventArgs e);
		void RaiseMouseMoveEvent(MouseEventArgs e);
		void RaiseMouseScrollEvent(MouseEventArgs e);

	private:
		EventDispatcher(){}

		std::vector<std::shared_ptr<EventListener>> m_ListenerArray;
		friend class Core;
	};

}

#endif