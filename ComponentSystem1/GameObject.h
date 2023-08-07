#pragma once
#include <vector>
#include <memory>
#include "Component.h"

class GameObject
{
public:

	virtual ~GameObject() {}

public:

	virtual void Init() {}

	virtual void Uninit() {}

	virtual void Update()
	{
		// 実際にコンポーネントの消去を行う
		std::erase_if(m_components, [](auto& component) 
		{
			// 消去申請がされたコンポーネントで && 消去可能なやつのみ
			return component->RequestDestroy() && component->Erasable();
		});
	}

public:

	template<class T>
	std::weak_ptr<T> AddComponent()
	{
		auto component = std::make_shared<T>(this);
		m_components.emplace_back(component);
		return component;
	}

	template<class T>
	std::weak_ptr<T> GetComponent()
	{
		for (auto& component : m_components)
		{
			if (typeid(*component.get()) == typeid(T))
			{
				// 消去待ちのコンポーネントは取得不可にする。
				if (component->RequestDestroy())
				{
					continue;
				}

				// shared_ptr のキャスト関数
				return std::dynamic_pointer_cast<T>(component);
			}
		}

		return std::weak_ptr<T>();
	}

	template<class T>
	void RemoveComponent()
	{
		for (auto& component : m_components)
		{
			if (typeid(*component.get()) == typeid(T))
			{
				// ここでは実際には消去しない
				component->Destory();
			}
		}
	}

protected:

	// ゲームオブジェクトが持つ部品たち
	std::vector<std::shared_ptr<Component>> m_components;
};