#pragma once
#include <memory>
#include "System.h"
#include "EntityManager.h"

class World
{
public:

	World() : m_entityManager(std::make_unique<EntityManager>()) {}
	virtual ~World() {}

public:

	void Init()
	{
		for (auto& system : m_systems)
		{
			system->Init();
		}
	}

	void Update(float deltaTime)
	{
		for (auto& system : m_systems)
		{
			system->Update(deltaTime);
		}
	}

public:

	// ���[���h�Ŏg�p�����V�X�e���̓o�^
	template<class T>
	T* RegisterSystem()
	{
		auto system = new T(this, m_entityManager.get());
		m_systems.emplace_back(system);
		return system;
	}

	// ���[���h�Ŏg�p����Ă���V�X�e���̎擾
	template<class T>
	T* GetSystem()
	{
		for (auto& system : m_systems)
		{
			if (typeid(*system.get()) == typeid(T))
			{
				return static_cast<T*>(system.get());
			}
		}

		return nullptr;
	}

protected:

	// Entity�ƃR���|�[�l���g���Ǘ�
	std::unique_ptr<EntityManager> m_entityManager;

	// World�ɓo�^���ꂽ�X�V�V�X�e���z��
	std::vector<std::unique_ptr<ISystem>> m_systems;
};