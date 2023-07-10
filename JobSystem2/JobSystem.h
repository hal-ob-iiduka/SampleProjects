#pragma once
#include <map>
#include <queue>
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

	void AddJob(FunctionType type, Job* job)
	{
		JobContainerMap[type].Register(job);
	}

	void AddQueue(Job* job)
	{
		jobQueue.push(job);
	}

	void Execute(float deletaTime, FunctionType type)
	{
		// 順にキューへの登録を試みます。ここでは、
		// 前提条件がないもののみキューへ追加されます。
		for (auto job : JobContainerMap[type])
		{
			job->TryAddQueue();
		}

		// 追加された順にジョブを実行します。
		while (jobQueue.size())
		{
			auto job = jobQueue.front();
			jobQueue.pop();

			job->Execute(deletaTime);
		}
	}

private:

	std::map<FunctionType, JobContainer> JobContainerMap;

	// 実行待ちジョブキュー
	std::queue<Job*> jobQueue;
};