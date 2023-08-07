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

	// ワールドで使用されるシステムの登録
	template<class T>
	std::weak_ptr<T> RegisterSystem()
	{
		auto system = std::make_shared<T>(this, m_entityManager.get());
		m_systems.emplace_back(system);
		return system;
	}

	// ワールドで使用されているシステムの取得
	template<class T>
	std::weak_ptr<T> GetSystem()
	{
		for (auto& system : m_systems)
		{
			if (typeid(*system.get()) == typeid(T))
			{
				// shared_ptr のキャスト関数
				return std::dynamic_pointer_cast<T>(system);
			}
		}

		return std::weak_ptr<T>();
	}

protected:

	// Entityとコンポーネントを管理
	std::unique_ptr<EntityManager> m_entityManager;

	// Worldに登録された更新システム配列
	std::vector<std::shared_ptr<ISystem>> m_systems;
};