#include "JobSystem.h"

void JobSystem::InitSystem()
{
	// �N���I�i�����m_thread�̋N�����O�ɌĂԕK�v������j
	m_isRunning = true;

	// �ʃX���b�h�p�W���u���[�v���J�n���܂��B
	m_thread = std::thread(&JobSystem::ThreadLoop, this);
}

void JobSystem::TermSystem()
{
	// ThreadLoop�֐��ɑ΂��ďI����ʒm����
	std::unique_lock<std::mutex> lock(m_mutex);
	m_isRunning = false;
	lock.unlock();

	// �ҋ@���̉\�������邽�ߋN��������B
	m_conditionVar.notify_all();

	// ThreadLoop�����S�ɏI������܂őҋ@
	m_thread.join();
}

void JobSystem::AddJob(FunctionType type, Job* job)
{
	m_jobContainerMap[type].Register(job);
}

void JobSystem::AddQueue(Job* job, bool isThreadSafe)
{
	if (isThreadSafe == false)
	{
		// �ʃX���b�h����̐ݒ肪������΁A���ʂ̎��s�L���[�ɒǉ�
		m_jobQueue.push(job);
	}
	else
	{
		// �ʃX���b�h�ݒ肪����΁A�ʃX���b�h�̎��s�L���[�ɒǉ�

		// ���s�L���[�ɒǉ�����O�ɔr�������������i����Ȃ�������o�O��܂��j
		std::unique_lock<std::mutex> lock(m_mutex);
		m_anyThreadjobQueue.push(job);

		// ���s�L���[�ɃW���u���ǉ����ꂽ���Ƃ�ʒm���܂��B
		m_conditionVar.notify_one();
	}
}

void JobSystem::Execute(float deletaTime, FunctionType type)
{
	// ���ɃL���[�ւ̓o�^�����݂܂��B�����ł́A
	// �O��������Ȃ����̂̂݃L���[�֒ǉ�����܂��B
	for (auto job : m_jobContainerMap[type])
	{
		job->TryAddQueue();
	}

	// �ǉ����ꂽ���ɃW���u�����s���܂��B
	while (m_jobQueue.size())
	{
		auto job = m_jobQueue.front();
		m_jobQueue.pop();

		job->Execute(deletaTime);
	}
}

void JobSystem::ThreadLoop()
{
	while (true)
	{
		// ���s�L���[�փA�N�Z�X����O�ɔr�����䂷��i����Ȃ�������o�O��܂��j
		std::unique_lock<std::mutex> lock(m_mutex);

		// ���s�L���[�ɃW���u���Ȃ�������ǉ��܂őҋ@����v���O�����ł��B
		m_conditionVar.wait(lock, [this] { return !m_anyThreadjobQueue.empty() || !m_isRunning; });

		// ThreadLoop�֐��̏I�������m�����ƁALoop�I��
		if (m_anyThreadjobQueue.empty() && !m_isRunning)
		{
			return;
		}

		auto job = m_anyThreadjobQueue.front();
		m_anyThreadjobQueue.pop();

		// �W���u�̎��s�O�ɕK�v�ŁA�r��������������܂��i�W���u�̒ǉ��������܂��j
		lock.unlock();

		printf("Any Thread!!\n");

		// ���s
		job->Execute(0);
	}
}