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

    // 現在ステートをセットする初期化用関数
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

    // 初期化時はルートから順にOnEnterを呼んでいく。
    void SendEnter()
    {
        OnEnter();

        // 現在ステートが設定されていれば通知
        if (m_currentState.expired() == false)
        {
            m_currentState.lock()->SendEnter();
        }
    }

    // 更新時はルートから順にOnUpdateを呼んでいく。
    void SendUpdate()
    {
        OnUpdate();

        // 現在ステートが設定されていれば通知
        if (m_currentState.expired() == false)
        {
            m_currentState.lock()->SendUpdate();
        }
    }

    // 終了時は末端ステートから順にOnExitを呼んでいく。
    void SendExit()
    {
        // 現在ステートが設定されていれば通知
        if (m_currentState.expired() == false)
        {
            m_currentState.lock()->SendExit();
        }

        OnExit();
    }

    // 新しく管理する子状態を追加する関数
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

    // From状態からTo状態へ状態遷移する設定を登録する関数
    template<class From, class To>
    void AddChildTransition(int eventId)
    {
        auto fromStateId = typeid(From).hash_code();
        auto toStateId   = typeid(To).hash_code();

        // 両方の状態が登録されているかを取得。
        auto hasState = m_childStateMap.contains(fromStateId) && m_childStateMap.contains(toStateId);
        if (hasState == false)
        {
            return;
        }

        m_childStateMap[fromStateId]->AddTransition(eventId, toStateId);
    }

    // 状態遷移の条件を追加する関数
    // このオブジェクトからの遷移条件だけを追加する。
    void AddTransition(int eventId, int stateId)
    {
        m_transitionMap[eventId] = stateId;
    }

    // 状態遷移の条件が設定されていれば遷移先のstateIdを返します。
    bool TryGetTransition(int eventId, int& stateId)
    {
        auto hasEvent = m_transitionMap.contains(eventId);
        if (hasEvent == true)
        {
            stateId = m_transitionMap[eventId];
        }
        return hasEvent;
    }

    // Triggerを通知して設定をチェックし、遷移する関数
    void SendTrigger(int eventId)
    {
        if (m_currentState.expired())
        {
            return;
        }

        // 遷移条件が登録されていればChageState関数を呼ぶ。
        int stateId = 0;
        if (m_currentState.lock()->TryGetTransition(eventId, stateId))
        {
            ChangeState(stateId);
        }

        m_currentState.lock()->SendTrigger(eventId);
    }

protected:

    // ステートに遷移した時に呼び出される関数
    virtual void OnEnter() = 0;

    // ステートの更新中呼び出される関数
    virtual void OnUpdate() = 0;

    // ステートが終了する時に呼び出される関数
    virtual void OnExit() = 0;

    void ChangeState(int stateId)
    {
        // 既にステートが入ってたら終了処理を呼ぶ。
        if (m_currentState.expired() == false)
        {
            m_currentState.lock()->SendExit();
        }

        // 新しくステートを生成して初期化している。
        m_currentState = m_childStateMap[stateId];
        m_currentState.lock()->SendEnter();
    }

private:

    T* m_context;

    // 現在セットされた子供ステートオブジェクト
    std::weak_ptr<State<T>> m_currentState;

    // 各状態遷移トリガーと遷移先の状態IDへのマップ
    std::map<int, int> m_transitionMap;

    // 各状態のIDと子供ステートオブジェクトへのマップ
    std::map<int, std::shared_ptr<State<T>>> m_childStateMap;
};