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
		// �q�m�[�h�����ɍX�V���Ă����܂��B
		// ���s�������s���Ԃ��ꂽ��I�����܂��B
		for (auto i = m_childIndex; i < m_children.size(); m_childIndex++)
		{
			auto child = m_children[i];
			auto state = child->OnUpdate();

			// ���������玟�̎q�m�[�h��
			if (state != NodeState::Success)
			{
				return state;
			}
		}

		// �S�Ă̎q�m�[�h����������ΏI���B
		return NodeState::Success;
	}

	void OnExit() override
	{

	}

private:

	// ���ݏ�������Ă���q�m�[�h�C���f�b�N�X�B
	int m_childIndex;
};