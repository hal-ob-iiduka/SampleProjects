#pragma once

template<class T>
class State
{
public:

    State(T* context)
    {
        m_context = context;
    }

    virtual ~State() {}

    // �X�e�[�g�ɑJ�ڂ������ɌĂяo�����֐�
    virtual void OnEnter() = 0;

    // �X�e�[�g�̍X�V���Ăяo�����֐�
    virtual void OnUpdate() = 0;

    // �X�e�[�g���I�����鎞�ɌĂяo�����֐�
    virtual void OnExit() = 0;

    // ��ԑJ�ڂ̏�����ǉ�����֐�
    // ���̃I�u�W�F�N�g����̑J�ڏ���������ǉ�����B
    void AddTransition(int eventId, int stateId)
    {
        m_transitionMap[eventId] = stateId;
    }

    // ��ԑJ�ڂ̏������ݒ肳��Ă���ΑJ�ڐ��stateId��Ԃ��܂��B
    bool TryGetTransition(int eventId, int& stateId)
    {
        auto hasEvent = m_transitionMap.contains(eventId);
        if (hasEvent == true)
        {
            stateId = m_transitionMap[eventId];
        }
        return hasEvent;
    }

private:

    T* m_context;

    // ���݃Z�b�g���ꂽ�q���X�e�[�g�I�u�W�F�N�g
    std::weak_ptr<State<T>> m_currentState;

    // �e��ԑJ�ڃg���K�[�ƑJ�ڐ�̏��ID�ւ̃}�b�v
    std::map<int, int> m_transitionMap;

    // �e��Ԃ�ID�Ǝq���X�e�[�g�I�u�W�F�N�g�ւ̃}�b�v
    std::map<int, std::shared_ptr<State<T>>> m_childStateMap;
};