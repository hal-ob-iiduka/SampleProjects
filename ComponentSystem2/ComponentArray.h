#pragma once
#include <map>
#include <memory>
#include <vector>
#include <assert.h>

// ComponentArrayがテンプレートクラスであるため、
// 通常は、静的タイミングでしかクラスを定義出来ない。（ComponentArray<T> test; みたいな）
// しかし、動的にComponentArrayの型を変更出来る用にしたかったので、このクラスを作成。
struct IComponentArray
{
	virtual ~IComponentArray() = default;

	virtual std::unique_ptr<IComponentArray> Create() = 0;

	virtual void Remove(size_t id) = 0;

	virtual bool Has(size_t id) = 0;

	virtual void Move(size_t id, IComponentArray* components) = 0;
};

template<class T>
struct ComponentArray : public IComponentArray
{
private:
	static inline size_t nullPosition { std::numeric_limits<Entity>::max() };

public:

	std::unique_ptr<IComponentArray> Create() override
	{
		return std::make_unique<ComponentArray<T>>();
	}

	template<typename ...Args>
	T& Add(size_t id, Args&& ...args)
	{
		if (id >= m_toDense.size())
		{
			m_toDense.resize(id + 1);
		}

		// アクセス用変数の初期化
		m_toDense[id] = m_datas.size();
		m_toSparse.emplace_back(id);

		// コンポーネントの生成 & 末端に追加
		m_datas.emplace_back(std::forward<Args>(args)...);

		return m_datas.back();
	}

	T& Get(size_t id)
	{
		return m_datas[m_toDense[id]];
	}

	void Remove(size_t id) override
	{
		// 元々データを持ってない
		if (std::binary_search(m_toDense.begin(), m_toDense.end(), id) == false)
		{
			return;
		}

		// 消去するデータ位置に末端データをコピー（消去処理を簡単にしてる）
		{
			const auto denseIdx = m_toDense[id];
			const auto sparseIdx = m_toSparse.back();

			// 実際にコピー
			m_toDense[sparseIdx] = denseIdx;
			m_toSparse[denseIdx] = sparseIdx;
			m_datas[denseIdx] = std::move(m_datas.back());
		}

		// 末端データを消去
		m_datas.pop_back();
		m_toSparse.pop_back();
		m_toDense[id] = nullPosition;
	}

	bool Has(size_t id) override
	{
		return std::binary_search(m_toDense.begin(), m_toDense.end(), id);
	}

	void Move(size_t id, IComponentArray* ptr) override
	{
		auto components = dynamic_cast<ComponentArray*>(ptr);
		assert(components);

		// 新しい配列に追加（データ持ち越し）＆この配列内のデータを消去
		const auto denseIdx = m_toDense[id];
		components->Add(id, m_datas[denseIdx]);
		Remove(id);
	}

public:

	// 範囲forを使用出来る用にするために定義。
	auto begin() { return m_datas.begin(); }
	auto end() { return m_datas.end(); }

private:

	// コンポーネントデータ配列（DenseIndexでアクセスする）
	std::vector<T> m_datas;

	// DenseIndex -> ID に変換
	std::vector<size_t> m_toSparse;

	// ID -> DenseIndex に変換（これをMapで管理するとビックリするくらい重くなるので試してみてください）
	std::vector<size_t> m_toDense;
};