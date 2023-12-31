#pragma once
#include <functional>
#include "BaseNode.h"

class ConditionNode : public LeafNode
{
public:

	ConditionNode(std::function<bool()>&& action)
	{
		m_action = action;
	}

	void OnEnter() override
	{
		m_state = NodeState::Running;
	}

	NodeState OnUpdate() override
	{
		// 条件判定が成功か失敗かを、列挙型に変換して返します。
		return m_action() ? NodeState::Success : NodeState::Failure;
	}

	void OnExit() override
	{

	}

private:

	// 条件判定の処理を保持する。
	std::function<bool()> m_action;
};