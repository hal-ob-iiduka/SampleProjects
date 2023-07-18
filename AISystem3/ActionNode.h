#pragma once
#include <functional>
#include "BaseNode.h"

class ActionNode : public LeafNode
{
public:

	ActionNode(std::function<NodeState()> action)
	{
		m_action = action;
	}

	void OnEnter() override
	{
		m_state = NodeState::Running;
	}

	NodeState OnUpdate() override
	{
		// 設定された処理の戻り値をそのまま返します。
		return m_state = m_action();
	}

	void OnExit() override
	{

	}

private:

	// 実際の行動処理を保持する。
	std::function<NodeState()> m_action;
};










