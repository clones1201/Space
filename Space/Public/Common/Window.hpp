#ifndef __SPACE_COMMON_WINDOWUTILITY_HPP__
#define __SPACE_COMMON_WINDOWUTILITY_HPP__

#include "Common/Prerequisites.hpp"
#include "Common/Basic.hpp"

namespace Space
{

	template <class _EventArgs>
	using Delegate = std::function < void(Object* const, _EventArgs&) >;
	template <class _EventArgs>
	using Event = std::vector < Delegate<_EventArgs> >;

	class SPACE_COMMON_API EventListener : public Object, public SharedPtrObject<EventListener>
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

	typedef std::shared_ptr<EventListener> EventListenerPtr;

	struct SPACE_COMMON_API MouseEventArgs
	{
		int32 X, Y, Delta;
		bool LeftButton;
		bool RightButton;
		bool MiddleButton;
	};

	class SPACE_COMMON_API MouseListener : public EventListener
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

	typedef std::shared_ptr<MouseListener> MouseListenerPtr;

	enum class SPACE_COMMON_API KeyCode
	{
		A,B,C,D,E,F
	};

	struct SPACE_COMMON_API KeyboardEventArgs
	{
		KeyCode code;
	};

	class SPACE_COMMON_API KeyboardListener : public EventListener
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
	
	typedef std::shared_ptr<KeyboardListener> KeybordListenerPtr;

	class SPACE_COMMON_API EventDispatcher : virtual public Uncopyable
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

	typedef std::shared_ptr<EventDispatcher> EventDispatcherPtr;

	
	class SPACE_COMMON_API Window : public Interface
	{
	public:
		static Window* Create(const std::string& name, int32 width, int32 height, bool fullscreen);
		static Window* Create(const std::wstring& name, int32 width, int32 height, bool fullscerrn);
		virtual ~Window();

		virtual void Show() = 0;
		
		inline int32 Width() const
		{
			return m_Width;
		}
		inline int32 Height() const
		{
			return m_Height;
		}
		inline bool IsFullscreen() const
		{
			return m_Fullscreen;
		}
				
	protected: 
		Window(const std::wstring& name, int32 width, int32 height, bool fullscreen);
		 
		EventDispatcher* m_EventDispatcher = nullptr;
		std::wstring m_Name;
		int32 m_Width, m_Height;
		bool m_Fullscreen;
		//std::function<void(*)> Resize;
	};

	typedef std::shared_ptr<Window> WindowPtr;


#if SPACE_PLATFORM == SPACE_WIN32
	class SPACE_COMMON_API Win32Window : public Window
	{
	public:
		virtual void Show();

		inline HWND GetHandle() const
		{
			return m_hWnd;
		}
		inline HDC GetDeviceContext() const
		{
			return m_hDC;
		}

		~Win32Window();
	private:
		Win32Window(std::wstring const &name,
			int32 width, int32 height, bool fullscreen);

		LRESULT CALLBACK WndProc(
			HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK MessageRouter(
			HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		
		HINSTANCE m_hInstance;
		HWND m_hWnd;
		HDC m_hDC;
		
		friend class Window;
	};
	
	typedef std::shared_ptr<Win32Window> Win32WindowPtr;
#endif
}

#endif