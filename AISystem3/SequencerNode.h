#pragma once
#include "BaseNode.h"

class SequencerNode : public BranchNode
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
		// 実行中か失敗が返されたら終了します。
		for (auto i = m_childIndex; i < m_children.size(); m_childIndex++)
		{
			auto child = m_children[i];
			auto state = child->OnUpdate();

			// 成功したら次の子ノードへ
			if (state != NodeState::Success)
			{
				return state;
			}
		}

		// 全ての子ノードが成功すれば終了。
		return NodeState::Success;
	}

	void OnExit() override
	{

	}

private:

	// 現在処理されている子ノードインデックス。
	int m_childIndex;
};