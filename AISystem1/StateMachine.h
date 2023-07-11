#pragma once
#include <memory>
#include <map>
#include "State.h"

template<class T>
class StateMachine
{
public:

	StateMachine(T* context)
	{
		m_context = context;
	}

	// ���������̏�ԑJ�ڂ��Z�b�g����֐�
	template<class T>
	void SetStartState()
	{
		auto stateId  = typeid(T).hash_code();
		auto hasState = m_stateMap.contains(stateId);

		if (hasState == false)
		{
			return;
		}

		// ��Ԃ��o�^����Ă���ΑJ��
		ChangeState(stateId);
	}

	// ���݃X�e�[�g��ێ����Ă���΍X�V����B
	void Update()
	{
		if (m_currentState.expired() == false)
		{
			m_currentState.lock()->OnUpdate();
		}
	}

	// �V�����Ǘ������Ԃ�ǉ�����֐�
	template<class T>
	void AddState()
	{
		auto stateId  = typeid(T).hash_code();
		auto hasState = m_stateMap.contains(stateId);

		if (hasState == true)
		{
			return;
		}

		m_stateMap[stateId] = std::make_shared<T>(m_context);
	}

	// From��Ԃ���To��Ԃ֏�ԑJ�ڂ���ݒ��o�^����֐�
	template<class From, class To>
	void AddTransition(int eventId)
	{
		auto fromStateId = typeid(From).hash_code();
		auto toStateId   = typeid(To).hash_code();

		// �����̏�Ԃ��o�^����Ă��邩���擾�B
		auto hasState = m_stateMap.contains(fromStateId) && m_stateMap.contains(toStateId);
		if (hasState == false)
		{
			return;
		}

		m_stateMap[fromStateId]->AddTransition(eventId, toStateId);
	}

	void SendTrigger(int eventId)
	{
		// �J�ڏ������o�^����Ă����ChageState�֐����ĂԁB
		int stateId = 0;
		if (m_currentState.lock()->TryGetTransition(eventId, stateId))
		{
			ChangeState(stateId);
		}
	}

private:

	void ChangeState(int stateId)
	{
		// ���ɃX�e�[�g�������Ă���I���������ĂԁB
		if (m_currentState.expired() == false)
		{
			m_currentState.lock()->OnExit();
		}

		// �V�����X�e�[�g�𐶐����ď��������Ă���B
		m_currentState = m_stateMap[stateId];
		m_currentState.lock()->OnEnter();
	}

private:

	// ���̃N���X��ێ�����I�u�W�F�N�g�iPlayer�Ƃ��j
	T* m_context;

	// ���݃Z�b�g���ꂽ�X�e�[�g�I�u�W�F�N�g
	std::weak_ptr<State<T>> m_currentState;

	// �e��Ԃ�ID�ƃI�u�W�F�N�g�ւ̃}�b�v
	std::map<int, std::shared_ptr<State<T>>> m_stateMap;
};