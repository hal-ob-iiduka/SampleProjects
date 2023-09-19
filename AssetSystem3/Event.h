#pragma once
#include <vector>
#include <functional>

/** 
* UEのイベントっぽいのを実装したクラス 
* 今のところ最低限しか実装していません。
*/
template<class Fty>
class Event
{
public:

	void Add(std::function<Fty>&& callback)
	{
		m_events.emplace_back(std::move(callback));
	}

	void Clear()
	{
		m_events.clear();
		m_events.shrink_to_fit();
	}

	template<class ...Args>
	void Execute(Args... prams) const
	{
		for (auto& func : m_events)
		{
			func(prams...);
		}
	}

private:

	std::vector<std::function<Fty>> m_events;
};

#define FUNC_DECLARE_EVENT( EventName, ReturnType, ... ) \
	using EventName = Event<ReturnType(__VA_ARGS__)>;

#define DECLARE_EVENT( EventName ) FUNC_DECLARE_EVENT(EventName, void)
#define DECLARE_EVENT_PRAM_1( EventName, Pram1 ) FUNC_DECLARE_EVENT(EventName, void, Pram1)
#define DECLARE_EVENT_PRAM_2( EventName, Pram1, Pram2 ) FUNC_DECLARE_EVENT(EventName, void, Pram1, Pram2)
#define DECLARE_EVENT_PRAM_3( EventName, Pram1, Pram2, Pram3 ) FUNC_DECLARE_EVENT(EventName, void, Pram1, Pram2, Pram3)
#define DECLARE_EVENT_PRAM_4( EventName, Pram1, Pram2, Pram3, Pram4 ) FUNC_DECLARE_EVENT(EventName, void, Pram1, Pram2, Pram3, Pram4)
#define DECLARE_EVENT_PRAM_5( EventName, Pram1, Pram2, Pram3, Pram4, Pram5 ) FUNC_DECLARE_EVENT(EventName, void, Pram1, Pram2, Pram3, Pram4, Pram5)
#define DECLARE_EVENT_PRAM_6( EventName, Pram1, Pram2, Pram3, Pram4, Pram5, Pram6 ) FUNC_DECLARE_EVENT(EventName, void, Pram1, Pram2, Pram3, Pram4, Pram5, Pram6)
#define DECLARE_EVENT_PRAM_7( EventName, Pram1, Pram2, Pram3, Pram4, Pram5, Pram6, Pram7 ) FUNC_DECLARE_EVENT(EventName, void, Pram1, Pram2, Pram3, Pram4, Pram5, Pram6, Pram7)
#define DECLARE_EVENT_PRAM_8( EventName, Pram1, Pram2, Pram3, Pram4, Pram5, Pram6, Pram7, Pram8 ) FUNC_DECLARE_EVENT(EventName, void, Pram1, Pram2, Pram3, Pram4, Pram5, Pram6, Pram7, Pram8)
#define DECLARE_EVENT_PRAM_9( EventName, Pram1, Pram2, Pram3, Pram4, Pram5, Pram6, Pram7, Pram8, Pram9 ) FUNC_DECLARE_EVENT(EventName, void, Pram1, Pram2, Pram3, Pram4, Pram5, Pram6, Pram7, Pram8, Pram9)

#define DECLARE_EVENT_RET_PRAM_1( EventName, ReturnType, Pram1 ) FUNC_DECLARE_EVENT(EventName, ReturnType, Pram1)
#define DECLARE_EVENT_RET_PRAM_2( EventName, ReturnType, Pram1, Pram2 ) FUNC_DECLARE_EVENT(EventName, ReturnType, Pram1, Pram2)
#define DECLARE_EVENT_RET_PRAM_3( EventName, ReturnType, Pram1, Pram2, Pram3 ) FUNC_DECLARE_EVENT(EventName, ReturnType, Pram1, Pram2, Pram3)
#define DECLARE_EVENT_RET_PRAM_4( EventName, ReturnType, Pram1, Pram2, Pram3, Pram4 ) FUNC_DECLARE_EVENT(EventName, ReturnType, Pram1, Pram2, Pram3, Pram4)
#define DECLARE_EVENT_RET_PRAM_5( EventName, ReturnType, Pram1, Pram2, Pram3, Pram4, Pram5 ) FUNC_DECLARE_EVENT(EventName, ReturnType, Pram1, Pram2, Pram3, Pram4, Pram5)
#define DECLARE_EVENT_RET_PRAM_6( EventName, ReturnType, Pram1, Pram2, Pram3, Pram4, Pram5, Pram6 ) FUNC_DECLARE_EVENT(EventName, ReturnType, Pram1, Pram2, Pram3, Pram4, Pram5, Pram6)
#define DECLARE_EVENT_RET_PRAM_7( EventName, ReturnType, Pram1, Pram2, Pram3, Pram4, Pram5, Pram6, Pram7 ) FUNC_DECLARE_EVENT(EventName, ReturnType, Pram1, Pram2, Pram3, Pram4, Pram5, Pram6, Pram7)
#define DECLARE_EVENT_RET_PRAM_8( EventName, ReturnType, Pram1, Pram2, Pram3, Pram4, Pram5, Pram6, Pram7, Pram8 ) FUNC_DECLARE_EVENT(EventName, ReturnType, Pram1, Pram2, Pram3, Pram4, Pram5, Pram6, Pram7, Pram8)
#define DECLARE_EVENT_RET_PRAM_9( EventName, ReturnType, Pram1, Pram2, Pram3, Pram4, Pram5, Pram6, Pram7, Pram8, Pram9 ) FUNC_DECLARE_EVENT(EventName, ReturnType, Pram1, Pram2, Pram3, Pram4, Pram5, Pram6, Pram7, Pram8, Pram9)