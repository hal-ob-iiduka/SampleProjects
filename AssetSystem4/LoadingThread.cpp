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

	// Waitされている場合、明示的に起動する必要がある。
	m_conditionVar.notify_one();
}

void LoadingThread::Stop()
{
	m_thread.request_stop();

	// Waitされている場合、明示的に起動する必要がある。
	m_conditionVar.notify_one();

	m_thread.join();
}

void LoadingThread::ThreadLoop(const std::stop_token& st)
{
	// 読み込み処理等で使用されるため、COM オブジェクト生成。
	// 特にTextureなどの初期化でバグが出る。（Windows限定処理）
	COM com(COMID::MULTITHREADED);

	while (true)
	{
		std::shared_ptr<ITask> task;

		std::unique_lock<std::mutex> lock(m_mutex);
		{
			// タスクが新しく追加されるまで待機（既にタスクがあれば何もしない）
			m_conditionVar.wait(lock, [this, st] { return !m_tasks.empty() || st.stop_requested(); });

			// 終了リクエストの時は即終了
			if (st.stop_requested())
			{
				return;
			}

			// 先頭タスクから取得（競合防止のためにPop処理が必要）
			task = m_tasks.front();
			m_tasks.pop();
		}

		task->Run();
	}
}