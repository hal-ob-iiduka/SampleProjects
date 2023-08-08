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
		// 消去するEntityのコンポーネントデータを全て消去
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
		auto archetype = GetArchetype<T>(oldChunk);

		// 新しいデータ格納用チャンクを取得
		auto newChunk = GetOrAddChunk(archetype);
		assert(!newChunk.expired());

		// 追加された直後の場合はセットアップ
		auto initialized = newChunk.lock()->HasComponentMap();
		if (!initialized)
		{
			newChunk.lock()->SetUpComponentMap<T>(archetype, oldChunk);
		}

		// データ保存先が変わる時の共通処理を実行
		MoveChunkData(entity, newChunk, oldChunk);

		// 実際に AddComponent を実行
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

		// コンポーネントタイプを消去
		auto typeId = TypeIDGenerator::GetID<T>();
		std::erase(archetype, typeId);

		// コンポーネント数が０になった場合
		if (archetype.empty())
		{
			RemoveAll(entity);
			return;
		}

		// 新しいデータ格納用チャンクを取得
		auto newChunk = GetOrAddChunk(archetype);
		assert(!newChunk.expired());

		// 追加された直後の場合はセットアップ
		auto initialized = newChunk.lock()->HasComponentMap();
		if (!initialized)
		{
			newChunk.lock()->SetUpComponentMap<T>(archetype, oldChunk);
		}

		// データ保存先が変わる時の共通処理を実行
		MoveChunkData(entity, newChunk, oldChunk);

		// 実際に RemoveComponent を実行（Remove<T>でも同様）
		oldChunk.lock()->RemoveAll(entity);
	}

private:

	void RemoveAll(Entity entity)
	{
		assert(IsValid(entity));
		auto chunk = m_entityChunkMap[entity];

		// 正常値なら全てのデータを消去
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

		// 指定された型を持つ、全てのチャンクを取得（キャッシュしておいた方が良い）
		std::vector<ArchetypeChunk*> chunkRefs;
		for (auto chunk : m_chunks)
		{
			auto containAll = true;
			auto chunkType = chunk->GetArchetype();

			for (const auto& id : targetType)
			{
				// Archetypeは全てソートされているため、高速な関数を使用（log2(last - start) + O(1)）
				if (std::binary_search(chunkType.begin(), chunkType.end(), id) == false)
				{
					containAll = false;
					break;
				}
			}

			// Chunkが指定された型を全て持っていれば追加
			if (containAll)
			{
				chunkRefs.emplace_back(chunk.get());
			}
		}

		// 指定された関数を実行
		for (auto chunkRef : chunkRefs)
		{
			for (const auto& entity : chunkRef->GetEntities())
			{
				std::apply(func, std::forward_as_tuple(chunkRef->Get<Args>(entity)...));
			}
		}
	}

private:

	// 指定型を保持した、ArchetypeIDを生成＆取得（ArchetypeIDの初期値を設定出来るようにしてる）
	template<typename... Args>
	Archetype GetArchetype(std::weak_ptr<ArchetypeChunk> chunk = {})
	{
		Archetype archetype{ TypeIDGenerator::GetID<Args>()... };

		// Chunkが正常値なら、Chunkが持つArchetypeを追加
		if (!chunk.expired())
		{
			for (auto& typeId : chunk.lock()->GetArchetype())
			{
				archetype.emplace_back(typeId);
			}
		}

		// 昇順に並べ替え（比較処理を簡単にするため）
		std::sort(archetype.begin(), archetype.end());

		return archetype;
	}

	// チャンクを必ず返す（絶対に戻り値が空にならない）
	std::weak_ptr<ArchetypeChunk> GetOrAddChunk(const Archetype& archetype)
	{
		auto& chunk = m_chunkMap[archetype];

		// chunk が空の（Addだった）場合は初期化する。
		if (chunk.expired())
		{
			auto newChunk = std::make_shared<ArchetypeChunk>(archetype);
			m_chunks.emplace_back(newChunk);
			chunk = newChunk;
		}

		assert(!chunk.expired());
		return chunk;
	}

	// チャンクデータの保存先を変更する
	void MoveChunkData(Entity entity, std::weak_ptr<ArchetypeChunk> newChunk, std::weak_ptr<ArchetypeChunk> oldChunk)
	{
		// 移動前のチェンクデータとのリンクを解除
		if (!oldChunk.expired())
		{
			auto& entities = oldChunk.lock()->GetEntities();
			entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.cend());
		}

		// 移動先のチャンクデータとをリンクさせる
		if (!newChunk.expired())
		{
			auto& entities = newChunk.lock()->GetEntities();
			entities.emplace_back(entity);

			m_entityChunkMap[entity] = newChunk;
		}

		// 実際にチャンク内データを移動
		if (!oldChunk.expired())
		{
			oldChunk.lock()->Move(entity, newChunk);
		}
	}

private:

	// Worldで生成されたEntity一覧（無効になったモノも含む）
	std::vector<Entity> m_entities;

	// Worldでの再利用待ちEntity群（DestoryされたEntity達）
	std::set<Entity> m_recicledEntities;

	// Worldで生成されたChunkオブジェクト
	std::vector<std::shared_ptr<ArchetypeChunk>> m_chunks;

	// ArchetypeごとのChunkへのマップ（同一キーは存在しないが、vectorをキーとしているためunordered_mapは使用出来ない。）
	std::map<Archetype, std::weak_ptr<ArchetypeChunk>> m_chunkMap;

	// Entityが所属するChunkへのマップ（所属先がない場合は null）
	std::map<Entity, std::weak_ptr<ArchetypeChunk>> m_entityChunkMap;
};