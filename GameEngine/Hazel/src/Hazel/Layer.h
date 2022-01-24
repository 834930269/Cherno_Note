#pragma once

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"

namespace Hazel {
	/// <summary>
	/// ������˵��ʲô�ǲ�
	/// ����Ϸ������,���ǿ��ܻᴥ���¼�,
	/// ���༭�������¼�,�����п��ܲ����������뵽ʵʱ�༭����Ϸ��
	/// ��֮ͬ��
	/// ������Ͳ�����һ��Ч��
	/// ������Ҫ�ѱ༭�����зֲ�
	/// ����ÿ�㶼��һ��LayerStack
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



