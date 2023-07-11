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

	// 初期化時の状態遷移をセットする関数
	template<class T>
	void SetStartState()
	{
		auto stateId  = typeid(T).hash_code();
		auto hasState = m_stateMap.contains(stateId);

		if (hasState == false)
		{
			return;
		}

		// 状態が登録されていれば遷移
		ChangeState(stateId);
	}

	// 現在ステートを保持していれば更新する。
	void Update()
	{
		if (m_currentState.expired() == false)
		{
			m_currentState.lock()->OnUpdate();
		}
	}

	// 新しく管理する状態を追加する関数
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

	// From状態からTo状態へ状態遷移する設定を登録する関数
	template<class From, class To>
	void AddTransition(int eventId)
	{
		auto fromStateId = typeid(From).hash_code();
		auto toStateId   = typeid(To).hash_code();

		// 両方の状態が登録されているかを取得。
		auto hasState = m_stateMap.contains(fromStateId) && m_stateMap.contains(toStateId);
		if (hasState == false)
		{
			return;
		}

		m_stateMap[fromStateId]->AddTransition(eventId, toStateId);
	}

	void SendTrigger(int eventId)
	{
		// 遷移条件が登録されていればChageState関数を呼ぶ。
		int stateId = 0;
		if (m_currentState.lock()->TryGetTransition(eventId, stateId))
		{
			ChangeState(stateId);
		}
	}

private:

	void ChangeState(int stateId)
	{
		// 既にステートが入ってたら終了処理を呼ぶ。
		if (m_currentState.expired() == false)
		{
			m_currentState.lock()->OnExit();
		}

		// 新しくステートを生成して初期化している。
		m_currentState = m_stateMap[stateId];
		m_currentState.lock()->OnEnter();
	}

private:

	// このクラスを保持するオブジェクト（Playerとか）
	T* m_context;

	// 現在セットされたステートオブジェクト
	std::weak_ptr<State<T>> m_currentState;

	// 各状態のIDとオブジェクトへのマップ
	std::map<int, std::shared_ptr<State<T>>> m_stateMap;
};