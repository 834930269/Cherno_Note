#pragma once

#include "Hazel/Core.h"

#include <string>
#include <functional>
#include <sstream>

namespace Hazel {
	/// <summary>
	/// HazelEngine被阻塞到当前的事件上,即意味着事件系统必须有效且即时的处理和分发这个事件
	/// 后面Hazel会把单个阻塞改为`buffer`式的事件流
	/// </summary>
	enum class EventType {
		None = 0,
		WindowClose,WindowResize,WindowFocus,WindowLostFocus,WindowMoved,
		AppTick,AppUpdate,AppRender,
		KeyPressed,KeyReleased,
		MouseButtonPressed,MouseButtonReleased,MouseMoved,MouseScrolled
	};

	///事件的过滤方式
	enum EventCategory {
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput       = BIT(1),
		EventCategoryKeyboard    = BIT(2),
		EventCategoryMouse       = BIT(3),
		EventCategoryMouseButton = BIT(4),
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override {return category;}

	class HAZEL_API Event {
		friend class EventDispatcher;
	public:
	
		bool Handled = false;
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		/// <summary>
		/// 是否是在当前组内的状态
		/// </summary>
		/// <param name="category"></param>
		/// <returns></returns>
		inline bool IsInCategory(EventCategory category) {
			return GetCategoryFlags() & category;
		}
	};

	/// <summary>
	/// 事件分发
	/// </summary>
	class EventDispatcher {
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event) : m_Event(event){
		}

		template<typename T>
		bool Dispatch(EventFn<T> func) {
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled |= func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}

}