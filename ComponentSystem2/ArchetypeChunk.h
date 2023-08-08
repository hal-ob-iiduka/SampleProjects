#pragma once
#include "Entity.h"
#include "Archetype.h"
#include "ComponentArray.h"

// 同じArchetypeを持つEntityのコンポーネントデータを保持
struct ArchetypeChunk
{
	using ComponentDataMap = std::map<TypeID, std::unique_ptr<IComponentArray>>;
public:

	ArchetypeChunk(const Archetype& archetype) : m_archetype(archetype)
	{

	}

public:

	//=====================================
	// Component Methods
	//=====================================

	// entityのコンポーネントデータを追加
	template<class T, typename... Args>
	T& Add(Entity entity, Args&& ...args)
	{
		return GetContainer<T>()->Add(entity, std::forward<Args>(args)...);
	}

	// entityのコンポーネントデータを取得
	template<class T>
	T& Get(Entity entity)
	{
		return GetContainer<T>()->Get(entity);
	}

	// entityのコンポーネントデータをチェック
	template<class T>
	bool Has(Entity entity)
	{
		return HasContainer<T>() && GetContainer<T>()->Has(entity);
	}

	// entityのコンポーネントデータを消去
	template<class T>
	void Remove(Entity entity)
	{
		if (HasContainer<T>())
		{
			GetContainer<T>()->Remove(entity);
		}
	}

	// entityが持つ全てのコンポーネントデータを消去
	void RemoveAll(Entity entity)
	{
		for (auto& components : m_componentMap)
		{
			components.second->Remove(entity);
		}
	}

	// 指定されたchunkのコンポーネントデータを移動
	void Move(Entity entity, std::weak_ptr<ArchetypeChunk> chunk)
	{
		assert(!chunk.expired());
		for (auto& component : m_componentMap)
		{
			// コンテナを保持しているか？（Remove時に使用）
			if (!chunk.lock()->HasContainer(component.first))
			{
				continue;
			}

			// このオブジェクトからchunkへ移動させる
			component.second->Move(entity, chunk.lock()->GetContainer(component.first));
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
			// テンプレート型は別途処理を記述（Add時のみ有効）
			if (typeId == targetId)
			{
				m_componentMap[typeId] = std::make_unique<ComponentArray<T>>();
				continue;
			}

			// chunkが空の時は、絶対に上の処理でループは終了する
			assert(!chunk.expired());

			// chunkにデータ配列が既にあるため、同じデータ配列を生成
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
	ComponentArray<T>* GetContainer()
	{
		// コンポーネントタイプ取得
		auto typeId = TypeIDGenerator::GetID<T>();
		assert(m_componentMap.contains(typeId));

		// ComponentArrayを指定型にキャスト（安全性取るならdynamic_cast）
		//auto components = dynamic_cast<ComponentArray<T>*>(m_componentMap[typeId].get());
		//assert(components);

		// ComponentArrayを指定型にキャスト（速度取るならstatic_cast、このサンプルでは2000ミリ秒ほど変わる）
		return static_cast<ComponentArray<T>*>(m_componentMap[typeId].get());
	}

	IComponentArray* GetContainer(size_t typeId)
	{
		assert(m_componentMap.contains(typeId));
		return m_componentMap[typeId].get();
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

	std::vector<Entity>& GetEntities()
	{
		return m_entities;
	}

	const std::vector<Entity>& GetEntities() const
	{
		return m_entities;
	}

private:

	// このチャンクのArchetypeを保持
	Archetype m_archetype;

	// コンポーネントデータ配列マップ
	ComponentDataMap m_componentMap;

	// チャンクが保持するEntity群
	std::vector<Entity> m_entities;
};