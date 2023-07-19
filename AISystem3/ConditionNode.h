#pragma once
#include <functional>
#include "BaseNode.h"

class ConditionNode : public LeafNode
{
public:

	ConditionNode(std::function<bool()> action)
	{
		m_action = action;
	}

	void OnEnter() override
	{
		m_state = NodeState::Running;
	}

	NodeState OnUpdate() override
	{
		// �������肪���������s�����A�񋓌^�ɕϊ����ĕԂ��܂��B
		return m_action() ? NodeState::Success : NodeState::Failure;
	}

	void OnExit() override
	{

	}

private:

	// ��������̏�����ێ�����B
	std::function<bool()> m_action;
};