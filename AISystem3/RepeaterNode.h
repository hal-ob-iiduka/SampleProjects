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

		// ���s���Ȃ炻�̂܂ܕԂ��B
		if (state == NodeState::Running)
		{
			return state;
		}

		// �������I�������ꍇ���ƁA���s�[�g�񐔂𒴂�����I������B
		++m_repeatCount;
		if (m_repeatCount == m_maxRepeatCount)
		{
			return NodeState::Success;
		}

		// ���s�[�g���Ȃ�q�m�[�h�����Z�b�g���čX�V����������B
		m_children[0]->Reset();
	}

	void OnExit() override
	{

	}

private:

	// ���݂̃��s�[�g�񐔂�ێ�����B
	int m_repeatCount;

	// �ő僊�s�[�g�񐔂�ێ�����B
	int m_maxRepeatCount;
};