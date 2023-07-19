#pragma once
#include <functional>
#include "BaseNode.h"

class ParallelNode : public InternalNode
{
public:

	void OnEnter() override
	{
		m_state = NodeState::Running;
	}

	NodeState OnUpdate() override
	{
		bool allSucceeded = true;
		for (auto child : m_children)
		{
			// 既に終了しているノードはスキップ
			if (child->IsFinished())
			{
				continue;
			}

			auto state = child->Execute();

			// 子ノードが一つでも失敗すれば終了。
			if (state == NodeState::Failure)
			{
				return NodeState::Failure;
			}

			// まだ実行中のものがあれば終了しない。
			if (state == NodeState::Running)
			{
				allSucceeded = false;
			}
		}

		// 全ての子ノードが成功すれば終了。
		if (allSucceeded)
		{
			return NodeState::Success;
		}

		return NodeState::Running;
	}

	void OnExit()
	{

	}
};