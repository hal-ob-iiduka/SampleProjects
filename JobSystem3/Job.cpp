#include "Job.h"
#include "JobSystem.h"

void Job::SetFunc(std::function<void(float)> func)
{
	m_func = func;
}

void Job::AddPrerequisite(Job* job)
{
	m_prerequisites.push_back(job);
}

void Job::SetThreadSafe(bool isThreadSafe)
{
	m_isThreadSafe = isThreadSafe;
}

void Job::TryAddQueue()
{
	if (m_prerequisites.size() == 0)
	{
		// 前提条件が無ければ直ぐにジョブをキューに登録する。
		AddQueue();
	}
	else
	{
		// 前提条件があれば完了イベントを登録する。
		SetupPrerequisite();
	}
}

void Job::Execute(float deletaTime)
{
	m_func(deletaTime);

	// 登録された完了イベントを発行する。
	for (auto completeEvent : m_completeEvents)
	{
		if (completeEvent)
		{
			completeEvent();
		}
	}
	m_completeEvents.clear();
}

void Job::SetupPrerequisite()
{
	// 前提条件となるジョブへ自分のイベントを登録している。
	for (auto prerequisite : m_prerequisites)
	{
		prerequisite->m_completeEvents.push_back([this]()
		{
			PrerequisiteCompleted();
		});
	}

	// 前提条件数を保存
	m_numPrerequisites = m_prerequisites.size();
}

void Job::PrerequisiteCompleted()
{
	--m_numPrerequisites;
	if (m_numPrerequisites == 0)
	{
		// 前提条件が全て完了するとキューに登録する
		AddQueue();
	}
}

void Job::AddQueue()
{
	JobSystem::Get().AddQueue(this, m_isThreadSafe);
}