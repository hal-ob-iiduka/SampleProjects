#pragma once
#include <vector>
#include "Job.h"

class JobContainer
{
public:

	void Register(Job* job)
	{
		Jobs.push_back(job);
	}

	// JobSystem内で範囲for使用するために定義。
	auto begin() { return Jobs.begin(); }
	auto end() { return Jobs.end(); }

private:

	std::vector<Job*> Jobs;
};