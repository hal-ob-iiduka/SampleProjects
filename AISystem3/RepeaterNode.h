#pragma once
#include <functional>
#include "BaseNode.h"

class RepeaterNode : public InternalNode
{
public:

	RepeaterNode(int repeatCount)
	{
		m_repeatCount = 0;
		m_maxRepeatCount = repeatCount;
	}

	void OnEnter() override
	{
		m_state = NodeState::Running;
		m_repeatCount = 0;
	}

	NodeState OnUpdate() override
	{
		auto state = m_children[0]->Execute();

		// 実行中ならそのまま返す。
		if (state == NodeState::Running)
		{
			return state;
		}

		// 処理が終了した場合だと、リピート回数を超えたら終了する。
		++m_repeatCount;
		if (m_repeatCount == m_maxRepeatCount)
		{
			return NodeState::Success;
		}

		// リピート中なら子ノードをリセットして更新準備をする。
		m_children[0]->Reset();
	}

	void OnExit() override
	{

	}

private:

	// 現在のリピート回数を保持する。
	int m_repeatCount;

	// 最大リピート回数を保持する。
	int m_maxRepeatCount;
};