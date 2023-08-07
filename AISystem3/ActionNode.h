#pragma once
#include <functional>
#include "BaseNode.h"

class ActionNode : public LeafNode
{
public:

	ActionNode(std::function<NodeState()>&& action)
	{
		m_action = action;
	}

	void OnEnter() override
	{
		m_state = NodeState::Running;
	}

	NodeState OnUpdate() override
	{
		// �ݒ肳�ꂽ�����̖߂�l�����̂܂ܕԂ��܂��B
		return m_action();
	}

	void OnExit() override
	{

	}

private:

	// ���ۂ̐U�镑��������ێ�����B
	std::function<NodeState()> m_action;
};