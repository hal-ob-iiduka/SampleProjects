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

    // ステートに遷移した時に呼び出される関数
    virtual void OnEnter() = 0;

    // ステートの更新中呼び出される関数
    virtual void OnUpdate() = 0;

    // ステートが終了する時に呼び出される関数
    virtual void OnExit() = 0;

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

private:

    T* m_context;

    // 現在セットされた子供ステートオブジェクト
    std::weak_ptr<State<T>> m_currentState;

    // 各状態遷移トリガーと遷移先の状態IDへのマップ
    std::map<int, int> m_transitionMap;

    // 各状態のIDと子供ステートオブジェクトへのマップ
    std::map<int, std::shared_ptr<State<T>>> m_childStateMap;
};