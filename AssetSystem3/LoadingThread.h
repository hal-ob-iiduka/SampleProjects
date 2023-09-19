#pragma once
#include <queue>
#include <memory>
#include <mutex>
#include <thread>

class ITask;

class LoadingThread
{
private:

	LoadingThread();

public:

	static LoadingThread& Get()
	{
		static LoadingThread instance;
		return instance;
	}

	~LoadingThread();

	/** �^�X�N��ǉ� */
	void AddTask(std::shared_ptr<ITask> task);

private:

	/** LoadingThread���~������B�i�����I���܂ŁA�u���b�L���O����������j*/
	void Stop();

	/** ���ۂ�Thread�������L�ڂ��Ă���֐� */
	void ThreadLoop(const std::stop_token& st);

private:

	/** �X���b�h���삳���^�X�N�L���[ */
	std::queue<std::shared_ptr<ITask>> m_tasks;

	/** ���[�h�������s�����ۂ̃X���b�h */
	std::jthread m_thread;

	/** �����h�~�̃��b�N�����ɕK�v */
	std::mutex m_mutex;

	/** Thread�̑ҋ@�����Ȃǂ����������邽�߂ɕK�v */
	std::condition_variable m_conditionVar;
};