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
		for (auto system : m_systems)
		{
			system->Init();
		}
	}

	void Update(float deltaTime)
	{
		for (auto system : m_systems)
		{
			system->Update(deltaTime);
		}
	}

public:

	// ���[���h�Ŏg�p�����V�X�e���̓o�^
	template<class T>
	std::weak_ptr<T> RegisterSystem()
	{
		auto system = std::make_shared<T>(this, m_entityManager.get());
		m_systems.emplace_back(system);
		return system;
	}

	// ���[���h�Ŏg�p����Ă���V�X�e���̎擾
	template<class T>
	std::weak_ptr<T> GetSystem()
	{
		for (auto& system : m_systems)
		{
			if (typeid(*system.get()) == typeid(T))
			{
				// shared_ptr �̃L���X�g�֐�
				return std::dynamic_pointer_cast<T>(system);
			}
		}

		return std::weak_ptr<T>();
	}

protected:

	// Entity�ƃR���|�[�l���g���Ǘ�
	std::unique_ptr<EntityManager> m_entityManager;

	// World�ɓo�^���ꂽ�X�V�V�X�e���z��
	std::vector<std::shared_ptr<ISystem>> m_systems;
};