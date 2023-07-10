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

	// JobSystem���Ŕ͈�for�g�p���邽�߂ɒ�`�B
	auto begin() { return Jobs.begin(); }
	auto end() { return Jobs.end(); }

private:

	std::vector<Job*> Jobs;
};