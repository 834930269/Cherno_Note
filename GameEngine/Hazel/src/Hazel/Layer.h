#pragma once

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"

namespace Hazel {
	/// <summary>
	/// 在这里说下什么是层
	/// 在游戏引擎中,我们可能会触发事件,
	/// 而编辑器触发事件,我们有可能不想让它进入到实时编辑的游戏中
	/// 反之同样
	/// 所以这就产生了一个效果
	/// 我们需要把编辑器进行分层
	/// 对于每层都有一个LayerStack
	/// </summary>
	class HAZEL_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}
		
		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}



