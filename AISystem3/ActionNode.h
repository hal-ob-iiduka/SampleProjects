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
		// İ’è‚³‚ê‚½ˆ—‚Ì–ß‚è’l‚ğ‚»‚Ì‚Ü‚Ü•Ô‚µ‚Ü‚·B
		return m_action();
	}

	void OnExit() override
	{

	}

private:

	// ÀÛ‚ÌU‚é•‘‚¢ˆ—‚ğ•Û‚·‚éB
	std::function<NodeState()> m_action;
};