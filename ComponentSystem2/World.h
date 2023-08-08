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

	// ワールドで使用されるシステムの登録
	template<class T>
	T* RegisterSystem()
	{
		auto system = new T(this, m_entityManager.get());
		m_systems.emplace_back(system);
		return system;
	}

	// ワールドで使用されているシステムの取得
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

	// Entityとコンポーネントを管理
	std::unique_ptr<EntityManager> m_entityManager;

	// Worldに登録された更新システム配列
	std::vector<std::unique_ptr<ISystem>> m_systems;
};