#pragma once
#include <functional>
#include "BaseNode.h"

class DecoratorNode : public InternalNode
{
public:

	DecoratorNode(std::function<bool()>&& action)
	{
		m_action = action;
	}

	void OnEnter() override
	{
		m_state = NodeState::Running;
	}

	NodeState OnUpdate() override
	{
		if (m_action())
		{
			// �������肪�������Ă���΁A�q�m�[�h�����������̂܂܌��ʂ�Ԃ��܂��B
			return m_children[0]->Execute();
		}
		else
		{
			// �������肪���s������I���B
			return NodeState::Failure;
		}
	}

	void OnExit() override
	{

	}

private:

	// ��������̏�����ێ�����B
	std::function<bool()> m_action;
};