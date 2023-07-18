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
		// 条件判定が成功していれば子ノードを処理します。
		if (m_action())
		{
			m_state = m_children[0]->OnUpdate();
		}
		else
		{
			// 条件判定が失敗したら終了。
			m_state = NodeState::Failure;
		}

		return m_state;
	}

	void OnExit() override
	{

	}

private:

	// 条件判定の処理を保持する。
	std::function<bool()> m_action;
};