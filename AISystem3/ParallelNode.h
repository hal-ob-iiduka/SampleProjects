#pragma once
#include <functional>
#include "BaseNode.h"

class ParallelNode : public InternalNode
{
public:

	void OnEnter()
	{

	}

	NodeState OnUpdate() override
	{
		bool allSucceeded = true;
		for (auto child : m_children)
		{
			// ���ɏI�����Ă���m�[�h�̓X�L�b�v
			if (child->IsFinished())
			{
				continue;
			}

			auto state = child->Execute();

			// �q�m�[�h����ł����s����ΏI���B
			if (state == NodeState::Failure)
			{
				return NodeState::Failure;
			}

			// �܂����s���̂��̂�����ΏI�����Ȃ��B
			if (state == NodeState::Running)
			{
				allSucceeded = false;
			}
		}

		// �S�Ă̎q�m�[�h����������ΏI���B
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