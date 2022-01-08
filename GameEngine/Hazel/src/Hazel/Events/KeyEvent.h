#pragma once

#include "Event.h"


namespace Hazel {
	/// <summary>
	/// KeyEvent是事件的子类
	/// 也是所有keycode事件的基类
	/// </summary>
	class HAZEL_API KeyEvent : public Event {
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(int keycode)
			:m_KeyCode(keycode){}
		int m_KeyCode;
	};

	/// <summary>
	/// 长按Key时的派生类
	/// </summary>
	class HAZEL_API KeyPressedEvent :public KeyEvent {
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount) {}
		inline int GetRepeatCount() const { return m_RepeatCount; }
		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int m_RepeatCount;
	};

	/// <summary>
	/// 释放Key时触发的事件
	/// </summary>
	class HAZEL_API KeyReleasedEvent :public KeyEvent {
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {}
		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};
}