#pragma once
#include <vector>
#include <memory>
#include "Component.h"

class GameObject
{
public:

	virtual ~GameObject() {}

public:

	virtual void Init() {}

	virtual void Uninit() {}

	virtual void Update()
	{
		// ���ۂɃR���|�[�l���g�̏������s��
		std::erase_if(m_components, [](auto& component) 
		{
			// �����\�������ꂽ�R���|�[�l���g�� && �����\�Ȃ�̂�
			return component->RequestDestroy() && component->Erasable();
		});
	}

public:

	template<class T>
	std::weak_ptr<T> AddComponent()
	{
		auto component = std::make_shared<T>(this);
		m_components.emplace_back(component);
		return component;
	}

	template<class T>
	std::weak_ptr<T> GetComponent()
	{
		for (auto& component : m_components)
		{
			if (typeid(*component.get()) == typeid(T))
			{
				// �����҂��̃R���|�[�l���g�͎擾�s�ɂ���B
				if (component->RequestDestroy())
				{
					continue;
				}

				// shared_ptr �̃L���X�g�֐�
				return std::dynamic_pointer_cast<T>(component);
			}
		}

		return std::weak_ptr<T>();
	}

	template<class T>
	void RemoveComponent()
	{
		for (auto& component : m_components)
		{
			if (typeid(*component.get()) == typeid(T))
			{
				// �����ł͎��ۂɂ͏������Ȃ�
				component->Destory();
			}
		}
	}

protected:

	// �Q�[���I�u�W�F�N�g�������i����
	std::vector<std::shared_ptr<Component>> m_components;
};