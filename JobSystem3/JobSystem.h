#pragma once
#include <map>
#include <queue>
#include <thread>
#include <mutex>
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

	// �������A�I���p�̊֐�
	void Initialize();
	void DeInitialize();

	// �W���u�̓o�^�p�֐�
	void AddJob(FunctionType type, Job* job);

	// �W���u�����s�҂��L���[�֒ǉ�����֐�
	void AddQueue(Job* job, bool isThreadSafe);

	// ���s�֐�
	void Execute(float deletaTime, FunctionType type);

private:

	// �ʃX���b�h�p�̃W���u���s�p�֐�
	void ThreadLoop();

private:

	std::map<FunctionType, JobContainer> m_jobContainerMap;

	// ���s�҂��W���u�L���[
	std::queue<Job*> m_jobQueue;

	// �ʃX���b�h�̎��s�҂��W���u�L���[
	std::queue<Job*> m_anyThreadjobQueue;

	// �ʃX���b�h��ThreadLoop�֐������s����p
	std::thread m_thread;

	// �r������p�i�ʃX���b�h���g���ۂɂقڕK���K�v�j
	std::mutex m_mutex;

	// ThreadLoop�Ŏ��s�L���[�ɒǉ������܂őҋ@���邽�߂̃N���X
	std::condition_variable m_conditionVar;

	// �ʃX���b�h�Ŏ��s����Ă��邩�H
	bool m_isRunning;
};