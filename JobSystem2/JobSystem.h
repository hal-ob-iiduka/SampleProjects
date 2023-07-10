#pragma once
#include <map>
#include <queue>
#include "JobContainer.h"

enum class FunctionType
{
	PreUpdate, 	// �X�V�̈�ԍŏ��ɌĂ΂��
	Update, 	// ���ʂ̍X�V�^�C�~���O
	LateUpdate,	// �X�V���̍Ō�ŌĂ΂��
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
		// ���ɃL���[�ւ̓o�^�����݂܂��B�����ł́A
		// �O��������Ȃ����̂̂݃L���[�֒ǉ�����܂��B
		for (auto job : JobContainerMap[type])
		{
			job->TryAddQueue();
		}

		// �ǉ����ꂽ���ɃW���u�����s���܂��B
		while (jobQueue.size())
		{
			auto job = jobQueue.front();
			jobQueue.pop();

			job->Execute(deletaTime);
		}
	}

private:

	std::map<FunctionType, JobContainer> JobContainerMap;

	// ���s�҂��W���u�L���[
	std::queue<Job*> jobQueue;
};