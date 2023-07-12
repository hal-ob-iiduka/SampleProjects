#pragma once
#include <memory>
#include "BaseNode.h"

class BehaviorTree
{
public:

	void SetRootNode(std::shared_ptr<Node> root)
	{
		m_root = root;
	}

	void Start()
	{
		m_state = NodeState::Running;
	}

	void Update()
	{
		// 既に全ての処理が終了している。
		if (IsFinished())
		{
			return;
		}

		if (m_root)
		{
			m_state = m_root->Execute();
		}
	}

	bool IsSuccess() { return m_state == NodeState::Success; }

	bool IsFailure() { return m_state == NodeState::Failure; }

	bool IsFinished() { return IsSuccess() || IsFailure(); }

private:

	// 木構造のルートノードを保持します。
	std::shared_ptr<Node> m_root;

	// このツリーの状態を保持します。
	NodeState m_state;
};