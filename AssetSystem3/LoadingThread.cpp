#include "LoadingThread.h"
#include "COM.h"
#include "Task.h"

LoadingThread::LoadingThread()
{
	m_thread = std::jthread([this](std::stop_token st) { ThreadLoop(st); });
}

LoadingThread::~LoadingThread()
{
	Stop();
}

void LoadingThread::AddTask(std::shared_ptr<ITask> task)
{
	std::unique_lock<std::mutex> lock(m_mutex);

	m_tasks.emplace(task);

	// Wait����Ă���ꍇ�A�����I�ɋN������K�v������B
	m_conditionVar.notify_one();
}

void LoadingThread::Stop()
{
	m_thread.request_stop();

	// Wait����Ă���ꍇ�A�����I�ɋN������K�v������B
	m_conditionVar.notify_one();

	m_thread.join();
}

void LoadingThread::ThreadLoop(const std::stop_token& st)
{
	// �ǂݍ��ݏ������Ŏg�p����邽�߁ACOM �I�u�W�F�N�g�����B
	// ����Texture�Ȃǂ̏������Ńo�O���o��B�iWindows���菈���j
	COM com(COMID::MULTITHREADED);

	while (true)
	{
		std::shared_ptr<ITask> task;

		std::unique_lock<std::mutex> lock(m_mutex);
		{
			// �^�X�N���V�����ǉ������܂őҋ@�i���Ƀ^�X�N������Ή������Ȃ��j
			m_conditionVar.wait(lock, [this, st] { return !m_tasks.empty() || st.stop_requested(); });

			// �I�����N�G�X�g�̎��͑��I��
			if (st.stop_requested())
			{
				return;
			}

			// �擪�^�X�N����擾�i�����h�~�̂��߂�Pop�������K�v�j
			task = m_tasks.front();
			m_tasks.pop();
		}

		task->Run();
	}
}