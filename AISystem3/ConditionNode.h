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
		// ğŒ”»’è‚ª¬Œ÷‚©¸”s‚©‚ğA—ñ‹“Œ^‚É•ÏŠ·‚µ‚Ä•Ô‚µ‚Ü‚·B
		return m_action() ? NodeState::Success : NodeState::Failure;
	}

	void OnExit() override
	{

	}

private:

	// ğŒ”»’è‚Ìˆ—‚ğ•Û‚·‚éB
	std::function<bool()> m_action;
};