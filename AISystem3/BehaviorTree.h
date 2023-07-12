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
		// ���ɑS�Ă̏������I�����Ă���B
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

	// �؍\���̃��[�g�m�[�h��ێ����܂��B
	std::shared_ptr<Node> m_root;

	// ���̃c���[�̏�Ԃ�ێ����܂��B
	NodeState m_state;
};