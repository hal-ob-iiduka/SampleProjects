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
		// �L���[�̓���ւ�
		const auto numQueue = NumActiveQueue;
		(++NumActiveQueue) %= EventQueues.max_size();

		while (EventQueues[numQueue].size() != 0)
		{
			// �擪����o�^���ꂽ�C�x���g�f�[�^���擾����B
			auto& event = EventQueues[numQueue].front();

			// �w�肳�ꂽ�C�x���g�����s����B
			const auto& listeners = EventListenerMap[event.id];
			for (auto listener : listeners)
			{
				listener->Action(event.data);
			}

			EventQueues[numQueue].pop_front();

			// �^�C���A�E�g���Ă��邩���`�F�b�N
			// �����16�~���i60Fps�j�o�܂ł̓C�x���g����������B
			constexpr double maxTime = 16.0;
			if (maxTime <= Stopwatch.GetRap(TimePrecision::Milli))
			{
				break;
			}
		}

		// �^�C���A�E�g���Ɏc���Ă���^�X�N�����̃L���[�ɒǉ����Ă����B
		while (EventQueues[numQueue].size() != 0)
		{
			auto& event = EventQueues[numQueue].front();
			EventQueues[NumActiveQueue].push_back(event);
			EventQueues[numQueue].pop_front();
		}
	}

	// �ǉ��A�����������L�q���Ă�����̂Ƃ��Ă��܂��B
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

	// �^�C���A�E�g�̎��Ԍv���p
	Stopwatch Stopwatch;

	// * ���ݎg�p���L���[�ԍ�
	uint32_t NumActiveQueue = 0;

	// * ���͏d�v�ł͂Ȃ��ł����A���̕z�΂̂��߂Q�L���[��p��
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