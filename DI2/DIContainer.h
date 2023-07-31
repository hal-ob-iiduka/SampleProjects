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

	// Key�̐����ݒ��o�^����֐�
	template<class Key, class T>
	void Register()
	{
		Register<Key>([](auto c) { return new T; });
	}

	// Key�̐����ݒ��o�^����֐�
	template<class Key>
	void Register(Provider func)
	{
		auto typeId = typeid(Key).hash_code();
		auto hasType = m_providerMap.contains(typeId);

		// ��d�o�^�͕s��
		if (hasType == true)
		{
			return;
		}

		m_providerMap[typeId] = func;
	}

	// Key������ۂɃI�u�W�F�N�g���擾����֐�
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

	// �����N���XId�ƃI�u�W�F�N�g�̐����֐��ւ̃}�b�v
	std::map<TypeId, Provider> m_providerMap;
};






