#pragma once
#include "Entity.h"
#include "Archetype.h"
#include "ComponentArray.h"

// ����Archetype������Entity�̃R���|�[�l���g�f�[�^��ێ�
struct ArchetypeChunk
{
	using ComponentDataMap = std::map<size_t, std::shared_ptr<IComponentArray>>;
public:

	ArchetypeChunk(const Archetype& archetype) : m_archetype(archetype)
	{

	}

public:

	//=====================================
	// Component Methods
	//=====================================

	// entity�̃R���|�[�l���g�f�[�^��ǉ�
	template<class T, typename... Args>
	T& Add(Entity entity, Args&& ...args)
	{
		return GetContainer<T>()->Add(entity, std::forward<Args>(args)...);
	}

	// entity�̃R���|�[�l���g�f�[�^���擾
	template<class T>
	T& Get(Entity entity)
	{
		return GetContainer<T>()->Get(entity);
	}

	// entity�̃R���|�[�l���g�f�[�^���`�F�b�N
	template<class T>
	bool Has(Entity entity)
	{
		return HasContainer<T>() && GetContainer<T>()->Has(entity);
	}

	// entity�̃R���|�[�l���g�f�[�^������
	template<class T>
	void Remove(Entity entity)
	{
		if (HasContainer<T>())
		{
			GetContainer<T>()->Remove(entity);
		}
	}

	// entity�����S�ẴR���|�[�l���g�f�[�^������
	void RemoveAll(Entity entity)
	{
		for (auto& components : m_componentMap)
		{
			components.second->Remove(entity);
		}
	}

	// �w�肳�ꂽchunk�̃R���|�[�l���g�f�[�^���ړ�
	void Move(Entity entity, std::weak_ptr<ArchetypeChunk> chunk)
	{
		assert(!chunk.expired());
		for (auto& component : m_componentMap)
		{
			// �R���e�i��ێ����Ă��邩�H�iRemove���Ɏg�p�j
			if (!chunk.lock()->HasContainer(component.first))
			{
				continue;
			}

			// ���̃I�u�W�F�N�g����chunk�ֈړ�������
			component.second->Move(entity, chunk.lock()->GetContainer(component.first).get());
		}
	}

public:

	//=====================================
	// ComponentMap Methods
	//=====================================

	template<class T>
	void SetUpComponentMap(const Archetype& archetype, std::weak_ptr<ArchetypeChunk> chunk)
	{
		auto targetId = TypeIDGenerator::GetID<T>();
		for (auto& typeId : archetype)
		{
			// �e���v���[�g�^�͕ʓr�������L�q�iAdd���̂ݗL���j
			if (typeId == targetId)
			{
				m_componentMap[typeId] = std::make_shared<ComponentArray<T>>();
				continue;
			}

			// chunk����̎��́A��΂ɏ�̏����Ń��[�v�͏I������
			assert(!chunk.expired());

			// chunk�Ƀf�[�^�z�񂪊��ɂ��邽�߁A�����f�[�^�z��𐶐�
			m_componentMap[typeId] = chunk.lock()->GetContainer(typeId)->Create();
		}
	}

	bool HasComponentMap()
	{
		return !m_componentMap.empty();
	}

private:

	//=====================================
	// Helper Methods
	//=====================================

	template<class T>
	std::shared_ptr<ComponentArray<T>> GetContainer()
	{
		// �R���|�[�l���g�^�C�v�擾
		auto typeId = TypeIDGenerator::GetID<T>();
		assert(m_componentMap.contains(typeId));

		// ComponentArray���w��^�ɃL���X�g�istd::shared_ptr�p�̃L���X�g�֐��j
		auto components = std::dynamic_pointer_cast<ComponentArray<T>>(m_componentMap[typeId]);
		assert(components);

		return components;
	}

	std::shared_ptr<IComponentArray> GetContainer(size_t typeId)
	{
		assert(m_componentMap.contains(typeId));
		return m_componentMap[typeId];
	}

	template<class T>
	bool HasContainer()
	{
		auto typeId = TypeIDGenerator::GetID<T>();
		return m_componentMap.contains(typeId);
	}

	bool HasContainer(size_t typeId)
	{
		return m_componentMap.contains(typeId);
	}

public:

	const Archetype& GetArchetype()
	{
		return m_archetype;
	}

private:

	// ���̃`�����N��Archetype��ێ�
	Archetype m_archetype;

	// �R���|�[�l���g�f�[�^�z��}�b�v
	ComponentDataMap m_componentMap;
};