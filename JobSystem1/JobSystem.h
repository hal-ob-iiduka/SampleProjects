#pragma once
#include <map>
#include "Job.h"
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

	void Execute(float deletaTime, FunctionType type)
	{
		// 関数が登録されていれば処理する。
		for (auto job : JobContainerMap[type])
		{
			job->Execute(deletaTime);
		}
	}

private:

	std::map<FunctionType, JobContainer> JobContainerMap;
};