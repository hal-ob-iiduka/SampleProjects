#pragma once

class GameObject;

class Component
{
public:

	Component() = delete;
	Component(GameObject* owner) { m_owner = owner; }

	virtual ~Component() {}

public:

	virtual void Init() {}

	virtual void UnInit() {}

	virtual void Update() {}

	// �����\������t��֐��ŏ����҂���Ԃɓ���܂��B
	virtual void Destory()
	{
		m_requestDestroy = true;
	}

	// ���̃R���|�[�l���g�����ݏ����\�Ȃ̂���Ԃ��܂��B
	virtual bool Erasable()
	{
		return true;
	}

	bool RequestDestroy()
	{
		return m_requestDestroy;
	}

protected:

	// �����Q�[���I�u�W�F�N�g
	GameObject* m_owner = nullptr;

	// ���̃R���|�[�l���g�������O�ł��邩
	bool m_requestDestroy = false;
};