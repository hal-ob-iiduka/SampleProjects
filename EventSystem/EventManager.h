#pragma once
#include <iostream>
#include <functional>
#include <set>
#include <map>
#include <list>
#include <array>
#include <any>
#include "Stopwatch.h"

class EventListener
{
public:

	void Action(std::any data)
	{
		func(data);
	}

	void SetFunction(int InEventId, std::function<void(std::any)> InFunc)
	{
		eventId = InEventId;
		func = InFunc;
	}

	void RegisterToEventManager();
	void UnRegisterFromEventManager();

private:

	int eventId;

	std::function<void(std::any)> func;
};

class EventManager
{
public:

	static EventManager& Get() noexcept
	{
		static EventManager instance;
		return instance;
	}

	void Invoke(int id, std::any data)
	{
		auto hasEvent = EventListenerMap.contains(id);
		if (hasEvent == false)
		{
			return;
		}

		EventQueues[NumActiveQueue].push_back(EventData(id, data));
	}

	void PreTick()
	{
		Stopwatch.Start();
	}

	void Tick()
	{
		// キューの入れ替え
		const auto numQueue = NumActiveQueue;
		(++NumActiveQueue) %= EventQueues.max_size();

		while (EventQueues[numQueue].size() != 0)
		{
			// 先頭から登録されたイベントデータを取得する。
			auto& event = EventQueues[numQueue].front();

			// 指定されたイベントを実行する。
			const auto& listeners = EventListenerMap[event.id];
			for (auto listener : listeners)
			{
				listener->Action(event.data);
			}

			EventQueues[numQueue].pop_front();

			// タイムアウトしているかをチェック
			// 今回は16ミリ（60Fps）経つまではイベントを処理する。
			constexpr double maxTime = 16.0;
			if (maxTime <= Stopwatch.GetRap(TimePrecision::Milli))
			{
				break;
			}
		}

		// タイムアウト時に残っているタスクを次のキューに追加しておく。
		while (EventQueues[numQueue].size() != 0)
		{
			auto& event = EventQueues[numQueue].front();
			EventQueues[NumActiveQueue].push_back(event);
			EventQueues[numQueue].pop_front();
		}
	}

	// 追加、解除処理を記述しているものとしています。
	void AddEventListener(EventListener* InEventListener, int InEventId) 
	{
		if (!EventListenerMap.contains(InEventId))
		{
			EventListenerMap.emplace(InEventId, std::set<EventListener*>());
		}
		EventListenerMap[InEventId].emplace(InEventListener);
	}

	void RemoveEventListener(EventListener* InEventListener, int InEventId) {}

private:

	struct EventData
	{
		int id;
		std::any data;
	};

	// タイムアウトの時間計測用
	Stopwatch Stopwatch;

	// * 現在使用中キュー番号
	uint32_t NumActiveQueue = 0;

	// * 今は重要ではないですが、次の布石のため２つキューを用意
	std::array<std::list<EventData>, 2> EventQueues;

	std::map<int, std::set<EventListener*>> EventListenerMap;
};

void EventListener::RegisterToEventManager()
{
	EventManager::Get().AddEventListener(this, eventId);
}

void EventListener::UnRegisterFromEventManager()
{
	EventManager::Get().RemoveEventListener(this, eventId);
}