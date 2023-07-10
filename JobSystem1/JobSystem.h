#pragma once
#include <map>
#include "Job.h"
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

	void Execute(float deletaTime, FunctionType type)
	{
		// �֐����o�^����Ă���Ώ�������B
		for (auto job : JobContainerMap[type])
		{
			job->Execute(deletaTime);
		}
	}

private:

	std::map<FunctionType, JobContainer> JobContainerMap;
};