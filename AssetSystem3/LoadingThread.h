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

	/** タスクを追加 */
	void AddTask(std::shared_ptr<ITask> task);

private:

	/** LoadingThreadを停止させる。（処理終了まで、ブロッキング処理が入る）*/
	void Stop();

	/** 実際のThread処理を記載している関数 */
	void ThreadLoop(const std::stop_token& st);

private:

	/** スレッド操作されるタスクキュー */
	std::queue<std::shared_ptr<ITask>> m_tasks;

	/** ロード処理を行う実際のスレッド */
	std::jthread m_thread;

	/** 競合防止のロック処理に必要 */
	std::mutex m_mutex;

	/** Threadの待機処理などを実現させるために必要 */
	std::condition_variable m_conditionVar;
};