#pragma once
#include <set>
#include <tuple>
#include <memory>
#include <algorithm>
#include <functional>
#include "Entity.h"
#include "ArchetypeChunk.h"

class EntityManager
{
public:

	//=====================================
	// Entity Methods
	//=====================================

	Entity CreateEntity()
	{
		if (m_recicledEntities.empty())
		{
			return GenerateID();
		}
		else
		{
			return RecycleID();
		}
	}

	void DestroyEntity(Entity entity)
	{
		// ��������Entity�̃R���|�[�l���g�f�[�^��S�ď���
		RemoveAll(entity);

		m_recicledEntities.insert(entity);
	}

	bool IsValid(Entity entity) const
	{
		return entity < m_entities.size() && !m_recicledEntities.contains(entity);
	}

private:

	Entity GenerateID()
	{
		auto entity = Entity(m_entities.size());
		m_entities.emplace_back(entity);
		return entity;
	}

	Entity RecycleID()
	{
		auto entity = *m_recicledEntities.begin();
		m_recicledEntities.erase(entity);
		return entity;
	}

public:

	//=====================================
	// Component Methods
	//=====================================

	template<class T, typename... Args>
	T& AddComponent(Entity entity, Args&& ...args)
	{
		assert(IsValid(entity));
		auto oldChunk = m_entityChunkMap[entity];
		auto archetype = GetArchetype<T>(oldChunk.lock()->GetArchetype());

		// �V�����f�[�^�i�[�p�`�����N���擾
		auto newChunk = GetOrAddChunk(archetype);
		assert(!newChunk.expired());

		// �ǉ����ꂽ����̏ꍇ�̓Z�b�g�A�b�v
		auto initialized = newChunk.lock()->HasComponentMap();
		if (!initialized)
		{
			newChunk.lock()->SetUpComponentMap<T>(archetype, oldChunk);
		}

		// �f�[�^�ۑ��悪�ς�鎞�̋��ʏ��������s
		MoveChunkData(entity, newChunk, oldChunk);

		// ���ۂ� AddComponent �����s
		return newChunk.lock()->Add<T>(entity, std::forward<Args>(args)...);
	}

	template<class T>
	T& GetComponent(Entity entity)
	{
		assert(IsValid(entity));
		auto chunk = m_entityChunkMap[entity];
		assert(!chunk.expired());
		return chunk.lock()->Get<T>(entity);
	}

	template<class T>
	bool HasComponent(Entity entity)
	{
		assert(IsValid(entity));
		auto chunk = m_entityChunkMap[entity];

		if (chunk.expired())
		{
			return false;
		}

		return chunk.lock()->Has<T>(entity);
	}

	template<class T>
	void RemoveComponent(Entity entity)
	{
		assert(IsValid(entity));
		auto oldChunk = m_entityChunkMap[entity];
		auto archetype = oldChunk.lock()->GetArchetype();

		// �R���|�[�l���g�^�C�v������
		auto typeId = TypeIDGenerator::GetID<T>();
		std::erase(archetype, typeId);

		// �R���|�[�l���g�����O�ɂȂ����ꍇ
		if (archetype.empty())
		{
			RemoveAll(entity);
			return;
		}

		// �V�����f�[�^�i�[�p�`�����N���擾
		auto newChunk = GetOrAddChunk(archetype);
		assert(!newChunk.expired());

		// �ǉ����ꂽ����̏ꍇ�̓Z�b�g�A�b�v
		auto initialized = newChunk.lock()->HasComponentMap();
		if (!initialized)
		{
			newChunk.lock()->SetUpComponentMap<T>(archetype, oldChunk);
		}

		// �f�[�^�ۑ��悪�ς�鎞�̋��ʏ��������s
		MoveChunkData(entity, newChunk, oldChunk);

		// ���ۂ� RemoveComponent �����s�iRemove<T>�ł����l�j
		oldChunk.lock()->RemoveAll(entity);
	}

private:

	void RemoveAll(Entity entity)
	{
		assert(IsValid(entity));
		auto chunk = m_entityChunkMap[entity];

		// ����l�Ȃ�S�Ẵf�[�^������
		if (!chunk.expired())
		{
			chunk.lock()->RemoveAll(entity);
			m_entityChunkMap.erase(entity);
		}
	}

public:

	//=====================================
	// Helper Methods
	//=====================================

