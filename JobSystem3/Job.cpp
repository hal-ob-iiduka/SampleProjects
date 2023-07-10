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
		// �O�������������Β����ɃW���u���L���[�ɓo�^����B
		AddQueue();
	}
	else
	{
		// �O�����������Ί����C�x���g��o�^����B
		SetupPrerequisite();
	}
}

void Job::Execute(float deletaTime)
{
	m_func(deletaTime);

	// �o�^���ꂽ�����C�x���g�𔭍s����B
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
	// �O������ƂȂ�W���u�֎����̃C�x���g��o�^���Ă���B
	for (auto prerequisite : m_prerequisites)
	{
		prerequisite->m_completeEvents.push_back([this]()
		{
			PrerequisiteCompleted();
		});
	}

	// �O���������ۑ�
	m_numPrerequisites = m_prerequisites.size();
}

void Job::PrerequisiteCompleted()
{
	--m_numPrerequisites;
	if (m_numPrerequisites == 0)
	{
		// �O��������S�Ċ�������ƃL���[�ɓo�^����
		AddQueue();
	}
}

void Job::AddQueue()
{
	JobSystem::Get().AddQueue(this, m_isThreadSafe);
}