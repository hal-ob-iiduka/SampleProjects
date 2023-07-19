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
		if (m_action())
		{
			// 条件判定が成功していれば、子ノードを処理しそのまま結果を返します。
			return m_children[0]->Execute();
		}
		else
		{
			// 条件判定が失敗したら終了。
			return NodeState::Failure;
		}
	}

	void OnExit() override
	{

	}

private:

	// 条件判定の処理を保持する。
	std::function<bool()> m_action;
};