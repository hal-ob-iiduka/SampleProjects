#pragma once
#include <map>
#include <type_traits>
#include <concepts>
#include <functional>

class DIContainer
{
	using Provider = std::function<void*(DIContainer&)>;
	using TypeId = size_t;
public:

	// Keyの生成設定を登録する関数
	template<class Key, class T>
	void Register()
	{
		Register<Key>([](auto c) { return new T; });
	}

	// Keyの生成設定を登録する関数
	template<class Key>
	void Register(Provider&& func)
	{
		auto typeId = typeid(Key).hash_code();
		auto hasType = m_providerMap.contains(typeId);

		// 二重登録は不可
		if (hasType == true)
		{
			return;
		}

		m_providerMap[typeId] = func;
	}

	// Keyから実際にオブジェクトを取得する関数
	template<class Key>
	Key* Resolve()
	{
		auto typeId = typeid(Key).hash_code();
		auto hasType = m_providerMap.contains(typeId);

		if (hasType == false)
		{
			return nullptr;
		}

		return static_cast<Key*>(m_providerMap[typeId](*this));
	}

private:

	// 生成クラスIdとオブジェクトの生成関数へのマップ
	std::map<TypeId, Provider> m_providerMap;
};