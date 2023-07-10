#pragma once
#include <map>
#include <queue>
#include <thread>
#include <mutex>
#include "JobContainer.h"

enum class FunctionType
{
	PreUpdate, 	// 更新の一番最初に呼ばれる
	Update, 	// 普通の更新タイミング
	LateUpdate,	// 更新中の最後で呼ばれる
};

class JobSystem
{
public:

	static JobSystem& Get()
	{
		static JobSystem instance;
		return instance;
	}

	// 初期化、終了用の関数
	void Initialize();
	void DeInitialize();

	// ジョブの登録用関数
	void AddJob(FunctionType type, Job* job);

	// ジョブを実行待ちキューへ追加する関数
	void AddQueue(Job* job, bool isThreadSafe);

	// 実行関数
	void Execute(float deletaTime, FunctionType type);

private:

	// 別スレッド用のジョブ実行用関数
	void ThreadLoop();

private:

	std::map<FunctionType, JobContainer> m_jobContainerMap;

	// 実行待ちジョブキュー
	std::queue<Job*> m_jobQueue;

	// 別スレッドの実行待ちジョブキュー
	std::queue<Job*> m_anyThreadjobQueue;

	// 別スレッドでThreadLoop関数を実行する用
	std::thread m_thread;

	// 排他制御用（別スレッドを使う際にほぼ必ず必要）
	std::mutex m_mutex;

	// ThreadLoopで実行キューに追加されるまで待機するためのクラス
	std::condition_variable m_conditionVar;

	// 別スレッドで実行されているか？
	bool m_isRunning;
};