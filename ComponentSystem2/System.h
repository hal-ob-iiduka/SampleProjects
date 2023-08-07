#pragma once

class World;
class EntityManager;

class ISystem
{
public:

	ISystem() = delete;
	ISystem(World* world, EntityManager* entityManager) : m_world(world), m_entityManager(entityManager) {}

	virtual ~ISystem() {}

public:

	virtual void Init() = 0;

	virtual void UnInit() = 0;

	virtual void Update(float deltaTime) = 0;

protected:

	// システムが所属するワールド
	World* m_world;

	// システムが所属するワールドが保持するEntityManager
	EntityManager* m_entityManager;
};