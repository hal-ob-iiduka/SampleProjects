#pragma once
#include <functional>
#include <vector>

class Job
{
public:

	// �W���u�̓o�^�p�֐�
	void SetFunc(std::function<void(float)> func);

	// �O�������ǉ����邽�߂̊֐�
	void AddPrerequisite(Job* job);

	// �W���u�L���[�ւ̓o�^�����݂�B
	// �O��������ݒ肳��Ă���ꍇ�̓L���[�֓o�^����Ȃ��B
	void TryAddQueue();

	// �W���u�̎��s�p�֐�
	void Execute(float deletaTime);

private:

	// �O������̓o�^�p�֐�
	void SetupPrerequisite();

	// �O���������������邲�ƂɌĂ΂��C�x���g�֐�
	void PrerequisiteCompleted();

	// �������g���W���u�L���[�֓o�^����B
	void AddQueue();

private:

	// �W���u����
	std::function<void(float)> m_func;

	// �W���u�I�����C�x���g
	std::vector<std::function<void()>> m_completeEvents;

	// �O������ƂȂ�Job�ꗗ
	std::vector<Job*> m_prerequisites;

	// �c�芮���҂��A�O�������
	int m_numPrerequisites;
};