	template<class... Args, class Func>
	void ForEach(Func&& func)
	{
		auto&& targetType = GetArchetype<Args...>();

		// �w�肳�ꂽ�^�����A�S�Ẵ`�����N���擾�i�L���b�V�����Ă����������ǂ��j
		std::vector<ArchetypeChunk*> chunkRefs;
		for (auto chunk : m_chunks)
		{
			auto containAll = true;
			auto chunkType = chunk->GetArchetype();

			for (const auto& id : targetType)
			{
				// Archetype�͑S�ă\�[�g����Ă��邽�߁A�����Ȋ֐����g�p�ilog2(last - start) + O(1)�j
				if (std::binary_search(chunkType.begin(), chunkType.end(), id) == false)
				{
					containAll = false;
					break;
				}
			}

			// Chunk���w�肳�ꂽ�^��S�Ď����Ă���Βǉ�
			if (containAll)
			{
				chunkRefs.emplace_back(chunk.get());
			}
		}

		// �w�肳�ꂽ�֐������s
		for (auto chunkRef : chunkRefs)
		{
			for (const auto& entity : chunkRef->GetEntities())
			{
				std::apply(func, std::forward_as_tuple(chunkRef->Get<Args>(entity)...));
			}
		}
	}

private:

	// �w��^��ێ������AArchetypeID�𐶐����擾�iArchetypeID�̏����l��ݒ�o����悤�ɂ��Ă�j
	template<typename... Args>
	Archetype GetArchetype(const Archetype& archetype = {})
	{
		Archetype newArchetype { TypeIDGenerator::GetID<Args>()... };

		// �����l���w�肳��Ă���Βǉ�
		if (!archetype.empty())
		{
			for (auto& typeId : archetype)
			{
				newArchetype.emplace_back(typeId);
			}
		}

		// �����ɕ��בւ��i��r�������ȒP�ɂ��邽�߁j
		std::sort(newArchetype.begin(), newArchetype.end());

		return newArchetype;
	}

	// �`�����N��K���Ԃ��i��΂ɖ߂�l����ɂȂ�Ȃ��j
	std::weak_ptr<ArchetypeChunk> GetOrAddChunk(const Archetype& archetype)
	{
		auto& chunk = m_chunkMap[archetype];

		// chunk ����́iAdd�������j�ꍇ�͏���������B
		if (chunk.expired())
		{
			auto newChunk = std::make_shared<ArchetypeChunk>(archetype);
			m_chunks.emplace_back(newChunk);
			chunk = newChunk;
		}

		assert(!chunk.expired());
		return chunk;
	}

	// �`�����N�f�[�^�̕ۑ����ύX����
	void MoveChunkData(Entity entity, std::weak_ptr<ArchetypeChunk> newChunk, std::weak_ptr<ArchetypeChunk> oldChunk)
	{
		// �ړ��O�̃`�F���N�f�[�^�Ƃ̃����N������
		if (!oldChunk.expired())
		{
			auto& entities = oldChunk.lock()->GetEntities();
			entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.cend());
		}

		// �ړ���̃`�����N�f�[�^�Ƃ������N������
		if (!newChunk.expired())
		{
			auto& entities = newChunk.lock()->GetEntities();
			entities.emplace_back(entity);

			m_entityChunkMap[entity] = newChunk;
		}

		// ���ۂɃ`�����N���f�[�^���ړ�
		if (!oldChunk.expired())
		{
			oldChunk.lock()->Move(entity, newChunk);
		}
	}

private:

	// World�Ő������ꂽEntity�ꗗ�i�����ɂȂ������m���܂ށj
	std::vector<Entity> m_entities;

	// World�ł̍ė��p�҂�Entity�Q�iDestory���ꂽEntity�B�j
	std::set<Entity> m_recicledEntities;

	// World�Ő������ꂽChunk�I�u�W�F�N�g
	std::vector<std::shared_ptr<ArchetypeChunk>> m_chunks;

	// Archetype���Ƃ�Chunk�ւ̃}�b�v�i����L�[�͑��݂��Ȃ����Avector���L�[�Ƃ��Ă��邽��unordered_map�͎g�p�o���Ȃ��B�j
	std::map<Archetype, std::weak_ptr<ArchetypeChunk>> m_chunkMap;

	// Entity����������Chunk�ւ̃}�b�v�i�����悪�Ȃ��ꍇ�� null�j
	std::map<Entity, std::weak_ptr<ArchetypeChunk>> m_entityChunkMap;
};