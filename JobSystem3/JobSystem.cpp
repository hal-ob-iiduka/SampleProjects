#include "JobSystem.h"

void JobSystem::InitSystem()
{
	// 起動！（これはm_threadの起動より前に呼ぶ必要がある）
	m_isRunning = true;

	// 別スレッド用ジョブループを開始します。
	m_thread = std::thread(&JobSystem::ThreadLoop, this);
}

void JobSystem::TermSystem()
{
	// ThreadLoop関数に対して終了を通知する
	std::unique_lock<std::mutex> lock(m_mutex);
	m_isRunning = false;
	lock.unlock();

	// 待機中の可能性があるため起動させる。
	m_conditionVar.notify_all();

	// ThreadLoopが完全に終了するまで待機
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
		// 別スレッド動作の設定が無ければ、普通の実行キューに追加
		m_jobQueue.push(job);
	}
	else
	{
		// 別スレッド設定があれば、別スレッドの実行キューに追加

		// 実行キューに追加する前に排他制御をかける（これなかったらバグります）
		std::unique_lock<std::mutex> lock(m_mutex);
		m_anyThreadjobQueue.push(job);

		// 実行キューにジョブが追加されたことを通知します。
		m_conditionVar.notify_one();
	}
}

void JobSystem::Execute(float deletaTime, FunctionType type)
{
	// 順にキューへの登録を試みます。ここでは、
	// 前提条件がないもののみキューへ追加されます。
	for (auto job : m_jobContainerMap[type])
	{
		job->TryAddQueue();
	}

	// 追加された順にジョブを実行します。
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
		// 実行キューへアクセスする前に排他制御する（これなかったらバグります）
		std::unique_lock<std::mutex> lock(m_mutex);

		// 実行キューにジョブがなかったら追加まで待機するプログラムです。
		m_conditionVar.wait(lock, [this] { return !m_anyThreadjobQueue.empty() || !m_isRunning; });

		// ThreadLoop関数の終了を検知されると、Loop終了
		if (m_anyThreadjobQueue.empty() && !m_isRunning)
		{
			return;
		}

		auto job = m_anyThreadjobQueue.front();
		m_anyThreadjobQueue.pop();

		// ジョブの実行前に必要で、排他制御を解除します（ジョブの追加を許可します）
		lock.unlock();

		printf("Any Thread!!\n");

		// 実行
		job->Execute(0);
	}
}