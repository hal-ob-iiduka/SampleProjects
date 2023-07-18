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
		// �q�m�[�h�����ɍX�V���Ă����܂��B
		// ���s�����������Ԃ��ꂽ��I�����܂��B
		for (auto& i = m_childIndex; i < m_children.size(); m_childIndex++)
		{
			auto child = m_children[i];
			auto state = child->OnUpdate();

			// ���s�����玟�̎q�m�[�h��
			if (state != NodeState::Failure)
			{
				return state;
			}
		}

		// �S�Ă̎q�m�[�h�����s����ΏI���B
		return NodeState::Failure;
	}

	void OnExit() override
	{

	}

private:

	// ���ݏ�������Ă���q�m�[�h�C���f�b�N�X�B
	int m_childIndex;
};










