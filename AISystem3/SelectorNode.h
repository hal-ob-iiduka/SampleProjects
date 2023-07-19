#pragma once
#include "BaseNode.h"

class SelectorNode : public InternalNode
{
public:

	void OnEnter() override
	{
		m_state = NodeState::Running;
		m_childIndex = 0;
	}

	NodeState OnUpdate() override
	{
		// 子ノードを順に更新していきます。
		// 実行中か成功が返されたら終了します。
		for (auto& i = m_childIndex; i < m_children.size(); m_childIndex++)
		{
			auto child = m_children[i];
			auto state = child->Execute();

			// 失敗したら次の子ノードへ
			if (state != NodeState::Failure)
			{
				return state;
			}
		}

		// 全ての子ノードが失敗すれば終了。
		return NodeState::Failure;
	}

	void OnExit() override
	{

	}

private:

	// 現在処理されている子ノードインデックス。
	int m_childIndex;
};