#include <iostream>
#include "World.h"

struct Transform
{
	float x;
	float y;
	float z;
};

struct RigidBody
{
	float speed;
};

// Entity操作が正常に動作しているかをテストするシステム
class TestSystem : public ISystem
{
public:

	TestSystem(World* world, EntityManager* entityManager) : ISystem(world, entityManager) {}

public:

	void Init() override
	{
		auto entity = m_entityManager->CreateEntity();

		// Add
		{
			auto& t = m_entityManager->AddComponent<Transform>(entity);
			t.x = 10;

			auto& r = m_entityManager->AddComponent<RigidBody>(entity);
			r.speed = 1000;
		}

		// Has
		{
			assert(m_entityManager->HasComponent<Transform>(entity));
			assert(m_entityManager->HasComponent<RigidBody>(entity));
		}

		// Get
		{
			auto& t = m_entityManager->GetComponent<Transform>(entity);
			assert(t.x == 10);

			auto& r = m_entityManager->GetComponent<RigidBody>(entity);
			assert(r.speed == 1000);
		}

		// Remove
		{
			m_entityManager->RemoveComponent<Transform>(entity);
			m_entityManager->RemoveComponent<RigidBody>(entity);
		}

		// Not Has
		{
			assert(!m_entityManager->HasComponent<Transform>(entity));
			assert(!m_entityManager->HasComponent<RigidBody>(entity));
		}

		// Destroy Entity
		{
			assert(m_entityManager->IsValid(entity));
			m_entityManager->DestroyEntity(entity);
			assert(!m_entityManager->IsValid(entity));
		}
	}

	void UnInit() override {}
	void Update(float deltaTime) override {}
};

// RigidBodyのパラメーターからTransformを変化させるシステム
class PhysicsSystem : public ISystem
{
public:

	PhysicsSystem(World* world, EntityManager* entityManager) : ISystem(world, entityManager) {}

public:

	void Init() override
	{
		// ワールドに3体のオブジェクトを生成
		for (auto i = 0; i < 2; i++)
		{
			auto entity = m_entityManager->CreateEntity();
			auto& t = m_entityManager->AddComponent<Transform>(entity);
			auto& r = m_entityManager->AddComponent<RigidBody>(entity);

			// 生成IDでスピードを変えておく
			r.speed = 10 * i;
		}
	}

	void UnInit() override {}

	void Update(float deltaTime) override
	{
		m_entityManager->ForEach<Transform, RigidBody>([&](Transform& trans, RigidBody& rb)
		{
			trans.x += rb.speed * deltaTime;
			printf("x = %f\n", trans.x);
		});
	}
};

int main()
{
	World world;

	// システムを登録
	world.RegisterSystem<TestSystem>();
	world.RegisterSystem<PhysicsSystem>();

	// システムの初期化
	world.Init();

	// ワールドの更新
	for (auto i = 0; i < 10; i++)
	{
		world.Update(0.0166666f);
	}
}