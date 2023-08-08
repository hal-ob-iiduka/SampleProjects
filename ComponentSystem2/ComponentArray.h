#pragma once
#include <map>
#include <memory>
#include <vector>
#include <assert.h>

// ComponentArray���e���v���[�g�N���X�ł��邽�߁A
// �ʏ�́A�ÓI�^�C�~���O�ł����N���X���`�o���Ȃ��B�iComponentArray<T> test; �݂����ȁj
// �������A���I��ComponentArray�̌^��ύX�o����p�ɂ����������̂ŁA���̃N���X���쐬�B
struct IComponentArray
{
	virtual ~IComponentArray() = default;

	virtual std::unique_ptr<IComponentArray> Create() = 0;

	virtual void Remove(size_t id) = 0;

	virtual void Move(size_t id, IComponentArray* components) = 0;
};

template<class T>
struct ComponentArray : public IComponentArray
{
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

		// �A�N�Z�X�p�ϐ��̏�����
		m_toDense[id] = m_datas.size();
		m_toSparse.emplace_back(id);

		// �R���|�[�l���g�̐��� & ���[�ɒǉ�
		m_datas.emplace_back(std::forward<Args>(args)...);

		return m_datas.back();
	}

	T& Get(size_t id)
	{
		return m_datas[m_toDense[id]];
	}

	void Remove(size_t id) override
	{
		// ���X�f�[�^�������ĂȂ�
		if (std::binary_search(m_toDense.begin(), m_toDense.end(), id) == false)
		{
			return;
		}

		// ��������f�[�^�ʒu�ɖ��[�f�[�^���R�s�[�i�����������ȒP�ɂ��Ă�j
		{
			const auto denseIdx = m_toDense[id];
			const auto sparseIdx = m_toSparse.back();

			// ���ۂɃR�s�[
			m_toDense[sparseIdx] = denseIdx;
			m_toSparse[denseIdx] = sparseIdx;
			m_datas[denseIdx] = std::move(m_datas.back());
		}

		// ���[�f�[�^������
		m_datas.pop_back();
		m_toSparse.pop_back();
		m_toDense[id] = null_entity;
	}

	void Move(size_t id, IComponentArray* ptr) override
	{
		auto components = dynamic_cast<ComponentArray*>(ptr);
		assert(components);

		// �V�����z��ɒǉ��i�f�[�^�����z���j�����̔z����̃f�[�^������
		const auto denseIdx = m_toDense[id];
		components->Add(id, m_datas[denseIdx]);
		Remove(id);
	}

public:

	// �͈�for���g�p�o����p�ɂ��邽�߂ɒ�`�B
	auto begin() { return m_datas.begin(); }
	auto end() { return m_datas.end(); }

private:

	// �R���|�[�l���g�f�[�^�z��iDenseIndex�ŃA�N�Z�X����j
	std::vector<T> m_datas;

	// DenseIndex -> ID �ɕϊ�
	std::vector<size_t> m_toSparse;

	// ID -> DenseIndex �ɕϊ��i�����Map�ŊǗ�����ƃr�b�N�����邭�炢�d���Ȃ�̂Ŏ����Ă݂Ă��������j
	std::vector<size_t> m_toDense;
};