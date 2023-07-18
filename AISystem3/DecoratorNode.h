#pragma once
#include <functional>
#include "BaseNode.h"

class DecoratorNode : public InternalNode
{
public:

	DecoratorNode(std::function<bool()> action)
	{
		m_action = action;
	}

	void OnEnter() override
	{
		m_state = NodeState::Running;
	}

	NodeState OnUpdate() override
	{
		// �������肪�������Ă���Ύq�m�[�h���������܂��B
		if (m_action())
		{
			m_state = m_children[0]->OnUpdate();
		}
		else
		{
			// �������肪���s������I���B
			m_state = NodeState::Failure;
		}

		return m_state;
	}

	void OnExit() override
	{

	}

private:

	// ��������̏�����ێ�����B
	std::function<bool()> m_action;
};