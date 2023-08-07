#pragma once
#include <vector>

using TypeID = size_t;

class TypeIDGenerator
{
public:

	template<class T>
	static TypeID GetID()
	{
		static TypeID typeId = m_typeCounter++;
		return typeId;
	}

private:

	static inline size_t m_typeCounter = 0;
};

// コンポーネントタイプの組み合わせ情報を保持
using Archetype = std::vector<TypeID>;