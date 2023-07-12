#pragma once
#include <map>
#include <memory>

template<class T>
class State
{
public:

    State(T* context)
    {
        m_context = context;
    }

    virtual ~State() {}

    // ���݃X�e�[�g���Z�b�g���鏉�����p�֐�
    template<class T>
    void SetCurrentState()
    {
        auto stateId  = typeid(T).hash_code();
        auto hasState = m_childStateMap.contains(stateId);

        if (hasState == false)
        {
            return;
        }

        m_currentState = m_childStateMap[stateId];
    }

    // ���������̓��[�g���珇��OnEnter���Ă�ł����B
    void SendEnter()
    {
        OnEnter();

        // ���݃X�e�[�g���ݒ肳��Ă���Βʒm
        if (m_currentState.expired() == false)
        {
            m_currentState.lock()->SendEnter();
        }
    }

    // �X�V���̓��[�g���珇��OnUpdate���Ă�ł����B
    void SendUpdate()
    {
        OnUpdate();

        // ���݃X�e�[�g���ݒ肳��Ă���Βʒm
        if (m_currentState.expired() == false)
        {
            m_currentState.lock()->SendUpdate();
        }
    }

    // �I�����͖��[�X�e�[�g���珇��OnExit���Ă�ł����B
    void SendExit()
    {
        // ���݃X�e�[�g���ݒ肳��Ă���Βʒm
        if (m_currentState.expired() == false)
        {
            m_currentState.lock()->SendExit();
        }

        OnExit();
    }

    // �V�����Ǘ�����q��Ԃ�ǉ�����֐�
    template<class T>
    void AddChildState()
    {
        auto stateId  = typeid(T).hash_code();
        auto hasState = m_childStateMap.contains(stateId);

        if (hasState == true)
        {
            return;
        }

        m_childStateMap[stateId] = std::make_shared<T>(m_context);
    }

    // From��Ԃ���To��Ԃ֏�ԑJ�ڂ���ݒ��o�^����֐�
    template<class From, class To>
    void AddChildTransition(int eventId)
    {
        auto fromStateId = typeid(From).hash_code();
        auto toStateId   = typeid(To).hash_code();

        // �����̏�Ԃ��o�^����Ă��邩���擾�B
        auto hasState = m_childStateMap.contains(fromStateId) && m_childStateMap.contains(toStateId);
        if (hasState == false)
        {
            return;
        }

        m_childStateMap[fromStateId]->AddTransition(eventId, toStateId);
    }

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

    // Trigger��ʒm���Đݒ���`�F�b�N���A�J�ڂ���֐�
    void SendTrigger(int eventId)
    {
        if (m_currentState.expired())
        {
            return;
        }

        // �J�ڏ������o�^����Ă����ChageState�֐����ĂԁB
        int stateId = 0;
        if (m_currentState.lock()->TryGetTransition(eventId, stateId))
        {
            ChangeState(stateId);
        }

        m_currentState.lock()->SendTrigger(eventId);
    }

protected:

    // �X�e�[�g�ɑJ�ڂ������ɌĂяo�����֐�
    virtual void OnEnter() = 0;

    // �X�e�[�g�̍X�V���Ăяo�����֐�
    virtual void OnUpdate() = 0;

    // �X�e�[�g���I�����鎞�ɌĂяo�����֐�
    virtual void OnExit() = 0;

    void ChangeState(int stateId)
    {
        // ���ɃX�e�[�g�������Ă���I���������ĂԁB
        if (m_currentState.expired() == false)
        {
            m_currentState.lock()->SendExit();
        }

        // �V�����X�e�[�g�𐶐����ď��������Ă���B
        m_currentState = m_childStateMap[stateId];
        m_currentState.lock()->SendEnter();
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